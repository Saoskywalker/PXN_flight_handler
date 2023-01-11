/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#include "app_timer.h"
#include "gpio_board.h"
// #include "display_define.h"
#include "key_board.h"
// #include "simulate_uart.h"
#include "MTF_io.h"

timer_flag_type app_timer_flag = {0, 0, 0, 0, 0, 0, 0, 0};

void app_timer(void)
{
    static uint8_t count_1ms = 0, count_2ms = 0, count_10ms = 0, count_100ms = 0;

    // 200us int
    app_timer_flag._200us = 1;
    // TEST_TOGGLE_PIN();

    /* Handle keyboard scan */
    // KB_Scan( );

    /* Handle mouse scan */
    // MS_Scan( );

    // simulate_uart_send();
    key_input_check_timer();
    // Led_Scan();
    
    if(++count_1ms>=5)
    {
        count_1ms = 0;
        app_timer_flag._1ms = 1;
    }

    if (++count_2ms >= 10)
    {
        count_2ms = 0;
        app_timer_flag._2ms = 1;
        
        count_10ms++;
        if (count_10ms >= 5)
        {
            count_10ms = 0;
            app_timer_flag._10ms = 1;

            count_100ms++;
            if(count_100ms>=10)
            {
                count_100ms = 0;
                app_timer_flag._100ms = 1;
            }
        }
    }
}
