#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);

int irLeft = A1;
int irRight = A0;
int lVal = 900;
int rVal = 750;
int mSpeed = 25;
int mSpeedChange = 5;

void setup() {
  AFMS.begin();
  motorLeft->setSpeed(mSpeed);
  motorRight->setSpeed(mSpeed);

  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  Serial.begin(9600);

}

void loop() {
  Serial.print("IRSensor  ");
  Serial.print(analogRead(A0));
  Serial.print(" ");
  Serial.print(analogRead(A1));
  Serial.println();

  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);
  
  if (analogRead(irLeft) > lVal)
  {
    motorRight->setSpeed(mSpeed + mSpeedChange);
  }

  else if (analogRead(irRight) > rVal)
  {
    motorLeft->setSpeed(mSpeed + mSpeedChange);
  }

  else {
    motorRight->setSpeed(mSpeed);
    motorLeft->setSpeed(mSpeed);
  }
}
