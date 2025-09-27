# 电磁车项目编译说明 (Electromagnetic Vehicle Project Build Instructions)

## Arduino IDE 编译方法

### 1. 准备环境
1. 下载并安装 Arduino IDE (1.8.x 或 2.x)
2. 连接 Arduino 开发板到计算机
3. 在 Arduino IDE 中选择正确的开发板类型和端口

### 2. 库依赖
本项目不需要额外的第三方库，所有功能都使用Arduino标准库实现。

### 3. 编译步骤
1. 打开 `electromagnetic_vehicle.ino` 文件
2. 确保以下文件在同一目录下：
   - electromagnetic_vehicle.ino (主程序)
   - config.h (配置文件)
   - motor_control.h (电机控制头文件)
   - motor_control.cpp (电机控制实现)
   - sensor.h (传感器头文件)
   - sensor.cpp (传感器实现)
3. 点击 "验证/编译" 按钮检查代码
4. 点击 "上传" 按钮将程序烧录到Arduino

### 4. 硬件测试
1. 先上传 `hardware_test.ino` 进行硬件测试
2. 打开串口监视器查看测试结果
3. 确认所有硬件连接正常后再上传主程序

## 命令行编译方法 (使用 arduino-cli)

### 1. 安装 arduino-cli
```bash
# Linux/Mac
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Windows (使用 PowerShell)
Invoke-WebRequest -Uri https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh -UseBasicParsing | Invoke-Expression
```

### 2. 初始化和配置
```bash
# 创建配置文件
arduino-cli config init

# 更新核心索引
arduino-cli core update-index

# 安装 Arduino AVR 核心
arduino-cli core install arduino:avr
```

### 3. 编译和上传
```bash
# 编译主程序
arduino-cli compile --fqbn arduino:avr:uno electromagnetic_vehicle.ino

# 上传程序 (替换 /dev/ttyUSB0 为实际端口)
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno electromagnetic_vehicle.ino

# 编译硬件测试程序
arduino-cli compile --fqbn arduino:avr:uno hardware_test.ino

# 上传硬件测试程序
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno hardware_test.ino
```

### 4. 监控串口输出
```bash
# 监控串口输出
arduino-cli monitor -p /dev/ttyUSB0 -b 9600
```

## PlatformIO 编译方法

### 1. 创建 platformio.ini 文件
```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
monitor_speed = 9600
lib_deps = 
build_flags = 
    -D ARDUINO_AVR_UNO
```

### 2. PlatformIO 命令
```bash
# 编译
pio run

# 上传
pio run --target upload

# 监控串口
pio device monitor
```

## 编译选项说明

### 1. 板卡选择
- Arduino Uno: `arduino:avr:uno`
- Arduino Nano: `arduino:avr:nano`
- Arduino Mega: `arduino:avr:mega`

### 2. 编译标志
可以在编译时添加以下标志来控制功能：
```bash
-D DEBUG_ENABLED=1      # 启用调试输出
-D SENSOR_TYPE=1        # 设置传感器类型
-D MOTOR_DRIVER_TYPE=1  # 设置电机驱动类型
```

## 故障排除

### 编译错误
1. **找不到头文件**: 确保所有 .h 和 .cpp 文件在同一目录
2. **内存不足**: 减少调试输出或优化代码
3. **语法错误**: 检查代码语法，特别是中文注释编码

### 上传错误
1. **端口错误**: 检查Arduino连接和端口选择
2. **权限问题**: Linux下可能需要添加用户到 dialout 组
3. **驱动问题**: 确保Arduino驱动正确安装

### 运行时问题
1. **串口无输出**: 检查波特率设置 (9600)
2. **传感器无反应**: 运行硬件测试程序检查连接
3. **电机不转**: 检查电机驱动和电源连接

## 性能优化

### 代码优化
1. 减少Serial.print()调用频率
2. 使用整数运算替代浮点运算
3. 优化循环和条件判断

### 内存优化
1. 使用F()宏存储字符串到程序内存
2. 减少全局变量使用
3. 优化数组大小

## 调试技巧

### 1. 串口调试
```cpp
// 使用条件编译控制调试输出
#if DEBUG_ENABLED
  Serial.println("调试信息");
#endif
```

### 2. LED 指示调试
```cpp
// 使用LED显示状态
digitalWrite(13, digitalRead(2)); // LED跟随按钮状态
```

### 3. 蜂鸣器声音提示
```cpp
// 不同频率表示不同状态
tone(12, 1000, 100); // 正常状态
tone(12, 2000, 100); // 警告状态
```