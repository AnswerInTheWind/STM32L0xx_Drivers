
#include "flash.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define FLASH_USER_START_ADDR   0x8008000   /* Start @ of user Flash area */
//#define FLASH_USER_END_ADDR     0x800A000   /* End @ of user Flash area */

#define DATA_32                 ((uint32_t)0x12345678)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t PageError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;


static void Error_Handler(void);


void flash_erase(uint32_t flash_user_start_addr,uint32_t flash_user_end_addr)
{
	uint32_t NbOfPages = 0;
	   /* Get the number of sector to erase from 1st sector*/
  NbOfPages = (flash_user_end_addr - flash_user_start_addr + 1) >> 7;//Ò»¸öpage 128 ×Ö½Ú = 2>>7

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = flash_user_start_addr;
  EraseInitStruct.NbPages = NbOfPages;
  
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  { 
    /* 
      Error occurred while page erase. 
      User can add here some code to deal with this error. 
      PageError will contain the faulty page.
    */
    Error_Handler();
  }
}

void flash_program(uint32_t flash_user_start_addr,uint32_t flash_user_end_addr)
{
		uint32_t Address = 0;
	  Address = flash_user_start_addr;

		while (Address < flash_user_end_addr)
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
			{
				Address = Address + 4;
			}
			else
			{ 
				/* Error occurred while writing data in Flash memory. 
					 User can add here some code to deal with this error */
				/*
					FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
				*/
				Error_Handler();
			}
		}
}


void flash_unlock()
{
		/* Unlock the Flash to enable the flash control register access *************/ 
		HAL_FLASH_Unlock();
}

void flash_lock()
{
	  HAL_FLASH_Lock(); 
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
  }
}

