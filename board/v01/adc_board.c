/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#include "adc_board.h"
#include "stdio.h"

#define MTF_ADC_debug(...) //printf(__VA_ARGS__)

uint16_t _adc_value[ADC_CHANNEL_TOTAL] = {2047};
void adc_run(void)
{
    static uint16_t adc_result = 20000;
    static uint8_t sample_count = 255, channel_count = 0, _reject_count = 0;

    if (sample_count >= 8)
    {
        sample_count = 0;                            //单通道采样次数
        _reject_count = 0;

        _adc_value[channel_count] = adc_result >> 3; // adc_result/8, 注意复位的第一次值
        adc_result = 0;

        MTF_ADC_debug("ad %d: %d\r\n", channel_count, _adc_value[channel_count]);
        if (++channel_count >= ADC_CHANNEL_TOTAL)
        {
            channel_count = 0;
        }

        //为保证结果准确度, 切换通道后建议500ns后开采样
        if (channel_count == 0) 
        {
            // MM_adc1_set_ref(MTF_ADC_VREF_2P4V);
            MM_adc1_set_channel(MTF_ADC_CH6);
        }
        else 
        {
            // MM_adc1_set_ref(MTF_ADC_VREF_2P4V);
            MM_adc1_set_channel(MTF_ADC_CH7);
        }
        MM_adc1_convert_start(); //开启转换
    }
    else
    {
        if (MM_adc1_get_state() == 0) //是否转换完成
        {
            if (++_reject_count >= 2)
            {
                adc_result += MM_adc1_get_result(); //获取AD值
                sample_count++;
            }
            MM_adc1_convert_start(); //开启转换
        }
    }
}
