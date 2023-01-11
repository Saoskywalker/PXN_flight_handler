#ifndef _APP_GLOBAL_H
#define _APP_GLOBAL_H

#include "types_base.h"

typedef struct
{
    uint8_t sleep;              // 睡眠
    uint8_t sys_ready;          // 系统准备完毕
    uint8_t work;               // 工作
    uint8_t error;              // 系统错误
    uint8_t sleep_updata;       // 用于标记在唤醒后一段时间内, 进行一定的唤醒处理
    uint8_t light_state;        // 灯开关状态
    uint8_t usb_insert;         // 充电线插入
    uint8_t charge_full;        // 充满电
    uint8_t ntc_error;          // ntc AD错误
    uint8_t current_error;      // 电流AD错误
    uint8_t battery_error;      // 电池AD错误
    uint8_t temp_unit_C;        // 系统温度单位
    uint8_t disp_battery_level; // 显示电池电量
    uint8_t nc2;
    uint8_t nc3;
    uint8_t nc4;
} app_flag_type;

extern app_flag_type app_flag;

// typedef enum
// {
//   AD_NORMAL = 0,
//   AD_SHORT = 1,
//   AD_CUT = 2
// } AISTATUS;

#define AD_NORMAL 0
#define AD_SHORT 1
#define AD_CUT 2

// typedef struct
// {
//   int8_t C_value;
//   int8_t value;
//   uint16_t AD_value;
//   uint8_t status; // AISTATUS status;
// } ADC_PARA;

// extern ADC_PARA water_temp;

// typedef enum
// {
//   MODE_A = 0,
//   MODE_B,
//   MODE_C,
//   MODE_D
// };

// typedef enum
// {
//   BATTERY_LOSE = 0,
//   BATTERY_LV0,
//   BATTERY_LV1,
//   BATTERY_LV2,
//   BATTERY_FULL,
//   BATTERY_HIGH
// };

#define MODE_A 0
#define MODE_B 1
#define MODE_C 2
#define MODE_D 3

#define BATTERY_LOSE 0
#define BATTERY_LV0 1
#define BATTERY_LV1 2
#define BATTERY_LV2 3
#define BATTERY_FULL 4
#define BATTERY_HIGH 5

extern uint8_t app_work_mode;
extern uint8_t app_battery_level;
extern uint16_t wakeup_overtime_cnt; //睡眠唤醒超时

#endif
