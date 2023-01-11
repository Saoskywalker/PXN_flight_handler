/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#include "delay.h"

#define CPU_FREQUENCY 1440 //CPU: 144M

#if CPU_FREQUENCY == 3300
#define _MS_CNT 450000
#define _US_CNT 400
#else
#define _MS_CNT 51000
#define _US_CNT 36
#endif
