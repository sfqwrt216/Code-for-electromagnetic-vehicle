/**
 * 电磁传感器库 (Electromagnetic Sensor Library)
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include "config.h"

class ElectromagneticSensor {
private:
    int sensorPins[NUM_SENSORS];
    int sensorValues[NUM_SENSORS];
    int calibratedMin[NUM_SENSORS];
    int calibratedMax[NUM_SENSORS];
    bool calibrated;
    
    // 滤波参数
    static const int FILTER_SIZE = 5;
    int sensorHistory[NUM_SENSORS][FILTER_SIZE];
    int historyIndex;

public:
    // 构造函数
    ElectromagneticSensor(int pin1, int pin2, int pin3);
    
    // 初始化
    void begin();
    
    // 传感器读取
    void readSensors();
    void readFilteredSensors();
    
    // 校准
    void startCalibration();
    void endCalibration();
    bool isCalibrated() { return calibrated; }
    
    // 位置计算
    double getPosition();
    int getLinePosition();
    bool isLineDetected();
    
    // 数据访问
    int getSensorValue(int index);
    int* getAllSensorValues() { return sensorValues; }
    
    // 调试输出
    void printSensorValues();
    void printCalibrationValues();
    
private:
    void updateHistory(int sensorIndex, int value);
    int getFilteredValue(int sensorIndex);
};

#endif // SENSOR_H