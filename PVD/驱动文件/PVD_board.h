#ifndef PVD_BOARD_H
#define PVD_BOARD_H

#include <stdint.h>

#define ONE_POINT_NINE		PWR_PVDLEVEL_0
#define TWO_POINT_ONE			PWR_PVDLEVEL_1
#define TWO_POINT_THREE		PWR_PVDLEVEL_2
#define TWO_POINT_FIVE		PWR_PVDLEVEL_3
#define TWO_POINT_SEVEN		PWR_PVDLEVEL_4
#define TWO_POINT_NINE		PWR_PVDLEVEL_5
#define THREE_POINT_ONE		PWR_PVDLEVEL_6
#define COMPARE_WITH_PB7	PWR_PVDLEVEL_7

#define PVD_MODE_EVT 							PWR_MODE_EVT                     /*!< No Interrupt */
#define PVD_MODE_RISING 					PWR_MODE_IT_RISING               /*!< External Interrupt Mode with Rising edge trigger detection */
#define PVD_MODE_FAILING 					PWR_MODE_IT_FALLING              /*!< External Interrupt Mode with Falling edge trigger detection */
#define PVD_MODE_RISING_FALLING 	PWR_MODE_IT_RISING_FALLING       /*!< External Interrupt Mode with Rising/Falling edge trigger detection */

/**
  * @brief  Configures the PVD resources.
  * @param  uint8_t PVD_Threshold	PVD Threshold volatage
  *			uint8_t edge			PVD_INT trigger edge
  * @retval None
  */
extern void PVD_Config(uint8_t PVD_Threshold,uint8_t edge);

#endif

