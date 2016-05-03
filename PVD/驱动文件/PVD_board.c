#include "stm32l0xx_hal.h"
#include "PVD_board.h"


/* Private variables ---------------------------------------------------------*/
PWR_PVDTypeDef sConfigPVD;

/**
  * @brief  Configures the PVD resources.
  * @param  uint8_t PVD_Threshold	PVD Threshold volatage
  *			uint8_t edge			PVD_INT trigger edge
  * @retval None
  */
void PVD_Config(uint8_t PVD_Threshold,uint8_t edge)
{
	if(	(PVD_Threshold != ONE_POINT_NINE) && (PVD_Threshold != TWO_POINT_ONE) &&
			(PVD_Threshold != TWO_POINT_THREE) && (PVD_Threshold != TWO_POINT_FIVE) &&
			(PVD_Threshold != TWO_POINT_SEVEN) && (PVD_Threshold != TWO_POINT_NINE) &&
			(PVD_Threshold != THREE_POINT_ONE) && (PVD_Threshold != COMPARE_WITH_PB7))
		return ;
	
  /*##-1- Enable Power Clock #################################################*/
  __PWR_CLK_ENABLE();

  /*##-2- Configure the NVIC for PVD #########################################*/
  HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(PVD_IRQn);

  /* Configure the PVD Level to 3 and generate an interrupt on rising and falling
     edges(PVD detection level set to 2.5V, refer to the electrical characteristics
     of you device datasheet for more details) */
  sConfigPVD.PVDLevel = PVD_Threshold;
  sConfigPVD.Mode = PWR_MODE_IT_RISING_FALLING;
  HAL_PWR_PVDConfig(&sConfigPVD);

  /* Enable the PVD Output */
  HAL_PWR_EnablePVD();
}

