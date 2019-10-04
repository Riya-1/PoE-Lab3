#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor1= AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);

void setup() {
  // put your setup code here, to run once:
  AFMS.begin();
  motor1->setSpeed(0);
  motor2->setSpeed(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  motor1->run(FORWARD);
  motor2->run(FORWARD);
}
