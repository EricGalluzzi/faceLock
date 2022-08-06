#include <Stepper.h>
#include <EEPROM.h>
const int stepsPerRevolution = 32;
const float steps_per_out_rev = stepsPerRevolution * 64;

int stepsRequired = 1174;
unsigned char unlockState; 
unsigned char prevState;


#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define recvPin 6
#define outPin 3
#define toggleLock 5
#define keepOn 7

Stepper steppermotor(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  //output
  DDRB |= (1 << 5);
  PORTB ^= PORTB;
  DDRD = B10001000;
  PORTD = B00100000;
  
  if(EEPROM.read(0) == 255){
    EEPROM.write(0, 0);
    
  } else {
    
    unlockState = EEPROM.read(0);
  }
  
 
  
  if(unlockState == 1){ //if unlockState is 1 (door is unlocked)
 
    PORTD |= (1 << outPin);
  }
}


unsigned long prevMillis = millis();

void loop() {
  
  if((PIND & _BV(recvPin)) >> recvPin == 1){ //resetButton on ESP32 cam can override the lock in case there is no facial match.
    if(unlockState == 0){ 
      PORTB |= (1 << 5); 
      unlockState = 1;
      steppermotor.setSpeed(350);  
      steppermotor.step(stepsRequired);
    }
    //reset when unlockState
    delay(2000);
    terminatePwr();
    
  }
  
   if((PIND & _BV(toggleLock)) >> toggleLock == 0){
    PORTB |= (1 << 5); 
    unlockState ^= 1;
    steppermotor.setSpeed(350);  
    (unlockState == 0) ? steppermotor.step(-stepsRequired) : steppermotor.step(stepsRequired);
    delay(2000);
    terminatePwr();
   }
  
   if(millis() > prevMillis + 60000){ //1 minute global timer
    terminatePwr();
   }
 


  
}

void terminatePwr(){
  
  if(prevState != unlockState) EEPROM.update(0, unlockState); //prevent unnecessary writes
  delay(1000);
  PORTD |= (1 << keepOn);
}


  
