# faceLock
A personal project dealing with secure websockets, Amazon ECR + Docker, interesting circuitry, and a facial recognition door lock.

Got covid, got bored, and tried to make an incredibly basic project as difficult as possible. Using an Arduino and ESP32-CAM, I dove into direct I/O port manipulation (to an extent), AWS Lambda until I realized the facial detection library exceeded the max size, AWS ECR + ECS + EC2 + Docker, manual turn on/automatic shut down circuits with transistor logic (stopped designing when I ran out of resistors), EAGLE schematic design, WebSocket Secure (didn't want to port-forward my camera data), 3D-printing crucial design components (the "lock" has to lock somewhere), and countless hours on SO, ESE, Physics Classroom, Serial Print Debug Statements in order to understand what the hell my circuit (and the occasional re-explanation on how a capacitor, which isn't even used, works) was doing!

To sum everything up, 3 months later I have the world's most unreliable door lock and an immeasurable passion for embedded systems! 

Now what is an STM Microntroller and where can I get one?


Here's some photos, enjoy :)

ignore the yellow button on the camera breadboard


![IMG-4077](https://user-images.githubusercontent.com/92798736/183235718-d14b7e8b-ad51-4996-87c2-bf61fb8a1954.jpg)

Cam on
![IMG-4076](https://user-images.githubusercontent.com/92798736/183235720-f368277b-f8e7-425d-9728-79036ea25ba4.jpg)

Cam off
![IMG-4074](https://user-images.githubusercontent.com/92798736/183235722-8c670d0a-a9e7-4233-9efb-a7bf059bfe8d.jpg)

(some peripherals were added that are not included on the schematic, probably because at one point, I had no way to unlock the door from the inside nor turn on the circuit :( silly me!)
