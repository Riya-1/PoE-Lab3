int irLeft = A1;
int irRight = A0;

void setup() {
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  Serial.begin(9600);
}
void loop() {

  Serial.print("IRSensor  ");
  Serial.print(analogRead(irLeft));
  Serial.print(" ");
  Serial.print(analogRead(irRight));
  Serial.println();
}
