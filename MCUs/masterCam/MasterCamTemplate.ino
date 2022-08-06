
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <esp32cam.h>
#include <base64.h>
#define slavePin BIT2
#define slaveRecv BIT13
#define flash BIT4

const char* WIFI_SSID = "";
const char* WIFI_PASS = "";



WebSocketsClient webSocket;


static auto midRes = esp32cam::Resolution::find(350, 530);
//static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }

  auto frame = esp32cam::capture();

  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    webSocket.sendTXT("{\"action\" : \"captureFailure\", \"message\" : \"Capture Fail\" }"); //send a failed capture message so Server responds adequately
    return;
  }


  String imgData = "";
  for (int i = 0; i < static_cast<int>(frame->size()); i++) {
    imgData += frame->data()[i];
    imgData += " ";
  }


  String encoded = base64::encode(imgData); //encode imageData

  webSocket.sendTXT("{\"action\" : \"dataHandling\", \"message\" : \"" + encoded + "\" }");


}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  
    if(type == WStype_DISCONNECTED){
      Serial.printf("[WSc] Disconnected!\n");
    }
    else if(type == WStype_CONNECTED){ 

      Serial.printf("[WSc] Connected to url: %s\n", payload);
      serveJpg();
      
//     
    }
    else if(type == WStype_TEXT){
        Serial.printf("[WSc] get text: %s\n", payload);
        int match = strcmp((const char*)payload, "Match");
        
        
        if(match == 0){ //add functionality so that door does not open when not needed (get data from eeprom on arduino on boot);
          Serial.println("Match, Opening Door");
          REG_WRITE(GPIO_OUT_W1TS_REG, slavePin);
          REG_WRITE(GPIO_OUT_W1TS_REG, flash); //led turns on as a visual confirmation
          webSocket.disconnect();
        }
    }
    

}

void pauseESP(){
 
  while(((REG_READ(GPIO_IN_REG) & BIT13) >> 13 )== 1){
    //delay with infinite loop;
    
    delay(2000);
  }
}

void setup() { 
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(200);
  }

  
  
  


  REG_WRITE(IO_MUX_GPIO13_REG, FUN_IE); // enable input on pin 13;
  
  
  
  //outputs
  REG_SET_BIT(GPIO_ENABLE_REG, flash); 
  REG_SET_BIT(GPIO_ENABLE_REG, slavePin);
  
  pauseESP();
  delay(200);
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(midRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
    if(!ok) REG_WRITE(GPIO_OUT_W1TS_REG, slavePin); //if camera fails, reset
    
    
  }
  
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected to Wifi, Connecting to server. ");
  Serial.println(WiFi.localIP());
  
  
  webSocket.beginSSL("ec2-100-25-117-135.compute-1.amazonaws.com", 8000, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try every 1000 again if connection has failed
  webSocket.setReconnectInterval(1000);

}

void loop() {
  webSocket.loop();
  
}
