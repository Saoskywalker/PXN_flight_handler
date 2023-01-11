/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#ifndef _DELAY_H
#define _DELAY_H

#include "stdint.h"
#include "debug.h"

static inline void delay(uint32_t ms)
{
    Delay_Ms(ms);
}

static inline void delay_us(uint32_t us)
{
    Delay_Us(us);
}

static inline void delay_ms(uint32_t ms)
{
    Delay_Ms(ms);
}

#if defined(__CC_ARM)

// static __inline void sdelay(int loops)
// {
// 	delay_us(loops);
// }

#else

// static inline void sdelay(int loops)
// {
// 	__asm__ __volatile__ ("1:\n" "subs %0, %1, #1\n"
// 		"bne 1b":"=r" (loops):"0"(loops));
// }

#endif

#endif
