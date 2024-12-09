//
// Created by Ryan Shores on 12/8/24.
//

#include <Arduino.h>

#include "ServoControl.h"
#include "StringHelper.h"

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

void ServoControl::fire(int darts){ //function for firing a number of darts
    Serial.print(F("Firing " STR(darts) " darts!"));
    for(int i = 0; i < darts; i++){
        rollServo.write(rollStopSpeed + rollMoveSpeed);//start rotating the servo
        delay(rollPrecision);//time for approximately 60 degrees of rotation
        rollServo.write(rollStopSpeed);//stop rotating the servo
        delay(5); //delay for smoothness
        if (dartsFired < 6){ // track how many darts have been fired
            dartsFired++;
        } else {
            dartsFired = 6; //lock the number at 6
            return;
        }
    }
}

void ServoControl::fireAll(){ // this function fires all the darts by spinning the barrel
    rollServo.write(rollStopSpeed + rollMoveSpeed);//start rotating the servo
    delay(rollPrecision * 6); //time for 360 degrees of rotation
    rollServo.write(rollStopSpeed);//stop rotating the servo
    delay(5); // delay for smoothness
    Serial.println("FIRING ALL DARTS");
    dartsFired = 6;
}

/*
** this function acts as a form of Roulette, in which a spinning turret stops periodically
** rolls a (simulated) 6 sided die each time it stops in order to decide whether it should fire or not.
*/
void ServoControl::randomRoulette() {
    Serial.println("ENTERING ROULETTE MODE");
    dartsFired = 0;
    while(dartsFired < 6){ //while we still have darts, stay within this while loop
        if (dartsFired < 6){ // if we still have darts do stuff (this is redundancy to help break out of the while loop in case something weird happens)
            pitchServoVal = 110;
            pitchServo.write(pitchServoVal); // set PITCH servo to flat angle
            yawServo.write(145); // set YAW to rotate slowly
            delay(400); // rotating for a moment
            yawServo.write(90); // stop
            delay(400 * random(1,4)); //wait for a random delay each time

            if(random(3) == 0){ // you have a 1 in six chance of being hit
                delay(700);
                if(random(2) == 0){ // 50% chance to either shake the head yes before firing or just fire
                    shakeHeadYes();
                    delay(150);
                    fire(1); // fire 1 dart
                    delay(100);
                    // } else if(random(6) == 0){
                    //   shakeHeadYes();
                    //   delay(50);
                    //   fireAll(); // fire all the darts
                    //   delay(50);
                } else {
                    fire(1); // fire 1 dart
                    delay(50);
                }
            }else{
                if(random(6) == 5){
                    delay(700);
                    shakeHeadNo();
                    delay(300);
                } else{
                    delay(700);
                }
            }
        } else{
            yawServo.write(90); // redundancy to stop the spinning and break out of the while loop
            return;
        }
    }
    yawServo.write(90); // finally, stop the yaw movement
}

