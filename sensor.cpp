/**
 * 电磁传感器库实现 (Electromagnetic Sensor Library Implementation)
 */

#include "sensor.h"

ElectromagneticSensor::ElectromagneticSensor(int pin1, int pin2, int pin3) {
    sensorPins[0] = pin1;
    sensorPins[1] = pin2;
    sensorPins[2] = pin3;
    
    calibrated = false;
    historyIndex = 0;
    
    // 初始化校准值
    for(int i = 0; i < NUM_SENSORS; i++) {
        calibratedMin[i] = 1023;
        calibratedMax[i] = 0;
        sensorValues[i] = 0;
        
        // 初始化历史记录
        for(int j = 0; j < FILTER_SIZE; j++) {
            sensorHistory[i][j] = 0;
        }
    }
}

void ElectromagneticSensor::begin() {
    for(int i = 0; i < NUM_SENSORS; i++) {
        pinMode(sensorPins[i], INPUT);
    }
}

void ElectromagneticSensor::readSensors() {
    for(int i = 0; i < NUM_SENSORS; i++) {
        sensorValues[i] = analogRead(sensorPins[i]);
    }
}

void ElectromagneticSensor::readFilteredSensors() {
    for(int i = 0; i < NUM_SENSORS; i++) {
        int rawValue = analogRead(sensorPins[i]);
        updateHistory(i, rawValue);
        sensorValues[i] = getFilteredValue(i);
    }
}

void ElectromagneticSensor::updateHistory(int sensorIndex, int value) {
    sensorHistory[sensorIndex][historyIndex] = value;
}

int ElectromagneticSensor::getFilteredValue(int sensorIndex) {
    long sum = 0;
    for(int i = 0; i < FILTER_SIZE; i++) {
        sum += sensorHistory[sensorIndex][i];
    }
    return sum / FILTER_SIZE;
}

void ElectromagneticSensor::startCalibration() {
    Serial.println("开始传感器校准，请移动小车经过所有可能路径...");
    
    // 重置校准值
    for(int i = 0; i < NUM_SENSORS; i++) {
        calibratedMin[i] = 1023;
        calibratedMax[i] = 0;
    }
    
    // 校准循环
    for(int j = 0; j < 400; j++) {
        readSensors();
        
        for(int i = 0; i < NUM_SENSORS; i++) {
            if(sensorValues[i] < calibratedMin[i]) {
                calibratedMin[i] = sensorValues[i];
            }
            if(sensorValues[i] > calibratedMax[i]) {
                calibratedMax[i] = sensorValues[i];
            }
        }
        
        delay(20);
    }
    
    calibrated = true;
    Serial.println("校准完成!");
}

void ElectromagneticSensor::endCalibration() {
    calibrated = true;
}

double ElectromagneticSensor::getPosition() {
    readFilteredSensors();
    
    long numerator = 0;
    long denominator = 0;
    
    for(int i = 0; i < NUM_SENSORS; i++) {
        int value = sensorValues[i];
        
        // 如果已校准，使用校准值
        if(calibrated) {
            value = map(value, calibratedMin[i], calibratedMax[i], 0, 1000);
            value = constrain(value, 0, 1000);
        }
        
        // 加权计算位置
        numerator += (long)(value * (i * 1000));
        denominator += value;
    }
    
    if(denominator != 0) {
        return ((double)numerator / denominator) / 1000.0 - 1.0;  // 返回-1到1的范围
    }
    
    return 0.0;  // 无法确定位置时返回中心位置
}

int ElectromagneticSensor::getLinePosition() {
    return (int)(getPosition() * 1000);  // 返回-1000到1000的整数值
}

bool ElectromagneticSensor::isLineDetected() {
    readSensors();
    
    for(int i = 0; i < NUM_SENSORS; i++) {
        if(sensorValues[i] > SENSOR_THRESHOLD) {
            return true;
        }
    }
    
    return false;
}

int ElectromagneticSensor::getSensorValue(int index) {
    if(index >= 0 && index < NUM_SENSORS) {
        return sensorValues[index];
    }
    return 0;
}

void ElectromagneticSensor::printSensorValues() {
    Serial.print("传感器值: ");
    for(int i = 0; i < NUM_SENSORS; i++) {
        Serial.print(sensorValues[i]);
        if(i < NUM_SENSORS - 1) Serial.print(" | ");
    }
    Serial.print(" 位置: ");
    Serial.println(getPosition(), 3);
}

void ElectromagneticSensor::printCalibrationValues() {
    if(!calibrated) {
        Serial.println("传感器未校准");
        return;
    }
    
    Serial.println("校准值:");
    for(int i = 0; i < NUM_SENSORS; i++) {
        Serial.print("传感器 "); Serial.print(i);
        Serial.print(": 最小="); Serial.print(calibratedMin[i]);
        Serial.print(" 最大="); Serial.println(calibratedMax[i]);
    }
}