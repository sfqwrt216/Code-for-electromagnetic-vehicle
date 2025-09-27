/**
 * 示例配置文件 (Example Configuration File)
 * 
 * 复制这个文件并根据你的硬件设置进行修改
 * Copy this file and modify it according to your hardware setup
 */

// 根据不同的硬件平台选择配置
// Choose configuration based on different hardware platforms

#ifdef ARDUINO_AVR_UNO
  // Arduino Uno 配置
  #define BOARD_TYPE "Arduino Uno R3"
  #define MAX_PWM_VALUE 255
  #define ADC_RESOLUTION 10
#endif

#ifdef ARDUINO_AVR_NANO
  // Arduino Nano 配置  
  #define BOARD_TYPE "Arduino Nano"
  #define MAX_PWM_VALUE 255
  #define ADC_RESOLUTION 10
#endif

#ifdef ARDUINO_AVR_MEGA2560
  // Arduino Mega 配置
  #define BOARD_TYPE "Arduino Mega 2560"
  #define MAX_PWM_VALUE 255
  #define ADC_RESOLUTION 10
#endif

// 传感器类型配置
// Sensor type configuration
#define SENSOR_TYPE_HALL        1  // 霍尔传感器
#define SENSOR_TYPE_INDUCTIVE   2  // 电感式传感器
#define SENSOR_TYPE_OPTICAL     3  // 光学传感器

// 选择传感器类型 (Choose sensor type)
#define SENSOR_TYPE SENSOR_TYPE_HALL

#if SENSOR_TYPE == SENSOR_TYPE_HALL
  #define SENSOR_THRESHOLD 300
  #define SENSOR_ACTIVE_HIGH true
#elif SENSOR_TYPE == SENSOR_TYPE_INDUCTIVE
  #define SENSOR_THRESHOLD 200
  #define SENSOR_ACTIVE_HIGH false
#elif SENSOR_TYPE == SENSOR_TYPE_OPTICAL
  #define SENSOR_THRESHOLD 400
  #define SENSOR_ACTIVE_HIGH false
#endif

// 电机类型配置
// Motor type configuration
#define MOTOR_DRIVER_L298N      1  // L298N驱动板
#define MOTOR_DRIVER_TB6612     2  // TB6612驱动板
#define MOTOR_DRIVER_DRV8833    3  // DRV8833驱动板

// 选择电机驱动类型 (Choose motor driver type)
#define MOTOR_DRIVER_TYPE MOTOR_DRIVER_L298N

// 车辆物理参数 (Vehicle physical parameters)
#define VEHICLE_LENGTH 200      // 车身长度 (mm)
#define VEHICLE_WIDTH 150       // 车身宽度 (mm)
#define WHEEL_DIAMETER 65       // 轮子直径 (mm)
#define WHEELBASE 120           // 前后轮距 (mm)
#define TRACK_WIDTH 100         // 左右轮距 (mm)

// 赛道参数 (Track parameters)
#define TRACK_WIDTH_MM 20       // 导线宽度 (mm)
#define CURVE_RADIUS_MIN 300    // 最小弯道半径 (mm)
#define STRAIGHT_SPEED_MAX 200  // 直道最大速度
#define CURVE_SPEED_MAX 150     // 弯道最大速度

// PID参数预设 (PID parameter presets)
// 可以根据不同情况选择不同的PID参数
// Different PID parameters for different situations

// 保守型 - 稳定但较慢 (Conservative - stable but slow)
#define PID_CONSERVATIVE_KP 1.5
#define PID_CONSERVATIVE_KI 0.0
#define PID_CONSERVATIVE_KD 0.8

// 平衡型 - 平衡速度和稳定性 (Balanced - balance speed and stability)
#define PID_BALANCED_KP 2.0
#define PID_BALANCED_KI 0.1
#define PID_BALANCED_KD 1.0

// 激进型 - 快速但可能不稳定 (Aggressive - fast but potentially unstable)
#define PID_AGGRESSIVE_KP 3.0
#define PID_AGGRESSIVE_KI 0.2
#define PID_AGGRESSIVE_KD 1.5

// 选择PID预设 (Choose PID preset)
#define PID_PRESET_BALANCED

#ifdef PID_PRESET_CONSERVATIVE
  #define DEFAULT_KP PID_CONSERVATIVE_KP
  #define DEFAULT_KI PID_CONSERVATIVE_KI
  #define DEFAULT_KD PID_CONSERVATIVE_KD
#elif defined(PID_PRESET_BALANCED)
  #define DEFAULT_KP PID_BALANCED_KP
  #define DEFAULT_KI PID_BALANCED_KI
  #define DEFAULT_KD PID_BALANCED_KD
#elif defined(PID_PRESET_AGGRESSIVE)
  #define DEFAULT_KP PID_AGGRESSIVE_KP
  #define DEFAULT_KI PID_AGGRESSIVE_KI
  #define DEFAULT_KD PID_AGGRESSIVE_KD
#endif

// 高级功能开关 (Advanced feature switches)
#define ENABLE_SPEED_ADAPTATION true    // 自适应速度控制
#define ENABLE_PREDICTIVE_CONTROL false // 预测控制
#define ENABLE_SENSOR_FUSION true       // 传感器融合
#define ENABLE_DATA_LOGGING false       // 数据记录
#define ENABLE_WIRELESS_DEBUG false     // 无线调试

// 安全功能 (Safety features)
#define ENABLE_EMERGENCY_STOP true      // 紧急停止
#define ENABLE_COLLISION_DETECTION false// 碰撞检测
#define ENABLE_BATTERY_MONITOR true     // 电池监控
#define ENABLE_THERMAL_PROTECTION false // 过热保护

// 调试级别 (Debug levels)
#define DEBUG_LEVEL_NONE    0
#define DEBUG_LEVEL_ERROR   1
#define DEBUG_LEVEL_WARNING 2
#define DEBUG_LEVEL_INFO    3
#define DEBUG_LEVEL_VERBOSE 4

#define DEBUG_LEVEL DEBUG_LEVEL_INFO

// 性能监控 (Performance monitoring)
#define ENABLE_PERFORMANCE_MONITOR true
#define PERFORMANCE_SAMPLE_RATE 10      // 每秒采样次数

// 通信配置 (Communication configuration)
#define SERIAL_BAUD_RATE 9600           // 串口波特率
#define ENABLE_BLUETOOTH false          // 蓝牙通信
#define ENABLE_WIFI false               // WiFi通信

/*
使用说明 (Usage Instructions):

1. 根据你的硬件选择合适的传感器和电机驱动类型
2. 测量你的车辆尺寸并更新物理参数
3. 根据你的赛道特点调整赛道参数
4. 选择合适的PID预设或自定义PID参数
5. 根据需要启用或禁用高级功能

注意事项 (Notes):
- 修改配置后需要重新编译和上传代码
- 某些功能组合可能会增加内存使用
- 在实际使用前请进行充分测试
*/