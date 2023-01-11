/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#ifndef _GPIO_BOARD_H
#define _GPIO_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v20x.h"

/****************************
应用定义接口
*****************************/
//io定义
//key
#define KEY_6_PIN() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)
#define KEY_7_PIN() GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)
#define KEY_8_PIN() GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)
#define KEY_9_PIN() GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)

#define MODE1_LED_PIN(x) GPIO_WriteBit(GPIOC, GPIO_Pin_1, x)
#define MODE2_LED_PIN(x) GPIO_WriteBit(GPIOC, GPIO_Pin_2, x)
#define MODE3_LED_PIN(x) GPIO_WriteBit(GPIOC, GPIO_Pin_3, x)

void main_IO_init(void);
void main_IO_exit(void);
///////////////////////////////

#define TEST_TOGGLE_PIN() P23 = ~P23 //测试引脚, 翻转
#define TEST_PIN(x) P23 = x //测试引脚

void gpio_test(void);

#ifdef __cplusplus
}
#endif

#endif
