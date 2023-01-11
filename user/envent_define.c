/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#include "envent_define.h"
#include "key_board.h"
#include "app_global.h"
#include "adc_board.h"

// uint16_t sys_envent = 0;
uint8_t jostick_key[4] = {0};

//-----------------------------------------------------------
//	事件产生
//-----------------------------------------------------------
void event_produce(void)
{
    // uint8_t key = 0;
    // static uint8_t power_key_long_count = 0;
    // static uint8_t key_old = 0;

    jostick_key[0] = key_get_result(1); //key: 1~8

    // key = key_get_result(0);

    // if ((key & bit2)) //插入充电线
    //     sys_envent |= USB_INSERT;

    // if ((key & bit3)) //充满电
    //     sys_envent |= CHANGE_FULL;

    // if (key & bit0)
    // {
    //     wakeup_overtime_cnt = 0;
    //     if (power_key_long_count < 255)
    //     {
    //         power_key_long_count++;
    //     }
    //     if (power_key_long_count == 200) // 2s
    //     {
    //         // power_key_long_count = 0;
    //         sys_envent |= LED_MODE_KEY; //电源键长按
    //     }
    // }
    // else
    // {
    //     if (power_key_long_count >= 5 && power_key_long_count <= 50)
    //     {
    //         sys_envent |= POWER_KEY; //单击电源键
    //     }

    //     power_key_long_count = 0;
    // }

    //PCB生产检测进入
    // if ((key & bit0)) //电源键长按
    // {
    //     sys_envent |= PCB_CHECK;
    // }

    // key_old = key;
}

void work_off(void)
{
    // app_work_mode = MODE_A;
    // app_flag.work = 0;
}

//-----------------------------------------------------------
//	事件处理
//-----------------------------------------------------------
void event_handle(void)
{
    // uint16_t temp = 1;
    
    // if (app_flag.sys_ready==0)
    // {
    //     sys_envent = 0;
    //     return;
    // }

    // while (temp)
    // {   
    //     switch (temp) //注意事件顺序
    //     {
    //     case LED_MODE_KEY:
    //         if (sys_envent & temp)
    //         {
    //             app_flag.sleep = 0;
    //             app_flag.disp_battery_level = 1;
    //             app_flag.light_state = !app_flag.light_state;
    //         }
    //         break;
    //     case POWER_KEY:
    //         if (sys_envent & temp)
    //         {
    //             // if (app_flag.usb_insert == 0)
    //             {
    //                 app_flag.sleep = 0;
    //                 app_flag.disp_battery_level = 1;

    //                 if (app_flag.work)
    //                 {
    //                     if (++app_work_mode > MODE_B)
    //                     {
    //                         work_off();
    //                     }
    //                 }
    //                 else
    //                 {
    //                     if (app_battery_level > BATTERY_LOSE)
    //                     {
    //                         app_flag.work = 1;
    //                     }
    //                 }
    //             }
    //         }
    //         break;
    //     case USB_INSERT:
    //         if (sys_envent & temp)
    //         {
    //             app_flag.sleep = 0;
    //             app_flag.disp_battery_level = 1;
    //             app_flag.usb_insert = 1;
    //             // work_off();
    //         }
    //         else
    //         {
    //             app_flag.usb_insert = 0;
    //             app_flag.charge_full = 0;
    //         }
    //         break;
    //     case DIS_BATTERY:
    //         if (sys_envent & temp)
    //         {
    //             app_flag.sleep = 0;
    //             app_flag.disp_battery_level = 1;
    //         }
    //         break;
    //     case CHANGE_TEMP_UNIT:
    //         if (sys_envent & temp)
    //         {
    //             if (app_flag.temp_unit_C)
    //                 app_flag.temp_unit_C = 0;
    //             else
    //                 app_flag.temp_unit_C = 1;
    //         }
    //         break;
    //     case CHANGE_FULL:
    //         if (sys_envent & temp)
    //             app_flag.charge_full = 1;
    //         break;
    //     default:
    //         break;
    //     }
    //     temp <<= 1;
    // }
    // sys_envent = 0;

    // //error handle
    // if (app_flag.current_error) //电流错误
    //     work_off();
    // if (app_battery_level <= BATTERY_LOSE) //电池耗尽停止工作
    // {
    //     app_flag.light_state = 0;
    //     work_off();
    // }
}
