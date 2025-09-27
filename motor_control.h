/**
 * 电机控制库 (Motor Control Library)
 */

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "Arduino.h"
#include "config.h"

class MotorControl {
private:
    int leftPWMPin;
    int leftDirPin;
    int rightPWMPin;
    int rightDirPin;
    
    int currentLeftSpeed;
    int currentRightSpeed;
    bool motorsEnabled;

public:
    // 构造函数
    MotorControl(int leftPWM, int leftDir, int rightPWM, int rightDir);
    
    // 初始化
    void begin();
    
    // 电机控制
    void setSpeed(int leftSpeed, int rightSpeed);
    void forward(int speed);
    void backward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void stop();
    
    // 高级控制
    void smoothSpeedChange(int targetLeftSpeed, int targetRightSpeed, int steps = 10);
    void emergencyStop();
    
    // 状态查询
    int getLeftSpeed() { return currentLeftSpeed; }
    int getRightSpeed() { return currentRightSpeed; }
    bool isEnabled() { return motorsEnabled; }
    
    // 使能控制
    void enable() { motorsEnabled = true; }
    void disable() { stop(); motorsEnabled = false; }
};

#endif // MOTOR_CONTROL_H