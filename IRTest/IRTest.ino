void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
void loop() {

  Serial.print("IRSensor  ");
  Serial.println(analogRead(A1));
  //Serial.print("IR Sensor 2    ");
  //Serial.println(analogRead(A1));
  if (analogRead(7) == 0)
  {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
}
