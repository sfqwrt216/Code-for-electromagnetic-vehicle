/**
 * 电磁车系统测试程序 (Electromagnetic Vehicle System Test)
 * 
 * 这个程序用于测试各个硬件组件是否正常工作
 * This program tests if all hardware components are working correctly
 */

#include "config.h"

// 测试模式选择
#define TEST_ALL_COMPONENTS     0
#define TEST_SENSORS_ONLY       1  
#define TEST_MOTORS_ONLY        2
#define TEST_LED_BUZZER_ONLY    3
#define TEST_BUTTON_ONLY        4

// 选择要运行的测试
#define TEST_MODE TEST_ALL_COMPONENTS

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("=====================================");
  Serial.println("电磁车系统硬件测试程序");
  Serial.println("Electromagnetic Vehicle Hardware Test");
  Serial.println("=====================================");
  
  // 初始化所有引脚
  initializePins();
  
  // 根据测试模式运行相应测试
  switch(TEST_MODE) {
    case TEST_ALL_COMPONENTS:
      Serial.println("运行全部组件测试...");
      runAllTests();
      break;
      
    case TEST_SENSORS_ONLY:
      Serial.println("运行传感器测试...");
      runSensorTest();
      break;
      
    case TEST_MOTORS_ONLY:
      Serial.println("运行电机测试...");
      runMotorTest();
      break;
      
    case TEST_LED_BUZZER_ONLY:
      Serial.println("运行LED和蜂鸣器测试...");
      runLEDBuzzerTest();
      break;
      
    case TEST_BUTTON_ONLY:
      Serial.println("运行按钮测试...");
      runButtonTest();
      break;
      
    default:
      Serial.println("未知测试模式");
      break;
  }
}

void loop() {
  // 根据测试模式持续运行测试
  switch(TEST_MODE) {
    case TEST_SENSORS_ONLY:
      continuousSensorTest();
      break;
      
    case TEST_BUTTON_ONLY:
      continuousButtonTest();
      break;
      
    default:
      // 其他测试在setup中完成
      delay(1000);
      break;
  }
}

/**
 * 初始化所有引脚
 */
void initializePins() {
  // 传感器引脚（模拟输入，不需要设置）
  
  // 电机引脚
  pinMode(5, OUTPUT);  // 左电机PWM
  pinMode(4, OUTPUT);  // 左电机方向
  pinMode(6, OUTPUT);  // 右电机PWM
  pinMode(7, OUTPUT);  // 右电机方向
  
  // 指示器引脚
  pinMode(13, OUTPUT); // LED
  pinMode(12, OUTPUT); // 蜂鸣器
  
  // 输入引脚
  pinMode(2, INPUT_PULLUP); // 按钮
  
  Serial.println("引脚初始化完成");
}

/**
 * 运行所有测试
 */
void runAllTests() {
  Serial.println("\n=== 开始全面硬件测试 ===");
  
  // 1. LED和蜂鸣器测试
  Serial.println("1. 测试LED和蜂鸣器...");
  testLEDAndBuzzer();
  delay(2000);
  
  // 2. 传感器测试
  Serial.println("2. 测试传感器...");
  testSensors();
  delay(2000);
  
  // 3. 按钮测试
  Serial.println("3. 测试按钮（5秒内按下按钮）...");
  testButton();
  delay(1000);
  
  // 4. 电机测试
  Serial.println("4. 测试电机...");
  testMotors();
  
  Serial.println("\n=== 硬件测试完成 ===");
  Serial.println("如果所有测试都通过，硬件连接正常");
}

/**
 * LED和蜂鸣器测试
 */
void testLEDAndBuzzer() {
  Serial.println("  LED闪烁测试...");
  for(int i = 0; i < 5; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  
  Serial.println("  蜂鸣器测试...");
  for(int i = 0; i < 3; i++) {
    tone(12, 1000, 300);
    delay(500);
  }
  
  Serial.println("  LED和蜂鸣器测试完成");
}

/**
 * 传感器测试
 */
void testSensors() {
  Serial.println("  读取传感器数值（10次采样）:");
  Serial.println("  格式：左传感器 | 中传感器 | 右传感器");
  
  for(int i = 0; i < 10; i++) {
    int leftSensor = analogRead(A0);
    int centerSensor = analogRead(A1);
    int rightSensor = analogRead(A2);
    
    Serial.print("  ");
    Serial.print(leftSensor);
    Serial.print(" | ");
    Serial.print(centerSensor);
    Serial.print(" | ");
    Serial.println(rightSensor);
    
    delay(500);
  }
  
  Serial.println("  传感器测试完成");
  Serial.println("  正常值应在0-1023范围内");
}

/**
 * 按钮测试
 */
void testButton() {
  Serial.println("  等待按钮按下...");
  unsigned long startTime = millis();
  bool buttonPressed = false;
  
  while(millis() - startTime < 5000) {  // 等待5秒
    if(digitalRead(2) == LOW) {
      Serial.println("  按钮按下检测到！");
      buttonPressed = true;
      
      // 按下时LED亮，蜂鸣器响
      digitalWrite(13, HIGH);
      tone(12, 2000, 100);
      
      delay(100);
      digitalWrite(13, LOW);
      break;
    }
    delay(50);
  }
  
  if(!buttonPressed) {
    Serial.println("  警告：未检测到按钮按下");
  } else {
    Serial.println("  按钮测试完成");
  }
}

/**
 * 电机测试
 */
void testMotors() {
  Serial.println("  电机功能测试（请观察电机转动）：");
  
  // 左电机测试
  Serial.println("  测试左电机前转...");
  digitalWrite(4, HIGH);  // 方向
  analogWrite(5, 150);    // PWM速度
  delay(2000);
  analogWrite(5, 0);      // 停止
  
  delay(1000);
  
  // 右电机测试
  Serial.println("  测试右电机前转...");
  digitalWrite(7, HIGH);  // 方向
  analogWrite(6, 150);    // PWM速度
  delay(2000);
  analogWrite(6, 0);      // 停止
  
  delay(1000);
  
  // 双电机同时测试
  Serial.println("  测试双电机同时前转...");
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  analogWrite(5, 150);
  analogWrite(6, 150);
  delay(2000);
  analogWrite(5, 0);
  analogWrite(6, 0);
  
  Serial.println("  电机测试完成");
}

/**
 * 运行单独的传感器测试
 */
void runSensorTest() {
  Serial.println("连续传感器监控模式");
  Serial.println("格式：左 | 中 | 右");
}

void continuousSensorTest() {
  int leftSensor = analogRead(A0);
  int centerSensor = analogRead(A1);
  int rightSensor = analogRead(A2);
  
  Serial.print(leftSensor);
  Serial.print(" | ");
  Serial.print(centerSensor);
  Serial.print(" | ");
  Serial.println(rightSensor);
  
  delay(200);
}

/**
 * 运行单独的电机测试
 */
void runMotorTest() {
  Serial.println("电机测试序列：");
  Serial.println("1. 左电机正转");
  motorTest(true, false, 150);
  
  Serial.println("2. 右电机正转");
  motorTest(false, true, 150);
  
  Serial.println("3. 双电机正转");
  motorTest(true, true, 150);
  
  Serial.println("4. 左电机反转");
  motorTestReverse(true, false, 150);
  
  Serial.println("5. 右电机反转");
  motorTestReverse(false, true, 150);
  
  Serial.println("电机测试完成");
}

void motorTest(bool leftOn, bool rightOn, int speed) {
  if(leftOn) {
    digitalWrite(4, HIGH);
    analogWrite(5, speed);
  }
  if(rightOn) {
    digitalWrite(7, HIGH);
    analogWrite(6, speed);
  }
  
  delay(2000);
  
  analogWrite(5, 0);
  analogWrite(6, 0);
  delay(1000);
}

void motorTestReverse(bool leftOn, bool rightOn, int speed) {
  if(leftOn) {
    digitalWrite(4, LOW);  // 反向
    analogWrite(5, speed);
  }
  if(rightOn) {
    digitalWrite(7, LOW);  // 反向
    analogWrite(6, speed);
  }
  
  delay(2000);
  
  analogWrite(5, 0);
  analogWrite(6, 0);
  delay(1000);
}

/**
 * 运行LED和蜂鸣器测试
 */
void runLEDBuzzerTest() {
  testLEDAndBuzzer();
}

/**
 * 运行按钮测试
 */
void runButtonTest() {
  Serial.println("按钮监控模式 - 按下按钮查看响应");
}

void continuousButtonTest() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(2);
  
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("按钮按下");
    digitalWrite(13, HIGH);
    tone(12, 1500, 200);
  } else if(lastButtonState == LOW && currentButtonState == HIGH) {
    Serial.println("按钮释放");
    digitalWrite(13, LOW);
  }
  
  lastButtonState = currentButtonState;
  delay(50);
}