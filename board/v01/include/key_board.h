#ifndef _KEY_BOARD_H
#define _KEY_BOARD_H

#include "types_plus.h"

#define KEY_DATA_SIZE 5

extern uint8_t key_input_res[KEY_DATA_SIZE];

//注意是否需要互斥锁
#define key_get_result(x) key_input_res[x] //获取输入按键结果

void key_input_check_timer(void); //定期运行输入检查

#endif
