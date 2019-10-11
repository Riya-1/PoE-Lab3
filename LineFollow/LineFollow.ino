#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);

int irLeft = A1;
int irRight = A0;
int lVal = 700;
int rVal = 650;
int mSpeed = 25;
int mSpeedChange = 100;

// SERIAL PROTOCOL:
// Format -> [cmd_char][num_value];
// leading 'b' means set base speed
// leading 'a' means set added speed
// leading 'l' means set left ir threshold value
// leading 'r' means set right ir threshold value
// 's' means start streaming ir values
// 'd' means stop streaming ir values
// ';' is terminating character

char rx_byte;
String rx_string = "";
bool receiving_val = false;
char last_cmd_received;
String all_cmds = "balrsd";
bool streaming = false;

void setup() {
  AFMS.begin();
  motorLeft->setSpeed(mSpeed);
  motorRight->setSpeed(mSpeed);

  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  Serial.begin(9600);
}

void loop() {
  update_rx();
  
  if (streaming) {
    Serial.print("IRSensor  ");
    Serial.print(analogRead(A0));
    Serial.print(" ");
    Serial.print(analogRead(A1));
    Serial.println();
  }

  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);
  
  if (analogRead(irLeft) > lVal)
  {
    motorRight->setSpeed(mSpeed + mSpeedChange);
    motorLeft->setSpeed(0);
  }

  else if (analogRead(irRight) > rVal)
  {
    motorLeft->setSpeed(mSpeed + mSpeedChange);
    motorRight->setSpeed(0);
  }

  else {
    motorRight->setSpeed(mSpeed);
    motorLeft->setSpeed(mSpeed);
  }
}

void update_rx() {
  while (Serial.available()) {
    rx_byte = Serial.read();
    
    if (!receiving_val) {
      if (all_cmds.indexOf(rx_byte) > -1) {
        last_cmd_received = rx_byte;
        rx_string = "";
        receiving_val = true;
      }
    } else if (rx_byte == ';') {
      receiving_val = false;
      
      switch (last_cmd_received) {
        case 'b':
          set_base_speed();
          break;
        case 'a':
          set_added_speed();
          break;
        case 'l':
          set_left_ir();
          break;
        case 'r':
          set_right_ir();
          break;
        case 's':
          start_streaming();
          break;
        case 'd':
          stop_streaming();
          break;
      }
    } else if (isdigit(rx_byte)) {
      rx_string += rx_byte;
    }
  }
}

void set_base_speed() {
  mSpeed = rx_string.toInt();
  Serial.print("Base speed set to ");
  Serial.println(mSpeed);
  return;
}

void set_added_speed() {
  mSpeedChange = rx_string.toInt();
  Serial.print("Added speed set to ");
  Serial.println(mSpeedChange);
  return;
}

void set_left_ir() {
  lVal = rx_string.toInt();
  Serial.print("Left IR threshold value set to ");
  Serial.println(lVal);
  return;
}

void set_right_ir() {
  rVal = rx_string.toInt();
  Serial.print("Right IR threshold value set to ");
  Serial.println(rVal);
  return;
}

void start_streaming() {
  streaming = true;
  Serial.println("Streaming enabled");
  return;
}

void stop_streaming() {
  streaming = false;
  Serial.println("Streaming disabled");
  return;
}
