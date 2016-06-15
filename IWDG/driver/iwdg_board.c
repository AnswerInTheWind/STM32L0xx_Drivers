#include "iwdg_board.h"

IWDG_HandleTypeDef   IwdgHandle;

void IWDG_Init(uint16_t counter)
{
	/*##-3- Configure & Initialize the IWDG peripheral ######################################*/
	/* Set counter reload value to obtain 250ms IWDG TimeOut.
	 IWDG counter clock Frequency = LsiFreq/32
	 Counter Reload Value = 250ms/IWDG counter clock period
						  = 0.25s / (32/LsiFreq)
						  = LsiFreq/(32 * 4)
						  = LsiFreq/128 */
	IwdgHandle.Instance = IWDG;


	IwdgHandle.Init.Prescaler = IWDG_PRESCALER_128;
	IwdgHandle.Init.Reload = counter;//32.768KHZ 128分频 单次计数耗时约0.00390625s     
	IwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;
	
	HAL_IWDG_Init(&IwdgHandle);
	HAL_IWDG_Start(&IwdgHandle);
}

void IWDG_Feed()
{
	HAL_IWDG_Refresh(&IwdgHandle);
}
