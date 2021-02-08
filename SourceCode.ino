int alarm = 22;

void setup(){
  pinMode(alarm, OUTPUT);
}

void loop(){
  digitalWrite(alarm, LOW);
  delay(500);
  digitalWrite(alarm, HIGH);
  delay(200);
}
