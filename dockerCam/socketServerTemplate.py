import asyncio
import pathlib
import ssl
import json
import websockets
import pandas as pd
import cv2
import numpy as np
import os
import face_recognition
import base64


classNames = []

    
classNames.append("Eric")
print(classNames)

 

encodeListKnown = []
encodeListKnown.append(np.load("eric.npy"))
print('Encoding Complete')

        

async def connection(websocket, path): #fix connection and handling data
    payload = await websocket.recv()
    try:
        payload = json.loads(payload)
        
        greeting = "active"
    


        if payload["action"] == "dataHandling":
            imgRaw = base64.b64decode(payload["message"]).decode("utf-8")
            imgRaw = imgRaw.split(' ')
            imgRaw.pop()
            imgValues = [i for i in imgRaw]
            imgValues = list(map(int, imgValues))
            

            imgnp = np.array(bytearray(imgValues),dtype=np.uint8)
            img=cv2.imdecode(imgnp,-1)

            imgS = cv2.resize(img, (0, 0), None, 0.25, 0.25)
            imgS = cv2.cvtColor(imgS, cv2.COLOR_BGR2RGB)
            facesCurFrame = face_recognition.face_locations(imgS)
            encodesCurFrame = face_recognition.face_encodings(imgS, facesCurFrame)

            for encodeFace, faceLoc in zip(encodesCurFrame, facesCurFrame):
                matches = face_recognition.compare_faces(encodeListKnown, encodeFace) #if a match exists
                faceDis = face_recognition.face_distance(encodeListKnown, encodeFace)
        # print(faceDis)
                matchIndex = np.argmin(faceDis)

                if matches[matchIndex]: 
                    name = classNames[matchIndex].upper()
                    
                    if name == 'ERIC':
                        print("match")
                        greeting = "Match"
                    else:
                        greeting = "activeNoMatch"
    except:
        
        greeting = "Error"
        

    
    await websocket.send(greeting)
    
    

    

ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
ssl_context.load_cert_chain(
    pathlib.Path(__file__).with_name('server.pem'), keyfile = pathlib.Path(__file__).with_name('private_key.pem'))

start_server = websockets.serve(
    connection, '0.0.0.0', 8000, ssl=ssl_context)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()


