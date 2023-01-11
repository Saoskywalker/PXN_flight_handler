/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#ifndef _WATCH_DOG_PORT_H
#define _WATCH_DOG_PORT_H

#include "ch32v20x.h"

#ifndef _USER_DEBUG

static inline void MTF_watch_dog_init(void) //start WDOG, 3.2s IWDG reset
{
    
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(4000);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

static inline void MTF_watch_dog_feed(void) //feed WDOG
{
    IWDG_ReloadCounter();	//Feed dog
}

#else

#define MTF_watch_dog_init()
#define MTF_watch_dog_feed()

#endif

#endif
