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
const int echoPinL = 30;    // Left Ultrasonic Echo Pin
const int echoPinR = 31;    // Right Ultrasonic Echo Pin
long durationL, durationR;
int distanceL, distanceR;

// Buzzer
const int buzz = 22;        // Buzzer Trigger Pin

// Water Pump
const int pump = 22;        // Water Pump Trigger Pin

// Flame Sensors
const int flameMainAnalog = A7;     // Middle Flame Analog Pin
const int flameLeftAnalog = A7;     // Middle Flame Analog Pin
const int flameRightAnalog = A7;     // Middle Flame Analog Pin
const int flameMain = 42;    // Middle Flame Digital Pin
const int flameLeft = 40;    // Left Flame Digital Pin
const int flameRight = 44;    // Right Flame Digital Pin


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
  pinMode(flameLeft, INPUT);
  pinMode(flameMain, INPUT);
  pinMode(flameRight, INPUT);
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
  digitalWrite(flameLeft, HIGH);
  digitalWrite(flameMain, HIGH);
  digitalWrite(flameRight, HIGH);
  delay(1000);

  // Start Main Loop
  switch ( checkfire() ){
    case 1:
    Serial.println("Fire detected in left sensor, turning left..");
    navigate(3);
    delay(200);
    break;

    case 2:
    Serial.println("Fire Detected in Main Sensor, moving toward fire..");
    while( analogRead(flameMainAnalog) >= 30 ) {
      switch( obstacle() ){
        case 1:
        navigate(2);
        navigate(1);
        delay(300);
        break;

        case 2:
        navigate(3);
        navigate(1);
        delay(300); 
        break;

        default:
        navigate(1);
        delay(300); 
      }
    }
    while( digitalRead(flameMain) == LOW ) {
      Serial.println("Starting Water Spray");
      pumpwater();
    }
    break;

    case 3:
    Serial.println("Fire detected in right sensor, turning right..");
    navigate(2);
    delay(200);
    break;

    default:
    Serial.println("No fire detected! Waiting in standby mode.");
 }

}

// Additional Tasks

int navigate( int c ){
  switch(c){
    case 1:
    digitalWrite(motorFrontL, HIGH);
    digitalWrite(motorFrontR, HIGH);
    digitalWrite(motorBackL, HIGH);
    digitalWrite(motorBackR, HIGH);
    delay(500);
    digitalWrite(motorFrontL, LOW);
    digitalWrite(motorFrontR, LOW);
    digitalWrite(motorBackL, LOW);
    digitalWrite(motorBackR, LOW);
    break;

    case 2:
    digitalWrite(motorFrontR, HIGH);
    digitalWrite(motorBackR, HIGH);
    delay(500);
    digitalWrite(motorFrontR, LOW);
    digitalWrite(motorBackR, LOW);
    break;

    case 3:
    digitalWrite(motorFrontL, HIGH);
    digitalWrite(motorBackL, HIGH);
    delay(500);
    digitalWrite(motorFrontL, LOW);
    digitalWrite(motorBackL, LOW);
    break;
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

int checkfire(){
  // Check Flame Sensor Data
  if ( analogRead(flameLeftAnalog) <= 1000 || analogRead(flameMainAnalog) <= 1000 || analogRead(flameRightAnalog) <= 1000 ) 
  {
    if ( analogRead(flameMainAnalog) < analogRead(flameLeftAnalog) && analogRead(flameMainAnalog) < analogRead(flameRightAnalog) ) 
      return 2;
    else if ( analogRead(flameLeftAnalog) < analogRead(flameMainAnalog) && analogRead(flameLeftAnalog) < analogRead(flameRightAnalog) ) 
      return 1;
    else if ( analogRead(flameRightAnalog) < analogRead(flameMainAnalog) && analogRead(flameRightAnalog) < analogRead(flameLeftAnalog) ) 
      return 3;
  }
}

int pumpwater(){
  digitalWrite(buzz, HIGH);
  delay(500);
  digitalWrite(buzz, LOW);
  delay(100);
  digitalWrite(pump, HIGH);
  delay(5000);
  digitalWrite(pump, LOW);
  delay(100);
}
