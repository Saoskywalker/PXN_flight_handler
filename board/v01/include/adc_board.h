/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#ifndef _ADC_BOARD_H
#define _ADC_BOARD_H

#include "types_base.h"
#include "adc_port.h"

#define ADC_CHANNEL_TOTAL 2 //ADC需扫描通道数
extern uint16_t _adc_value[ADC_CHANNEL_TOTAL];
#define ADC_VR2_VALUE() _adc_value[0]
#define ADC_VR1_VALUE() _adc_value[1]
// #define ADC_MOTOR_CURRENT_VALUE() _adc_value[1]

void adc_run(void);

#endif
