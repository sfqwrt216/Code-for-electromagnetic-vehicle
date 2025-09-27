/**
 * 电磁车控制系统 (Electromagnetic Vehicle Control System)
 * 
 * 功能描述：
 * - 电磁传感器检测与处理
 * - 电机速度控制与保护
 * - PID控制算法优化
 * - 路径跟踪与导航
 * - 系统状态监控
 */

#include "config.h"
#include "motor_control.h"
#include "sensor.h"

// 引脚定义
#define SENSOR_LEFT     A0    // 左侧电磁传感器
#define SENSOR_CENTER   A1    // 中央电磁传感器
#define SENSOR_RIGHT    A2    // 右侧电磁传感器

#define MOTOR_LEFT_PWM  5     // 左电机PWM控制
#define MOTOR_LEFT_DIR  4     // 左电机方向控制
#define MOTOR_RIGHT_PWM 6     // 右电机PWM控制
#define MOTOR_RIGHT_DIR 7     // 右电机方向控制

#define LED_INDICATOR   13    // 状态指示灯
#define BUZZER_PIN      12    // 蜂鸣器
#define BUTTON_PIN      2     // 按钮输入

// 创建对象
MotorControl motors(MOTOR_LEFT_PWM, MOTOR_LEFT_DIR, MOTOR_RIGHT_PWM, MOTOR_RIGHT_DIR);
ElectromagneticSensor sensors(SENSOR_LEFT, SENSOR_CENTER, SENSOR_RIGHT);

// PID控制参数
double Kp = DEFAULT_KP, Ki = DEFAULT_KI, Kd = DEFAULT_KD;
double setpoint = 0.0;
double pidInput = 0.0;
double pidOutput = 0.0;
double lastError = 0.0;

// 系统状态
enum SystemState {
    INIT,
    CALIBRATING,
    READY,
    RUNNING,
    STOPPED,
    ERROR
};

SystemState currentState = INIT;
unsigned long lastControlTime = 0;
unsigned long lastStatusTime = 0;
int errorCount = 0;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("===============================");
  Serial.println("电磁车控制系统启动中...");
  Serial.println("Electromagnetic Vehicle v" FIRMWARE_VERSION);
  Serial.println("===============================");
  
  // 初始化引脚
  pinMode(LED_INDICATOR, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // 系统启动提示
  systemBeep(2, 200);
  blinkLED(3, 200);
  
  // 初始化模块
  motors.begin();
  sensors.begin();
  
  currentState = READY;
  
  Serial.println("系统初始化完成，等待开始信号...");
  Serial.println("按下按钮开始校准，长按2秒开始运行");
  
  waitForStart();
}

void loop() {
  unsigned long currentTime = millis();
  
  // 系统状态机
  switch(currentState) {
    case CALIBRATING:
      handleCalibration();
      break;
      
    case RUNNING:
      if(currentTime - lastControlTime >= (1000 / CONTROL_FREQUENCY)) {
        runMainControl();
        lastControlTime = currentTime;
      }
      break;
      
    case STOPPED:
      motors.stop();
      break;
      
    case ERROR:
      handleError();
      break;
      
    default:
      break;
  }
  
  // 状态显示
  if(currentTime - lastStatusTime >= 500) {
    displaySystemStatus();
    lastStatusTime = currentTime;
  }
  
  // 按钮处理
  handleButtonPress();
  
  // 串口命令处理
  handleSerialCommands();
  
  delay(10);
}

/**
 * 等待开始信号
 */
void waitForStart() {
  while(true) {
    if(digitalRead(BUTTON_PIN) == LOW) {
      delay(50); // 消抖
      if(digitalRead(BUTTON_PIN) == LOW) {
        unsigned long pressTime = millis();
        
        // 等待按钮释放或长按
        while(digitalRead(BUTTON_PIN) == LOW) {
          delay(10);
        }
        
        unsigned long pressDuration = millis() - pressTime;
        
        if(pressDuration > 2000) {
          // 长按：直接开始运行
          currentState = RUNNING;
          systemBeep(1, 500);
          Serial.println("开始运行模式");
          break;
        } else {
          // 短按：开始校准
          currentState = CALIBRATING;
          systemBeep(3, 100);
          Serial.println("开始校准模式");
          break;
        }
      }
    }
    delay(50);
  }
}

/**
 * 校准处理
 */
void handleCalibration() {
  sensors.startCalibration();
  currentState = READY;
  systemBeep(2, 300);
  Serial.println("校准完成，等待运行指令");
}

/**
 * 主控制循环
 */
void runMainControl() {
  // 检查传感器是否检测到导线
  if(!sensors.isLineDetected()) {
    errorCount++;
    if(errorCount > MAX_ERROR_COUNT) {
      currentState = ERROR;
      return;
    }
  } else {
    errorCount = 0;
  }
  
  // 获取位置偏差
  pidInput = sensors.getPosition();
  
  // PID计算
  pidOutput = calculatePID(pidInput);
  
  // 电机控制
  int baseSpeed = DEFAULT_SPEED;
  int leftSpeed = constrain(baseSpeed + pidOutput, MIN_SPEED, MAX_SPEED);
  int rightSpeed = constrain(baseSpeed - pidOutput, MIN_SPEED, MAX_SPEED);
  
  motors.setSpeed(leftSpeed, rightSpeed);
}

/**
 * PID控制计算
 */
double calculatePID(double error) {
  static double integral = 0.0;
  static double lastError = 0.0;
  static unsigned long lastTime = 0;
  
  unsigned long now = millis();
  double deltaTime = (now - lastTime) / 1000.0;
  
  if(deltaTime > 0) {
    // 比例项
    double proportional = Kp * error;
    
    // 积分项
    integral += error * deltaTime;
    integral = constrain(integral, -100, 100); // 积分限幅
    double integralTerm = Ki * integral;
    
    // 微分项
    double derivative = (error - lastError) / deltaTime;
    double derivativeTerm = Kd * derivative;
    
    lastError = error;
    lastTime = now;
    
    return proportional + integralTerm + derivativeTerm;
  }
  
  return 0.0;
}

/**
 * 按钮处理
 */
void handleButtonPress() {
  static unsigned long lastPress = 0;
  static bool buttonPressed = false;
  
  if(digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    if(millis() - lastPress > 200) { // 消抖
      buttonPressed = true;
      lastPress = millis();
      
      // 切换运行状态
      if(currentState == RUNNING) {
        currentState = STOPPED;
        motors.stop();
        systemBeep(1, 200);
        Serial.println("系统暂停");
      } else if(currentState == STOPPED || currentState == READY) {
        currentState = RUNNING;
        systemBeep(2, 100);
        Serial.println("系统运行");
      }
    }
  } else if(digitalRead(BUTTON_PIN) == HIGH && buttonPressed) {
    buttonPressed = false;
  }
}

/**
 * 错误处理
 */
void handleError() {
  motors.emergencyStop();
  digitalWrite(LED_INDICATOR, HIGH);
  
  Serial.println("系统错误：长时间未检测到导线");
  Serial.println("按按钮重启系统");
  
  // 错误提示音
  for(int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 1000, 100);
    delay(200);
  }
  
  // 等待按钮重启
  while(digitalRead(BUTTON_PIN) == HIGH) {
    delay(100);
  }
  
  errorCount = 0;
  currentState = READY;
  digitalWrite(LED_INDICATOR, LOW);
  Serial.println("系统重启");
}

/**
 * 系统状态显示
 */
void displaySystemStatus() {
  if(!DEBUG_ENABLED) return;
  
  static int displayCounter = 0;
  displayCounter++;
  
  // 每隔几次显示详细信息
  if(displayCounter % 4 == 0) {
    Serial.println("======== 系统状态 ========");
    Serial.print("状态: ");
    switch(currentState) {
      case INIT: Serial.println("初始化"); break;
      case CALIBRATING: Serial.println("校准中"); break;
      case READY: Serial.println("就绪"); break;
      case RUNNING: Serial.println("运行中"); break;
      case STOPPED: Serial.println("已停止"); break;
      case ERROR: Serial.println("错误"); break;
    }
    
    if(currentState == RUNNING) {
      sensors.printSensorValues();
      Serial.print("电机速度 - 左: "); Serial.print(motors.getLeftSpeed());
      Serial.print(" 右: "); Serial.println(motors.getRightSpeed());
      Serial.print("PID输出: "); Serial.println(pidOutput, 2);
      Serial.print("错误计数: "); Serial.println(errorCount);
    }
    Serial.println("========================");
  }
  
  // LED状态指示
  updateStatusLED();
}

/**
 * LED状态更新
 */
void updateStatusLED() {
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  switch(currentState) {
    case READY:
      // 慢闪
      if(millis() - lastBlink > 1000) {
        ledState = !ledState;
        digitalWrite(LED_INDICATOR, ledState);
        lastBlink = millis();
      }
      break;
      
    case RUNNING:
      // 快闪
      if(millis() - lastBlink > 200) {
        ledState = !ledState;
        digitalWrite(LED_INDICATOR, ledState);
        lastBlink = millis();
      }
      break;
      
    case STOPPED:
      // 常亮
      digitalWrite(LED_INDICATOR, HIGH);
      break;
      
    case ERROR:
      // 超快闪
      if(millis() - lastBlink > 100) {
        ledState = !ledState;
        digitalWrite(LED_INDICATOR, ledState);
        lastBlink = millis();
      }
      break;
      
    default:
      digitalWrite(LED_INDICATOR, LOW);
      break;
  }
}

/**
 * 系统蜂鸣器
 */
void systemBeep(int count, int duration) {
  for(int i = 0; i < count; i++) {
    tone(BUZZER_PIN, 2000, duration);
    delay(duration + 50);
  }
}

/**
 * LED闪烁
 */
void blinkLED(int count, int duration) {
  for(int i = 0; i < count; i++) {
    digitalWrite(LED_INDICATOR, HIGH);
    delay(duration);
    digitalWrite(LED_INDICATOR, LOW);
    delay(duration);
  }
}

/**
 * 串口命令处理
 */
void handleSerialCommands() {
  if(Serial.available()) {
    String command = Serial.readString();
    command.trim();
    
    if(command == "START") {
      currentState = RUNNING;
      Serial.println("通过串口启动系统");
    } else if(command == "STOP") {
      currentState = STOPPED;
      Serial.println("通过串口停止系统");
    } else if(command == "CALIBRATE") {
      currentState = CALIBRATING;
      Serial.println("通过串口开始校准");
    } else if(command == "STATUS") {
      displaySystemStatus();
    } else if(command.startsWith("KP=")) {
      Kp = command.substring(3).toDouble();
      Serial.print("设置Kp="); Serial.println(Kp);
    } else if(command.startsWith("KI=")) {
      Ki = command.substring(3).toDouble();
      Serial.print("设置Ki="); Serial.println(Ki);
    } else if(command.startsWith("KD=")) {
      Kd = command.substring(3).toDouble();
      Serial.print("设置Kd="); Serial.println(Kd);
    } else {
      Serial.println("未知命令。可用命令: START, STOP, CALIBRATE, STATUS, KP=x, KI=x, KD=x");
    }
  }
}