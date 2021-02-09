// Initialize pins
//
// Wheel Motors
const int motorFrontL = 9;   // Front Left Motor
const int motorFrontR = 8;   // Front Right Motor
const int motorBackL = 10;   // Back Left Motor
const int motorBackR = 11;   // Back Right Motor

// UltraSonic Sensors
const int trigPinL = 32;    // Ultrasonic Trig Pin
const int trigPinR = 33;    // Ultrasonic Trig Pin
const int echoPinL = 30;     // Left Ultrasonic Echo Pin
const int echoPinR = 31;      // Right Ultrasonic Echo Pin
long durationL, durationR;
int distanceL, distanceR;

// Buzzer
const int buzz = 22;          // Buzzer Trigger Pin

// Flame Sensors
const int LflameAO = 4;     // Left Flame Analog Pin
const int LflameDO = 40;     // Left Flame Digital Pin
const int MflameAO = 4;     // Middle Flame Analog Pin
const int MflameDO = 42;     // Middle Flame Digital Pin
const int RflameAO = 4;     // Right Flame Analog Pin
const int RflameDO = 44;     // Right Flame Digital Pin


void setup(){
  // Setup Pin Modes
  pinMode(motorFrontL, OUTPUT);
  pinMode(motorFrontR, OUTPUT);
  pinMode(motorBackL, OUTPUT);
  pinMode(motorBackR, OUTPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(echoPinR, INPUT);
  pinMode(LflameDO, INPUT);
  pinMode(MflameDO, INPUT);
  pinMode(RflameDO, INPUT);
  pinMode(buzz, OUTPUT);

  // Serial COM
  Serial.begin(9600);
  digitalWrite(buzz, HIGH);           // Notify about Standby Mode
  delay(50);
  digitalWrite(buzz, LOW);
  delay(100);
}

void loop(){
  // Set Sensors to Standby
  digitalWrite(trigPinL, LOW);
  digitalWrite(trigPinR, LOW);
  digitalWrite(LflameDO, HIGH);
  digitalWrite(MflameDO, HIGH);
  digitalWrite(RflameDO, HIGH);
  delay(1000);

  if ( isfire() == 0 ){
    Serial.println("No fire detected!");
  }
  else if ( isfire() == 1 ) {
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(100);
    Serial.println("Left side fire");
  }
  else if ( isfire() == 2 ) {
    digitalWrite(buzz, HIGH);
    delay(200);
    digitalWrite(buzz, LOW);
    delay(100);
    Serial.println("Middle fire");
  }
  else if ( isfire() == 3 ) {
    digitalWrite(buzz, HIGH);
    delay(300);
    digitalWrite(buzz, LOW);
    delay(100);
    Serial.println("Right side fire");
  }

  if ( obstacle() == 0 ) {
    Serial.println("No obstacles");
  }
  else if ( obstacle() == 1 ) {
    Serial.println("Obstacles on left");
    Serial.print("Left distance: ");
    Serial.println(distanceL);
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(100);
  }
  else if ( obstacle() == 2 ) {
    Serial.println("Obstacles on right");
    Serial.print("Right distance: ");
    Serial.println(distanceR);
    digitalWrite(buzz, HIGH);
    delay(200);
    digitalWrite(buzz, LOW);
    delay(100);
  }
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

int isfire(){
  // Check Flame Sensor
  if ( digitalRead(MflameDO) == LOW ) {
    return 2;
  }
  else if ( digitalRead(LflameDO) == LOW ){
    return 1;
  }
  else if ( digitalRead(RflameDO) == LOW ) {
    return 3;
  }
  else {
    return 0;
  }
}
