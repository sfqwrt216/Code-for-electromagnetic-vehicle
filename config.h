/**
 * 电磁车配置文件 (Electromagnetic Vehicle Configuration)
 */

#ifndef CONFIG_H
#define CONFIG_H

// 硬件配置
#define BOARD_TYPE "Arduino Uno"
#define FIRMWARE_VERSION "1.0.0"

// 传感器配置
#define NUM_SENSORS 3
#define SENSOR_THRESHOLD 200        // 传感器检测阈值
#define SENSOR_READ_DELAY 10        // 传感器读取延迟(ms)

// 电机配置
#define MOTOR_VOLTAGE 12            // 电机电压(V)
#define MOTOR_MAX_RPM 200           // 电机最大转速(RPM)
#define WHEEL_DIAMETER 65           // 车轮直径(mm)
#define WHEELBASE 150               // 轴距(mm)

// 控制参数
#define DEFAULT_SPEED 150           // 默认速度(0-255)
#define MAX_SPEED 255               // 最大速度
#define MIN_SPEED 80                // 最小速度
#define TURN_SPEED_RATIO 0.7        // 转弯时内侧车轮速度比例

// PID参数
#define DEFAULT_KP 2.0              // 比例系数
#define DEFAULT_KI 0.0              // 积分系数
#define DEFAULT_KD 1.0              // 微分系数

// 系统参数
#define CONTROL_FREQUENCY 20        // 控制频率(Hz)
#define SERIAL_BAUD_RATE 9600       // 串口波特率
#define DEBUG_ENABLED true          // 调试输出使能

// 安全参数
#define EMERGENCY_STOP_ENABLED true // 急停功能使能
#define MAX_ERROR_COUNT 10          // 最大错误计数
#define WATCHDOG_TIMEOUT 5000       // 看门狗超时(ms)

#endif // CONFIG_H