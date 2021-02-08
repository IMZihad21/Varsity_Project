// Autonomous Fire-Fighting Robot Source Code
// Author: Md. Mofajjal Rasul Jehad 
//
// Alarm Buzzer
int alarm = 22;

// Ultrasonic Sensors
int ustrig1 = 30;
int ustrig2 = 31;
int usecho1 = 32;
int usecho2 = 33;
int flame1 = 24;

// Required VARs
float dist1, dist2, dura1, dura2;

void setup(){
  // Serial COM port
  Serial.begin (9600);
  Serial.println("System online!");

  // Setup Pins
  Serial.println("Initializing PINs");
  pinMode(alarm, OUTPUT);
  pinMode(ustrig1, OUTPUT);
  pinMode(ustrig2, OUTPUT);
  pinMode(usecho1, INPUT);
  pinMode(usecho2, INPUT);
  pinMode(flame1, INPUT);

  Serial.println("Setup Complete, Initializing Loop");
}

void loop(){
  // Setup Others
  digitalWrite(alarm, LOW);
  digitalWrite(ustrig1, LOW);
  digitalWrite(ustrig2, LOW);
  
  // Triggering Ultrasonic 1
  digitalWrite(ustrig1, HIGH);
  delay(5);
  digitalWrite(ustrig1, LOW);
  
  // Calculate Distance 1
  dura1 = pulseIn(usecho1, HIGH);
  dist1 = (dura1 / 2) * 0.0343;
  
  // Triggering Ultrasonic 2
  digitalWrite(ustrig2, HIGH);
  delay(5);
  digitalWrite(ustrig2, LOW);
  
  // Calculate Distance 2
  dura2 = pulseIn(usecho2, HIGH);
  dist2 = (dura2 / 2) * 0.0343;
  
  Serial.print("Distance 1 = ");
  if ( dist1 >= 400 || dist1 <= 10 ) {
    digitalWrite(alarm, HIGH);
    delay(250);
    Serial.println("Out of range!");
  }
  else{
    Serial.println(dist1);
  }
  
  Serial.print("Distance 2 = ");
  if ( dist2 >= 400 || dist2 <= 10 ) {
    digitalWrite(alarm, HIGH);
    delay(250);
    Serial.println("Out of range!");
  }
  else{
    Serial.println(dist2);
  }

  // Check for Fire
  if ( digitalRead(flame1) == LOW){
    digitalWrite(alarm, HIGH);
    delay(250);
  }
}
