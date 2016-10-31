#ifndef __FLASH_H__
#define __FLASH_H__


#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"



extern void flash_erase(uint32_t flash_user_start_addr,uint32_t flash_user_end_addr);

extern void flash_program(uint32_t flash_user_start_addr,uint32_t flash_user_end_addr);

extern void flash_unlock(void);

extern void flash_lock(void);

#endif

