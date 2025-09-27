/**
 * 电机控制库实现 (Motor Control Library Implementation)
 */

#include "motor_control.h"

MotorControl::MotorControl(int leftPWM, int leftDir, int rightPWM, int rightDir) {
    leftPWMPin = leftPWM;
    leftDirPin = leftDir;
    rightPWMPin = rightPWM;
    rightDirPin = rightDir;
    
    currentLeftSpeed = 0;
    currentRightSpeed = 0;
    motorsEnabled = false;
}

void MotorControl::begin() {
    pinMode(leftPWMPin, OUTPUT);
    pinMode(leftDirPin, OUTPUT);
    pinMode(rightPWMPin, OUTPUT);
    pinMode(rightDirPin, OUTPUT);
    
    stop();
    motorsEnabled = true;
}

void MotorControl::setSpeed(int leftSpeed, int rightSpeed) {
    if (!motorsEnabled) return;
    
    // 限制速度范围
    leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);
    
    // 左电机控制
    if (leftSpeed >= 0) {
        digitalWrite(leftDirPin, HIGH);
        analogWrite(leftPWMPin, leftSpeed);
    } else {
        digitalWrite(leftDirPin, LOW);
        analogWrite(leftPWMPin, abs(leftSpeed));
    }
    
    // 右电机控制
    if (rightSpeed >= 0) {
        digitalWrite(rightDirPin, HIGH);
        analogWrite(rightPWMPin, rightSpeed);
    } else {
        digitalWrite(rightDirPin, LOW);
        analogWrite(rightPWMPin, abs(rightSpeed));
    }
    
    currentLeftSpeed = leftSpeed;
    currentRightSpeed = rightSpeed;
}

void MotorControl::forward(int speed) {
    setSpeed(speed, speed);
}

void MotorControl::backward(int speed) {
    setSpeed(-speed, -speed);
}

void MotorControl::turnLeft(int speed) {
    setSpeed(speed * TURN_SPEED_RATIO, speed);
}

void MotorControl::turnRight(int speed) {
    setSpeed(speed, speed * TURN_SPEED_RATIO);
}

void MotorControl::stop() {
    analogWrite(leftPWMPin, 0);
    analogWrite(rightPWMPin, 0);
    currentLeftSpeed = 0;
    currentRightSpeed = 0;
}

void MotorControl::smoothSpeedChange(int targetLeftSpeed, int targetRightSpeed, int steps) {
    int leftSpeedStep = (targetLeftSpeed - currentLeftSpeed) / steps;
    int rightSpeedStep = (targetRightSpeed - currentRightSpeed) / steps;
    
    for (int i = 0; i < steps; i++) {
        setSpeed(currentLeftSpeed + leftSpeedStep * i, 
                currentRightSpeed + rightSpeedStep * i);
        delay(50);
    }
    
    setSpeed(targetLeftSpeed, targetRightSpeed);
}

void MotorControl::emergencyStop() {
    motorsEnabled = false;
    stop();
}