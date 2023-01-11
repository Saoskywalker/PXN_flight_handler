/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/08/08
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/ 

/*
 * @Note
 * Composite Keyboard and Mouse Example:
 * This example uses PA4-PA7 and PB12-PB15 to simulate mouse movement and keyboard
 * key pressing respectively, active low. At the same time, it also uses USART2
 * to receive data sent from the host to simulate keyboard key pressing and releasing.
 *
 */

#include "app_global.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_prop.h"
#include "usbd_composite_km.h"
#include "app_global.h"
#include "types_base.h"
#include "timer_port.h"
#include "gpio_board.h"
#include "adc_board.h"
#include "watch_dog_port.h"
#include "envent_define.h"
#include "app_timer.h"
/* Global define */

/* Global Variable */
app_flag_type app_flag = {0};
uint8_t app_work_mode = MODE_A;
uint8_t app_battery_level = BATTERY_FULL;

/**************
 * 电池电量处理
 * *****************/
/* static void battery_deal(void)
{	
} */

/*****************************
 * 睡眠处理
***************************/
/* uint16_t wakeup_overtime_cnt = 0; //睡眠唤醒超时
static void sleep(void)
{
	if(app_flag.sleep)
	{
		Led_Display_exit();
		MM_adc1_suspend();
		PWM_SUSPEND();
		main_IO_exit();

		while (app_flag.sleep)
		{
			MTF_watch_dog_exit();
			MTF_timer_suspend();
			MTF_sys_stop(); //进入休眠模式
			MTF_timer_start();
			MTF_watch_dog_init();
			wakeup_overtime_cnt = 0;
			while (wakeup_overtime_cnt < 150)
			{
				MTF_watch_dog_feed();
				app_flag.sleep_updata = 1;
				if (app_timer_flag._10ms)
				{
					app_timer_flag._10ms = 0;
					event_produce();
					event_handle();
					wakeup_overtime_cnt++;
				}
				app_timer_flag._200us = 0;
				app_timer_flag._2ms = 0;
				app_timer_flag._100ms = 0;
				if (app_flag.sleep == 0)
				{
					break;
				}
			}
		}
		
		main_IO_init();
		PWM_START();
		MM_adc1_start();
		Led_Display_init();
	}
} */

/*********************************
 * hardware init
 * *********************************/
static void hardware_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    main_IO_init();
    MM_adc1_init();

#ifndef _USER_DEBUG
    USART_Printf_Init( 115200 );
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("USBD Keyboard&Mouse Demo\r\n");
#endif    
    
    /* Initialize USART2 for receiving the specified keyboard data */
    // USART2_Init( 115200 );
    // printf( "USART2 Init OK!\r\n" );

    /* Initialize GPIO for keyboard scan */
    // KB_Scan_Init( );
    // KB_Sleep_Wakeup_Cfg( );
    // printf( "KB Scan Init OK!\r\n" );

    /* Initialize GPIO for mouse scan */
    // MS_Scan_Init( );
    // MS_Sleep_Wakeup_Cfg( );
    // printf( "MS Scan Init OK!\r\n" );

    MTF_timer_init_handle();

    //init usb
    Set_USBConfig();
    USB_Init();
    USB_Interrupts_Config();

    MTF_watch_dog_init();
}

/**************************
 * app init
 * ****************************/
static void app_init(void)
{

}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    uint8_t sys_read_delay = 0, sleep_updata_delay = 0;

    hardware_init();
    app_init();
    
    while(1)
    {
        MTF_watch_dog_feed();

        if (app_timer_flag._200us)
		{
			app_timer_flag._200us = 0;
			adc_run();
		}

		if (app_timer_flag._1ms)
		{
			app_timer_flag._1ms = 0;
			// temp_deal();
			// battery_deal();
			// motor_current_deal();
			if (app_flag.sleep_updata == 0)
			{
				event_produce();
				event_handle();
				Jostick_Scan_Handle();
				// Led_display();
				// pcb_produce_test();
			}
		}

		if (app_timer_flag._100ms)
		{
			app_timer_flag._100ms = 0;

			if(app_flag.sys_ready==0)
			{
                sys_read_delay++;
				if(sys_read_delay>=10) //上电等待系统稳定
				{
					sys_read_delay = 0;
					app_flag.sys_ready = 1;
					app_flag.disp_battery_level = 1;
					app_flag.sleep = 0;
					app_flag.work = 1;
				}
			}

			if (app_flag.sleep_updata)
			{
				sleep_updata_delay++;
				if (sleep_updata_delay >= 2)
				{
					sleep_updata_delay = 0;
					app_flag.sleep_updata = 0;
				}
			}
		}

        // sleep();

#if 0
        if( bDeviceState == CONFIGURED )
        {
            /* Handle keyboard scan data */
            KB_Scan_Handle(  );

            /* Handle keyboard lighting */
            KB_LED_Handle( );

            /* Handle mouse scan data */
            MS_Scan_Handle();

            /* Handle USART2 receiving data */
            USART2_Receive_Handle( );
        }
#endif
    }
}







