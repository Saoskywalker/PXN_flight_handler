/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#ifndef _ENVENT_DEFINE_H
#define _ENVENT_DEFINE_H

#include "types_base.h"

#define LED_MODE_KEY 0X02
#define POWER_KEY 0X01
#define USB_INSERT 0X04
#define DIS_BATTERY 0X10
#define CHANGE_TEMP_UNIT 0X20
#define CHANGE_FULL 0X40
#define PCB_CHECK 0X80

void event_produce(void);
void event_handle(void);

void work_off(void);

extern uint8_t jostick_key[];

#endif
