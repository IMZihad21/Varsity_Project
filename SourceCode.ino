// Include prebuilt library supports
#include <Servo.h>   // Servo motor library header

// Initialize pins
//
// Wheel Motors
const int motorFrontL = 9;   // Front Left Motor
const int motorFrontR = 8;   // Front Right Motor
const int motorBackL = 10;   // Back Left Motor
const int motorBackR = 11;   // Back Right Motor

// UltraSonic Sensors
const int trigPinL = 30;    // Ultrasonic Trig Pin
const int trigPinR = 31;    // Ultrasonic Trig Pin
const int echoPinL = 32;    // Left Ultrasonic Echo Pin
const int echoPinR = 33;    // Right Ultrasonic Echo Pin
long durationL, durationR;
int distanceL, distanceR;

// buzzerer
const int buzzer = 22;        // buzzerer Trigger Pin

// Water Pump
const int pump = 52;        // Water Pump Trigger Pin

// Flame Sensors
const int flameMainAnalog = A11;     // Middle Flame Analog Pin
const int flameLeftAnalog = A10;     // Middle Flame Analog Pin
const int flameRightAnalog = A12;     // Middle Flame Analog Pin
const int flameMain = 41;    // Middle Flame Digital Pin
const int flameLeft = 40;    // Left Flame Digital Pin
const int flameRight = 42;    // Right Flame Digital Pin

// Servo Motor
Servo myservo;               // Create servo object 
int pos = 90;                 // Store servo position 

void setup(){
  // Serial COM
  Serial.begin(9600);
  Serial.println("System Booted!");   // Notify about Standby Boot
  alertTone();

  Serial.println("Setup Pin Modes"); // Setup Pin Modes
  pinMode(motorFrontL, OUTPUT);
  pinMode(motorFrontR, OUTPUT);
  pinMode(motorBackL, OUTPUT);
  pinMode(motorBackR, OUTPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(echoPinR, INPUT);
  pinMode(flameLeft, INPUT);
  pinMode(flameMain, INPUT);
  pinMode(flameRight, INPUT);

  myservo.attach(9);                 // attach the servo on pin 9
}

void loop(){
  // Set Devices to Standby
  digitalWrite(buzzer, LOW);
  digitalWrite(trigPinL, LOW);
  digitalWrite(trigPinR, LOW);
  digitalWrite(flameLeft, HIGH);
  digitalWrite(flameMain, HIGH);
  digitalWrite(flameRight, HIGH);
  myservo.write(pos);
  delay(500);

  // Start Main Loop
  switch ( checkfire() ){
    case 1:
    Serial.println("Fire detected in left sensor, turning left..");
    alertTone();
    moveForward();
    moveLeft();
    delay(200);
    break;

    case 2:
    Serial.println("Fire Detected in Main Sensor, Checking obstacles..");
    alertTone();
    if( analogRead(flameMainAnalog) >= 35 ) {
      switch( obstacle() ){
        case 1:
        Serial.println("Obstacle on left."); // Obstacle on left.
        moveRight();
        moveForward();
        moveForward();
        moveLeft();
        delay(300);
        break;

        case 2:
        Serial.println("Obstacle on right."); // Obstacle on right.
        moveLeft();
        moveForward();
        moveForward();
        moveRight();
        delay(300); 
        break;

        default:
        Serial.println("No obstacles. Moving Forward!");
        moveForward();
        delay(300); 
      }
    }
    while( digitalRead(flameMain) == LOW ) {
      Serial.println("Starting Water Spray");
      pumpStart();
      Serial.println("Starting Servo to spread water spray");
      while( digitalRead(flameMain) == LOW ) {
        servoControl();
      }
      pumpStop();
      alertTone();
    }
    break;

    case 3:
    Serial.println("Fire detected in right sensor, turning right..");
    alertTone();
    moveForward();
    moveRight();
    delay(200);
    break;

    default:
    Serial.println("No fire detected! Waiting in standby mode.");
 }

}

// Additional Tasks

int moveForward() {
  digitalWrite(motorFrontL, HIGH);
  digitalWrite(motorFrontR, HIGH);
  digitalWrite(motorBackL, HIGH);
  digitalWrite(motorBackR, HIGH);
  delay(500);
  digitalWrite(motorFrontL, LOW);
  digitalWrite(motorFrontR, LOW);
  digitalWrite(motorBackL, LOW);
  digitalWrite(motorBackR, LOW);
}

int moveLeft(){
  digitalWrite(motorFrontR, HIGH);
  digitalWrite(motorBackR, HIGH);
  delay(500);
  digitalWrite(motorFrontR, LOW);
  digitalWrite(motorBackR, LOW);
}

int moveRight(){
  digitalWrite(motorFrontL, HIGH);
  digitalWrite(motorBackL, HIGH);
  delay(500);
  digitalWrite(motorFrontL, LOW);
  digitalWrite(motorBackL, LOW);
}

int obstacle(){
  // Trigger Ultrasonics
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  
  durationL = pulseIn(echoPinL, HIGH);
  distanceL = durationL * 0.034 / 2;
  
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  durationR = pulseIn(echoPinR, HIGH);
  distanceR = durationR * 0.034 / 2;

  // Check for obstacles
  if ( distanceL <= 5 ){
    return 1;
  }
  else if ( distanceR <= 5 ){
    return 2;
  }
  else{
    return 0;
  }
}

int checkfire(){
  // Check Flame Sensor Data
  if ( analogRead(flameLeftAnalog) <= 900 || analogRead(flameMainAnalog) <= 900 || analogRead(flameRightAnalog) <= 900 ) 
  {
    if ( analogRead(flameLeftAnalog) < analogRead(flameMainAnalog) && analogRead(flameLeftAnalog) < analogRead(flameRightAnalog) ) 
      return 1;
    else if ( analogRead(flameMainAnalog) < analogRead(flameLeftAnalog) && analogRead(flameMainAnalog) < analogRead(flameRightAnalog) ) 
      return 2;
    else if ( analogRead(flameRightAnalog) < analogRead(flameMainAnalog) && analogRead(flameRightAnalog) < analogRead(flameLeftAnalog) ) 
      return 3;
  }
  else
    return 0;
}

int pumpStart(){
  digitalWrite(pump, HIGH);
  delay(15);
}

int pumpStop(){
    digitalWrite(pump, LOW);
    delay(15);
}

int servoControl(){
  for(pos = 90; pos > 45; pos -= 1)
  {
    myservo.write(pos);
    delay(20);
  } 
  for(pos = 45; pos <= 90; pos += 1)
  {
    myservo.write(pos);
    delay(20);
  }
  for(pos = 90; pos < 135; pos += 1)
  {
    myservo.write(pos);
    delay(20);
  } 
  for(pos = 135; pos >= 90; pos -= 1)
  {
    myservo.write(pos);
    delay(20);
  }
}

int alertTone(){
  tone(buzzer, 500, 100);
  delay(200);
  tone(buzzer, 1000, 250);
}
