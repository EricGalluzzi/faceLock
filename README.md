# faceLock
A personal project dealing with secure websockets, Amazon ECR + Docker, interesting circuitry, and a facial recognition door lock.

Got covid, got bored, and tried to make an incredibly basic project as difficult as possible. Using an Arduino and ESP32-CAM, I dove into direct I/O port manipulation (to an extent), AWS Lambda until I realized the facial detection library exceeded the max size, AWS ECR + ECS + EC2 + Docker, manual turn on/automatic shut down circuits with transistor logic (stopped designing when I ran out of resistors), EAGLE schematic design, WebSocket Secure (didn't want to port-forward my camera data), 3D-printing crucial design components (the "lock" has to lock somewhere), and countless hours on SO, ESE, Physics Classroom, Serial Print Debug Statements in order to understand what the hell my circuit (and the occasional re-explanation on how a capacitor, which isn't even used, works) was doing!

To sum everything up, 3 months later I have the world's most unreliable door lock and an immeasurable passion for embedded systems! 

Now what is an STM Microntroller and where can I get one?

:)
