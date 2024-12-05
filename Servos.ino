#include <ESP32Servo.h>

#define PIN_TRIG 22
#define PIN_ECHO 21

// Define the servo pins
#define HEADSERVO 25
#define TAILSERVO 27
#define BOX_1 32
#define BOX_2 33

Servo headServo;
Servo tailServo;
Servo boxServo1;
Servo boxServo2;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Attach servos to pins
  headServo.attach(HEADSERVO);
  tailServo.attach(TAILSERVO);
  boxServo1.attach(BOX_1);
  boxServo2.attach(BOX_2);

  // Initial positions
  headServo.write(90);   
  tailServo.write(90);   
  boxServo1.write(0);    
  boxServo2.write(0);    
}

void loop() {
  // Trigger a new measurement
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read the echo time
  float duration = pulseIn(PIN_ECHO, HIGH);
  float distanceCM = duration / 58.0;  

  Serial.print("Distancia en CM: ");
  Serial.println(distanceCM);
 
  if (distanceCM < 100.0) {
    // Move head and tail servos
    headServo.write(45); 
    tailServo.write(135); 
    delay(500);

    headServo.write(135); 
    tailServo.write(45); 
    delay(500);

    headServo.write(90);  
    tailServo.write(90);  

    // Simulate box opening
    boxServo1.write(90);  
    boxServo2.write(90); 
    delay(1000);
  } else{// Close box
    boxServo1.write(0);
    boxServo2.write(180);
    delay(1000);

  }

  delay(1000); // Wait 1 second before the next measurement
}