/********************************** (C) COPYRIGHT *******************************
 * File Name          : usbd_composite_km.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/20
 * Description        : USB keyboard and mouse processing.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/


/*******************************************************************************/
/* Header Files */
#include "usbd_composite_km.h"
#include "types_base.h"
#include "delay.h"
#include "gpio_board.h"

uint8_t USBD_ENDPx_DataUp( uint8_t endp, uint8_t *pbuf, uint16_t len );

/*******************************************************************************/
/* Variable Definition */

/* Mouse */
volatile uint8_t  MS_Scan_Done = 0x00;                                          // Mouse Movement Scan Done
volatile uint16_t MS_Scan_Result = 0x00F0;                                      // Mouse Movement Scan Result
uint8_t  MS_Data_Pack[ DEF_ENDP_SIZE_MS ] = { 0x00 };                           // Mouse IN Data Packet

/* Keyboard */
volatile uint8_t  KB_Scan_Done = 0x00;                                          // Keyboard Keys Scan Done
volatile uint16_t KB_Scan_Result = 0xF000;                                      // Keyboard Keys Current Scan Result
volatile uint16_t KB_Scan_Last_Result = 0xF000;                                 // Keyboard Keys Last Scan Result
uint8_t  KB_Data_Pack[ DEF_ENDP_SIZE_KB ] = { 0x00 };                           // Keyboard IN Data Packet
volatile uint8_t  KB_LED_Last_Status = 0x00;                                    // Keyboard LED Last Result
volatile uint8_t  KB_LED_Cur_Status = 0x00;                                     // Keyboard LED Current Result

/* USART */
volatile uint8_t  USART_Recv_Dat = 0x00;
volatile uint8_t  USART_Send_Flag = 0x00;
volatile uint8_t  USART_Send_Cnt = 0x00;

/*******************************************************************************/
/* Interrupt Function Declaration */
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      USART2_Init
 *
 * @brief   Initialize UART2 to receive keyboard data sent through the
 *          PC serial software.
 *
 * @param   baudrate - Serial baud rate
 *
 * @return  none
 */
void USART2_Init( uint32_t baudrate )
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    USART_InitTypeDef USART_InitStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init( USART2, &USART_InitStructure );
    USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
    USART_Cmd( USART2, ENABLE );
}

/*********************************************************************
 * @fn      USART2_IRQHandler
 *
 * @brief   This function handles USART2 global interrupt request.
 *
 * @return  none
 */
void USART2_IRQHandler( void )
{
    if( USART_GetITStatus( USART2, USART_IT_RXNE) != RESET )
    {
        /* Save the key value received from USART2 */
        USART_Recv_Dat = USART_ReceiveData( USART2 ) & 0xFF;
    }
}

/*********************************************************************
 * @fn      USART2_Receive_Handle
 *
 * @brief   This function handles the key value received from USART2.
 *
 * @return  none
 */
void USART2_Receive_Handle( void )
{
    uint8_t status;
    static uint8_t flag = 0x00;

    if( flag == 0 )
    {
        /* Store the received specified key value into the keyboard data buffer */
        if( ( USART_Recv_Dat == DEF_KEY_CHAR_A ) ||
            ( USART_Recv_Dat == DEF_KEY_CHAR_W ) ||
            ( USART_Recv_Dat == DEF_KEY_CHAR_S ) ||
            ( USART_Recv_Dat == DEF_KEY_CHAR_D ) )
        {
            memset( KB_Data_Pack, 0x00, DEF_ENDP_SIZE_KB );
            KB_Data_Pack[ 2 ] = USART_Recv_Dat;
            flag = 1;
        }
    }
    else if( flag == 1 )
    {
        /* Load keyboard data to endpoint 1 */
        status = USBD_ENDPx_DataUp(ENDP1, KB_Data_Pack, DEF_ENDP_SIZE_KB);

        if( status == USB_SUCCESS )
        {
            /* Enable timing for uploading the key value */
            USART_Send_Cnt = 0;
            USART_Send_Flag = 1;
            flag = 2;
        }
    }
    else if( flag == 2 )
    {
        /* Delay 10ms to ensure that the key value is successfully uploaded,
         * and prepare the data packet indicating the key release.
         */
        if( USART_Send_Cnt >= 50 )
        {
            USART_Send_Flag = 0;
            memset( KB_Data_Pack, 0x00, DEF_ENDP_SIZE_KB );
            flag = 3;
        }
    }
    else if( flag == 3 )
    {
        /* Load keyboard data to endpoint 1 */
        status = USBD_ENDPx_DataUp(ENDP1, KB_Data_Pack, DEF_ENDP_SIZE_KB);
        /* Clear variables for next reception */
        if( status == USB_SUCCESS )
        {
            USART_Recv_Dat = 0;
            flag = 0;
        }
    }
}

/*********************************************************************
 * @fn      KB_Scan_Init
 *
 * @brief   Initialize IO for keyboard scan.
 *
 * @return  none
 */
void KB_Scan_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    /* Enable GPIOB clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );

    /* Initialize GPIOB (Pin4-Pin7) for the keyboard scan */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
}

/*********************************************************************
 * @fn      KB_Sleep_Wakeup_Cfg
 *
 * @brief   Configure keyboard wake up mode.
 *
 * @return  none
 */
void KB_Sleep_Wakeup_Cfg( void )
{
 EXTI_InitTypeDef EXTI_InitStructure = { 0 };

    /* Enable GPIOB clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource12 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource13 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource14 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource15 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    EXTI->INTENR |= EXTI_INTENR_MR12 | EXTI_INTENR_MR13 | EXTI_INTENR_MR14 | EXTI_INTENR_MR15;
}

/*********************************************************************
 * @fn      KB_Scan
 *
 * @brief   Perform keyboard scan.
 *
 * @return  none
 */
void KB_Scan( void )
{
    static uint16_t scan_cnt = 0;
    static uint16_t scan_result = 0;

    scan_cnt++;
    if( ( scan_cnt % 10 ) == 0 )
    {
        scan_cnt = 0;

        /* Determine whether the two scan results are consistent */
        if( scan_result == ( GPIO_ReadInputData( GPIOB ) & 0xF000 ) )
        {
            KB_Scan_Done = 1;
            KB_Scan_Result = scan_result;
        }
    }
    else if( ( scan_cnt % 5 ) == 0 )
    {
        /* Save the first scan result */
        scan_result = ( GPIO_ReadInputData( GPIOB ) & 0xF000 );
    }
}

/*********************************************************************
 * @fn      KB_Scan_Handle
 *
 * @brief   Handle keyboard scan data.
 *
 * @return  none
 */
void KB_Scan_Handle( void )
{
    uint8_t i, j;
    uint8_t status;
    static uint8_t key_cnt = 0x00;
    static uint8_t flag = 0x00;
    if( KB_Scan_Done )
    {
        KB_Scan_Done = 0;

        if( KB_Scan_Result != KB_Scan_Last_Result )
        {
            for( i = 12; i < 16; i++ )
            {
                /* Determine that there is at least one key is pressed or released */
                if( ( KB_Scan_Result & ( 1 << i ) ) != ( KB_Scan_Last_Result & ( 1 << i ) ) )
                {
                    if( ( KB_Scan_Result & ( 1 << i ) ) )           // Key press
                    {
                        if( i == 12 )
                        {
                            for( j = 2; j < 8; j++ )
                            {
                                if( KB_Data_Pack[ j ] == DEF_KEY_CHAR_W )
                                {
                                    break;
                                }
                            }
                        }
                        else if( i == 13 )
                        {
                            for( j = 2; j < 8; j++ )
                            {
                                if( KB_Data_Pack[ j ] == DEF_KEY_CHAR_A )
                                {
                                    break;
                                }
                            }
                        }
                        else if( i == 14 )
                        {
                            for( j = 2; j < 8; j++ )
                            {
                                if( KB_Data_Pack[ j ] == DEF_KEY_CHAR_S )
                                {
                                    break;
                                }
                            }
                        }
                        else if( i == 15 )
                        {
                            for( j = 2; j < 8; j++ )
                            {
                                if( KB_Data_Pack[ j ] == DEF_KEY_CHAR_D )
                                {
                                    break;
                                }
                            }
                        }

                        if( j == 8 )
                        {
                            KB_Data_Pack[ 5 ] = 0;
                        }
                        else
                        {
                            memcpy( &KB_Data_Pack[ j ], &KB_Data_Pack[ j + 1 ], ( 8 - j - 1 ) );
                            KB_Data_Pack[ 7 ] = 0;
                        }
                        key_cnt--;
                    }
                    else                                            // Key release
                    {
                        if( i == 12 )
                        {
                            KB_Data_Pack[ 2 + key_cnt ] = DEF_KEY_CHAR_W;
                        }
                        else if( i == 13 )
                        {
                            KB_Data_Pack[ 2 + key_cnt ] = DEF_KEY_CHAR_A;
                        }
                        else if( i == 14 )
                        {
                            KB_Data_Pack[ 2 + key_cnt ] = DEF_KEY_CHAR_S;
                        }
                        else if( i == 15 )
                        {
                            KB_Data_Pack[ 2 + key_cnt ] = DEF_KEY_CHAR_D;
                        }
                        key_cnt++;
                    }
                }
            }

            /* Copy the keyboard data to the buffer of endpoint 1 and set the data uploading flag */
            KB_Scan_Last_Result = KB_Scan_Result;
            flag = 1;
        }
    }

    if( flag )
    {
        /* Load keyboard data to endpoint 1 */
        status = USBD_ENDPx_DataUp(ENDP1, KB_Data_Pack, DEF_ENDP_SIZE_KB);
        if( status == USB_SUCCESS )
        {
            /* Clear flag after successful loading */
            flag = 0;
        }
    }
    
}


/*********************************************************************
 * @fn      KB_LED_Handle
 *
 * @brief   Handle keyboard lighting.
 *
 * @return  none
 */
void KB_LED_Handle( void )
{
    if( KB_LED_Cur_Status != KB_LED_Last_Status )
    {
        if( ( KB_LED_Cur_Status & 0x01 ) != ( KB_LED_Last_Status & 0x01 ) )
        {
            if( KB_LED_Cur_Status & 0x01 )
            {
                printf("Turn on the NUM LED\r\n");
            }
            else
            {
                printf("Turn off the NUM LED\r\n");
            }
        }
        if( ( KB_LED_Cur_Status & 0x02 ) != ( KB_LED_Last_Status & 0x02 ) )
        {
            if( KB_LED_Cur_Status & 0x02 )
            {
                printf("Turn on the CAPS LED\r\n");
            }
            else
            {
                printf("Turn off the CAPS LED\r\n");
            }
        }
        if( ( KB_LED_Cur_Status & 0x04 ) != ( KB_LED_Last_Status & 0x04 ) )
        {
            if( KB_LED_Cur_Status & 0x04 )
            {
                printf("Turn on the SCROLL LED\r\n");
            }
            else
            {
                printf("Turn off the SCROLL LED\r\n");
            }
        }
        KB_LED_Last_Status = KB_LED_Cur_Status;
    }
}


/*********************************************************************
 * @fn      MS_Scan_Init
 *
 * @brief   Initialize IO for mouse scan.
 *
 * @return  none
 */
void MS_Scan_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
}

/*********************************************************************
 * @fn      MS_Sleep_Wakeup_Cfg
 *
 * @brief   Configure mouse wake up mode.
 *
 * @return  none
 */
void MS_Sleep_Wakeup_Cfg( void )
{
    EXTI_InitTypeDef EXTI_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOA, GPIO_PinSource4 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOA, GPIO_PinSource5 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOA, GPIO_PinSource6 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOA, GPIO_PinSource7 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    EXTI->INTENR |= EXTI_INTENR_MR4 | EXTI_INTENR_MR5 | EXTI_INTENR_MR6 | EXTI_INTENR_MR7;
}

/*********************************************************************
 * @fn      MS_Scan
 *
 * @brief   Perform mouse scan.
 *
 * @return  none
 */
void MS_Scan( void )
{
    static uint16_t scan_cnt = 0;
    static uint16_t scan_result = 0;

    scan_cnt++;
    if( scan_cnt >= 2 )
    {
        scan_cnt = 0;

        /* Determine whether the two scan results are consistent */
        if( scan_result == ( GPIO_ReadInputData( GPIOA ) & 0x00F0 ) )
        {
            MS_Scan_Result = scan_result;
            MS_Scan_Done = 1;
        }
    }
    else if( scan_cnt >= 1 )
    {
        /* Save the first scan result */
        scan_result = ( GPIO_ReadInputData( GPIOA ) & 0x00F0 );
    }
}

/*********************************************************************
 * @fn      MS_Scan_Handle
 *
 * @brief   Handle mouse scan data.
 *
 * @return  none
 */
void MS_Scan_Handle( void )
{
    uint8_t i;
    uint8_t status;
    static uint8_t flag = 0x00;

    if( MS_Scan_Done )
    {
        MS_Scan_Done = 0;

        memset( MS_Data_Pack, 0x00, DEF_ENDP_SIZE_MS );

        for( i = 4; i < 8; i++ )
        {
            /* Determine that the mouse is moved */
            if( ( MS_Scan_Result & ( 1 << i ) ) == 0 )
            {
                if( i == 4 )
                {
                    MS_Data_Pack[ 1 ] += 0x02;
                }
                else if( i == 5 )
                {
                    MS_Data_Pack[ 1 ] += 0xFE;
                }
                else if( i == 6 )
                {
                    MS_Data_Pack[ 2 ] += 0x02;
                }
                else if( i == 7 )
                {
                    MS_Data_Pack[ 2 ] += 0xFE;
                }

                /* Set the data uploading flag */
                flag = 1;
            }
        }
    }
    if( flag )
    {
        /* Load mouse data to endpoint 2 */
        status = USBD_ENDPx_DataUp(ENDP2, MS_Data_Pack, DEF_ENDP_SIZE_MS);
        if( status == USB_SUCCESS )
        {
            /* Clear flag after successful loading */
            flag = 0;
        }
    }
}

/*********************************************************************
 * @fn      Jostick_Scan_Handle
 *
 * @brief   Handle jostick scan data.
 *
 * @return  none
 */
/* Jostick */
#include "adc_board.h"
#include "envent_define.h"
// uint8_t Jostick_Scan_Done = 0; //data value changed                          
uint8_t Jostick_Data_Pack[DEF_ENDP_SIZE_JOSTICK] = {0};                           // jostick IN Data Packet
static uint8_t Jostick_old_Data_Pack[DEF_ENDP_SIZE_JOSTICK] = {0};                           // jostick IN Data Packet
void Jostick_Scan_Handle( void )
{
    // static uint8_t i = 0;
    // uint8_t ll = 0;
    uint8_t status = 0;
    static uint8_t flag = 0x00;
    char vr_x = 0, vr_y = 0;

    if( bDeviceState != CONFIGURED ) //USB????????????
        return;

#if 0 //for demo
    if( Jostick_Scan_Done )
    {
        Jostick_Scan_Done = 0;

        // memset( Jostick_Data_Pack, 0x00, DEF_ENDP_SIZE_JOSTICK );

        if (++i >= 8)
        {
            i = 0;
        }
        
        Jostick_Data_Pack[0] = 10*i;
        // Jostick_Data_Pack[1] = 13*i;
        // Jostick_Data_Pack[2] = -13*i;
        Jostick_Data_Pack[3] = -8*i;
        Jostick_Data_Pack[4] = -5*i;
        Jostick_Data_Pack[5] = 13*i;

        Jostick_Data_Pack[6] = 0X80>>i;
        Jostick_Data_Pack[7] = 0XC0>>i;
        Jostick_Data_Pack[8] = 0X03<<i;
        // Jostick_Data_Pack[9] = 0X07<<i;

        //change rang: -127~127
        vr_x = ADC_VR2_VALUE() >> 4;
        vr_x = vr_x - 128;
        if (vr_x > 127)
        {
            vr_x = 127;
        }
        else if (vr_x < -127)
        {
            vr_x = -127;
        }

        //change rang: -127~127
        vr_y = ADC_VR1_VALUE() >> 4;
        vr_y = vr_y - 128;
        if (vr_y > 127)
        {
            vr_y = 127;
        }
        else if (vr_y < -127)
        {
            vr_y = -127;
        }

        // printf("X = %d, Y = %d\r\n", vr_x, vr_y);
        Jostick_Data_Pack[1] = vr_x;
        Jostick_Data_Pack[2] = vr_y;

        if(KEY_6_PIN()==0)
        {
            ll |= 0X01;
            MODE1_LED_PIN(1);
        }
        else
        {
            MODE1_LED_PIN(0);
        }
        if(KEY_7_PIN()==0)
        {
            ll |= 0X02;
            MODE2_LED_PIN(1);
        }
        else
        {
            MODE2_LED_PIN(0);
        }
        if(KEY_8_PIN()==0)
        {
            ll |= 0X04;
            MODE3_LED_PIN(1);
        }
        else
        {
            MODE3_LED_PIN(0);
        }
        if(KEY_9_PIN()==0)
        {
            ll |= 0X08;
        }
        else
        {
        }
        Jostick_Data_Pack[9] = ll;

        flag = 1;
    }
#else

    Jostick_Data_Pack[0] = 0; //throttle

    Jostick_Data_Pack[3] = 0; //z
    Jostick_Data_Pack[4] = 0; //rx
    Jostick_Data_Pack[5] = 0; //ry

    //change rang: -127~127
    vr_x = ADC_VR2_VALUE() >> 4;
    vr_x = vr_x - 128;
    if (vr_x > 127)
    {
        vr_x = 127;
    }
    else if (vr_x < -127)
    {
        vr_x = -127;
    }

    //change rang: -127~127
    vr_y = ADC_VR1_VALUE() >> 4;
    vr_y = vr_y - 128;
    if (vr_y > 127)
    {
        vr_y = 127;
    }
    else if (vr_y < -127)
    {
        vr_y = -127;
    }

    // printf("X = %d, Y = %d\r\n", vr_x, vr_y);
    Jostick_Data_Pack[1] = vr_x;
    Jostick_Data_Pack[2] = vr_y;

    //key
    Jostick_Data_Pack[6] = jostick_key[0];
    Jostick_Data_Pack[7] = 0;
    Jostick_Data_Pack[8] = 0;
    Jostick_Data_Pack[9] = 0;

    for (size_t i = 0; i < DEF_ENDP_SIZE_JOSTICK; i++)
    {
        if(Jostick_old_Data_Pack[i]!=Jostick_Data_Pack[i])
        {
            Jostick_old_Data_Pack[i] = Jostick_Data_Pack[i];
            flag = 1;
        }
    }

#endif

    if( flag )
    {
        /* Load mouse data to endpoint 2 */
        status = USBD_ENDPx_DataUp(ENDP2, Jostick_Data_Pack, DEF_ENDP_SIZE_JOSTICK);
        if( status == USB_SUCCESS )
        {
            /* Clear flag after successful loading */
            flag = 0;
        }
    }
}

/*********************************************************************
 * @fn      MCU_Sleep_Wakeup_Operate
 *
 * @brief   Perform sleep operation
 *
 * @return  none
 */
void MCU_Sleep_Wakeup_Operate( void )
{
    EXTI_ClearFlag( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 );
    EXTI_ClearFlag( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 );

//    printf( "Sleep\r\n" );
    __WFE( );
    
    if( EXTI_GetFlagStatus( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 ) != RESET  )
    {
        EXTI_ClearFlag( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 );
        Resume(RESUME_INTERNAL);
    }
    else if( EXTI_GetFlagStatus( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 ) != RESET )
    {
        EXTI_ClearFlag( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 );
        Resume(RESUME_INTERNAL);
    }
//    printf( "Wake\r\n" );
}



