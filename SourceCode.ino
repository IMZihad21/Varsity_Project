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

// Required VARs
float distance, duration;

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
  
  // Calculate Distance
  duration = pulseIn(usecho1, HIGH);
  distance = (duration / 2) * 0.0343;
  Serial.print("Distance = ");
  if ( distance >= 400 || distance <= 10) {
    digitalWrite(alarm, HIGH);
    delay(400);
    Serial.println("Out of range!");
  }
  else{
    Serial.println(distance);
  }
}
