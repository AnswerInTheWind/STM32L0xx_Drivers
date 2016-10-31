/*
(C)2015 NPLink

Description: Target board general functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Robxr
*/

#ifndef __IWDG_BOARD_H__
#define __IWDG_BOARD_H__

#include <stdint.h>
#include "stm32l051xx.h"
#include "stm32l0xx_hal.h"

extern void IWDG_Init(uint16_t counter);

extern void IWDG_Feed(void);


#endif 
