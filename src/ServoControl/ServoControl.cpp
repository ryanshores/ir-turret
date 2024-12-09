//
// Created by Ryan Shores on 12/8/24.
//

#include <Arduino.h>
#include "ServoControl.h"

ServoControl::ServoControl(const int yawPin, const int pitchPin, const int rollPin) {
    this-> yawPin = yawPin;
    this-> pitchPin = pitchPin;
    this-> rollPin = rollPin;
}


void ServoControl::setup() {
    yawServo.attach(yawPin); //attach YAW servo to pin 10
    pitchServo.attach(pitchPin); //attach PITCH servo to pin 11
    rollServo.attach(rollPin); //attach ROLL servo to pin 12

    homeServos();
}

void ServoControl::homeServos() {
  yawServo.write(yawStopSpeed); //setup YAW servo to be STOPPED (90)
  delay(20);
  rollServo.write(rollStopSpeed); //setup ROLL servo to be STOPPED (90)
  delay(100);
  pitchServo.write(100); //set PITCH servo to 100 degree position
  delay(100);
  pitchServoVal = 100; // store the pitch servo value
  Serial.println("HOMING");
}

void ServoControl::shakeHeadYes(const int moves) {
  Serial.println("YES");
  const int startAngle = pitchServoVal; // Current position of the pitch servo
  const int nodAngle = startAngle + 20; // Angle for nodding motion

  for (int i = 0; i < moves; i++) { // Repeat nodding motion three times
    // Nod up
    for (int angle = startAngle; angle <= nodAngle; angle++) {
      pitchServo.write(angle);
      delay(7); // Adjust delay for smoother motion
    }
    delay(50); // Pause at nodding position
    // Nod down
    for (int angle = nodAngle; angle >= startAngle; angle--) {
      pitchServo.write(angle);
      delay(7); // Adjust delay for smoother motion
    }
    delay(50); // Pause at starting position
  }
}

void ServoControl::shakeHeadNo(const int moves) {
  Serial.println("NO");

  for (int i = 0; i < moves; i++) {
    // Repeat nodding motion three times
    // rotate right, stop, then rotate left, stop
    yawServo.write(140);
    delay(190); // Adjust delay for smoother motion
    yawServo.write(yawStopSpeed);
    delay(50);
    yawServo.write(40);
    delay(190); // Adjust delay for smoother motion
    yawServo.write(yawStopSpeed);
    delay(50); // Pause at starting position
  }
}

void ServoControl::leftMove(const int moves) {
    for (int i = 0; i < moves; i++) {
        yawServo.write(yawStopSpeed + yawMoveSpeed);
        // adding the servo speed = 180 (full counterclockwise rotation speed)
        delay(yawPrecision); // stay rotating for a certain number of milliseconds
        yawServo.write(yawStopSpeed); // stop rotating
        delay(5); //delay for smoothness
        Serial.println("LEFT");
    }
}

void ServoControl::rightMove(const int moves) {
    // function to move right
    for (int i = 0; i < moves; i++) {
        yawServo.write(yawStopSpeed - yawMoveSpeed); //subtracting the servo speed = 0 (full clockwise rotation speed)
        delay(yawPrecision);
        yawServo.write(yawStopSpeed);
        delay(5);
        Serial.println("RIGHT");
    }
}

void ServoControl::upMove(const int moves) {
    for (int i = 0; i < moves; i++) {
        if (pitchServoVal > pitchMin) {
            //make sure the servo is within rotation limits (greater than 10 degrees by default)
            pitchServoVal = pitchServoVal - pitchMoveSpeed; //decrement the current angle and update
            pitchServo.write(pitchServoVal);
            delay(50);
            Serial.println("UP");
        }
    }
}

void ServoControl::downMove(const int moves) {
    for (int i = 0; i < moves; i++) {
        if (pitchServoVal < pitchMax) {
            //make sure the servo is within rotation limits (less than 175 degrees by default)
            pitchServoVal = pitchServoVal + pitchMoveSpeed; //increment the current angle and update
            pitchServo.write(pitchServoVal);
            delay(50);
            Serial.println("DOWN");
        }
    }
}

/**
 * fire does xyz
 */
void ServoControl::fire() {
    //function for firing a single dart
    rollServo.write(rollStopSpeed + rollMoveSpeed); //start rotating the servo
    delay(rollPrecision); //time for approximately 60 degrees of rotation
    rollServo.write(rollStopSpeed); //stop rotating the servo
    delay(5); //delay for smoothness
    Serial.println("FIRING");
}

void ServoControl::fireAll() {
    //function to fire all 6 darts at once
    rollServo.write(rollStopSpeed + rollMoveSpeed); //start rotating the servo
    delay(rollPrecision * 6); //time for 360 degrees of rotation
    rollServo.write(rollStopSpeed); //stop rotating the servo
    delay(5); // delay for smoothness
    Serial.println("FIRING ALL");
}

