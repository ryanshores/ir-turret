//
// Created by Ryan Shores on 12/8/24.
//

#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

#include <Servo.h>
#define HEAD_SHAKE_DEFAULT 3

class ServoControl {
private:
    Servo yawServo; //names the servo responsible for YAW rotation, 360 spin around the base
    Servo pitchServo; //names the servo responsible for PITCH rotation, up and down tilt
    Servo rollServo; //names the servo responsible for ROLL rotation, spins the barrel to fire darts

    int pitchPin;
    int rollPin;
    int yawPin;

    int pitchServoVal = 100;

    int pitchMoveSpeed = 8; //this variable is the angle added to the pitch servo to control how quickly the PITCH servo moves - try values between 3 and 10
    int yawMoveSpeed = 90; //this variable is the speed controller for the continuous movement of the YAW servo motor. It is added or subtracted from the yawStopSpeed, so 0 would mean full speed rotation in one direction, and 180 means full rotation in the other. Try values between 10 and 90;
    int yawStopSpeed = 90; //value to stop the yaw motor - keep this at 90
    int rollMoveSpeed = 90; //this variable is the speed controller for the continuous movement of the ROLL servo motor. It is added or subtracted from the rollStopSpeed, so 0 would mean full speed rotation in one direction, and 180 means full rotation in the other. Keep this at 90 for best performance / highest torque from the roll motor when firing.
    int rollStopSpeed = 90; //value to stop the roll motor - keep this at 90

    int yawPrecision = 150; // this variable represents the time in milliseconds that the YAW motor will remain at it's set movement speed. Try values between 50 and 500 to start (500 milliseconds = 1/2 second)
    int rollPrecision = 158; // this variable represents the time in milliseconds that the ROLL motor with remain at it's set movement speed. If this ROLL motor is spinning more or less than 1/6th of a rotation when firing a single dart (one call of the fire(); command) you can try adjusting this value down or up slightly, but it should remain around the stock value (160ish) for best results.

    int pitchMax = 175; // this sets the maximum angle of the pitch servo to prevent it from crashing, it should remain below 180, and be greater than the pitchMin
    int pitchMin = 10; // this sets the minimum angle of the pitch servo to prevent it from crashing, it should remain above 0, and be less than the pitchMax

    void homeServos();

  public:
    ServoControl(int yawPin, int pitchPin, int rollPin);

    void setup();
    void shakeHeadYes(int moves = HEAD_SHAKE_DEFAULT);
    void shakeHeadNo(int moved = HEAD_SHAKE_DEFAULT);
    void leftMove(int moves);
    void rightMove(int moves);
    void upMove(int moves);
    void downMove(int moves);
    void fire();
    void fireAll();

};



#endif //SERVOCONTROL_H
