// Import statements
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Setting up the motors with the Adafruit Motor Shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);

// Pin out of IR Sensors
int irLeft = A1;
int irRight = A0;

// Robot constants, which can be changed via serial
int lVal = 700;           // Left IR cutoff value
int rVal = 650;           // Right IR cutoff value
int mSpeed = 25;          // Motor base speed
int mSpeedChange = 100;   // Motor added speed
int rSpeed = mSpeed;      // Setting each motor to base speed
int lSpeed = mSpeed;

// SERIAL PROTOCOL:
// Format -> [cmd_char][num_value];
// leading 'b' means set base speed
// leading 'a' means set added speed
// leading 'l' means set left ir threshold value
// leading 'r' means set right ir threshold value
// 's' means start streaming ir values
// 'd' means stop streaming ir values
// 'p' means start streaming plotting values
// 'o' means stop streaming plotting values
// ';' is terminating character


// Serial control variables
char rx_byte;
String rx_string = "";
bool receiving_val = false;
char last_cmd_received;
String all_cmds = "balrsdpo";
bool streaming = false;
bool plotStreaming = false;



void setup() {
  // Start the motors & set up the IR pins
  AFMS.begin();
  motorLeft->setSpeed(rSpeed);
  motorRight->setSpeed(lSpeed);
  delay(2000);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  Serial.begin(9600);
}

void loop() {
  update_rx();

  // Begin streaming IR Values (for debugging)
  if (streaming) {
    Serial.print("IRSensor  ");
    Serial.print(analogRead(irLeft));
    Serial.print(" ");
    Serial.print(analogRead(irRight));
    Serial.println();
  }

  // Begin streaming IR Values & their corresponding motor values for plotting
  if (plotStreaming) {
    Serial.print(analogRead(irLeft));     Serial.print(",");
    Serial.print(analogRead(irRight));     Serial.print(",");
    Serial.print(lSpeed);     Serial.print(",");
    Serial.println(rSpeed);
  }

  // Run the motors
  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);

  // If the Left IR Sensor Value is greater than the threshold
  if (analogRead(irLeft) > lVal)
  {
    // Increase the right motor speed & set the left motor to zero
    rSpeed = mSpeed + mSpeedChange; lSpeed = 0;
    motorRight->setSpeed(rSpeed);
    motorLeft->setSpeed(lSpeed);
  }

  // If the Right IR Sensor Value is greater than the threshold
  else if (analogRead(irRight) > rVal)
  {
    // Increase the left motor speed & set the right motor to zero
    lSpeed = mSpeed + mSpeedChange; rSpeed = 0;
    motorLeft->setSpeed(lSpeed);
    motorRight->setSpeed(rSpeed);
  }

  // Otherwise, set both motors to base speed 
  else {
    rSpeed = mSpeed; lSpeed = mSpeed;
    motorRight->setSpeed(mSpeed);
    motorLeft->setSpeed(mSpeed);
  }
}

// Function to handle serial inputs
void update_rx() {

  // While there character available to read
  while (Serial.available()) {
    rx_byte = Serial.read();

    // Check for a valid command
    if (!receiving_val) {
      if (all_cmds.indexOf(rx_byte) > -1) {
        last_cmd_received = rx_byte;
        rx_string = "";
        receiving_val = true;
      }
    } 
    // If the command is complete, start processing
    else if (rx_byte == ';') {
      receiving_val = false;

      // Call the respective method based on the command
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
        case 'p':
          start_plotting_stream();
          break;
        case 'o':
          stop_plotting_stream();
          break;
      }
    } 

    // If a number is typed, store it
    else if (isdigit(rx_byte)) {
      rx_string += rx_byte;
    }
  }
}

// Set the base speed of the motors
void set_base_speed() {
  mSpeed = rx_string.toInt();
  Serial.print("Base speed set to ");
  Serial.println(mSpeed);
  return;
}

// Set the added speed to the motors
void set_added_speed() {
  mSpeedChange = rx_string.toInt();
  Serial.print("Added speed set to ");
  Serial.println(mSpeedChange);
  return;
}

// Set the left IR cutoff value
void set_left_ir() {
  lVal = rx_string.toInt();
  Serial.print("Left IR threshold value set to ");
  Serial.println(lVal);
  return;
}


// Set the right IR cutoff value
void set_right_ir() {
  rVal = rx_string.toInt();
  Serial.print("Right IR threshold value set to ");
  Serial.println(rVal);
  return;
}

// Start streaming IR values
void start_streaming() {
  streaming = true;
  Serial.println("Streaming enabled");
  return;
}


// Stop streaming IR values
void stop_streaming() {
  streaming = false;
  Serial.println("Streaming disabled");
  return;
}


// Start streaming plotting values
void start_plotting_stream() {
  plotStreaming = true;
  return;
}


// Stop streaming plotting values
void stop_plotting_stream() {
  plotStreaming = false;
  return;
}
