/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#ifndef _ADC_PORT_H
#define _ADC_PORT_H

#include "types_base.h"
#include "ch32v20x.h"

//ADC vref define
// #define MTF_ADC_VREF_VDD 4  /*Vref = Vdd*/
// #define MTF_ADC_VREF_2V 1   /*Vref =2.0v*/
// #define MTF_ADC_VREF_2P4V 0 /*Vref =2.4v*/

//ADN channel define
#define MTF_ADC_CH6 ADC_Channel_6
#define MTF_ADC_CH7 ADC_Channel_7

uint8_t MM_adc1_init(void);
uint8_t MM_adc1_exit(void);
void MM_adc1_start(void);
void MM_adc1_suspend(void);
void MM_adc1_set_ref(uint8_t ref);
void MM_adc1_set_channel(uint8_t channel);
void MM_adc1_convert_start(void);
uint8_t MM_adc1_get_state(void);
uint16_t MM_adc1_get_result(void);

#endif
