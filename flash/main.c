/**
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram/Src/main.c 
  * @author  MCD Application Team
  * @version V1.7.0
  * @date    31-May-2016
  * @brief   This example provides a description of how to erase and program the 
  *          STM32L0xx Flash through the STM32L0xx HAL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "flash.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup FLASH_Erase
  * @{
  */ 

///* Private typedef -----------------------------------------------------------*/
///* Private define ------------------------------------------------------------*/
#define FLASH_USER_START_ADDR   0x8008000   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     0x800A000   /* End @ of user Flash area */

//#define DATA_32                 ((uint32_t)0x12345678)

///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/
//uint32_t FirstPage = 0, NbOfPages = 0, Address = 0;
//uint32_t PageError = 0;
//__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

///*Variable used for Erase procedure*/
//static FLASH_EraseInitTypeDef EraseInitStruct;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();
  
 /* Initialize LED2*/
  BSP_LED_Init(LED2);
  
  /* Configure the system clock to 2 Mhz */
  SystemClock_Config();
	
	flash_unlock();
	
	flash_erase(FLASH_USER_START_ADDR,FLASH_USER_END_ADDR);
	
	flash_program(FLASH_USER_START_ADDR,FLASH_USER_END_ADDR);
	
	flash_lock();
	
	
	

//  /* Unlock the Flash to enable the flash control register access *************/ 
//  HAL_FLASH_Unlock();

//  /* Erase the user Flash area
//    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

//   /* Get the number of sector to erase from 1st sector*/
//  NbOfPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR + 1) >> 7;

//  /* Fill EraseInit structure*/
//  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
//  EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
//  EraseInitStruct.NbPages = NbOfPages;
//  
//  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
//  { 
//    /* 
//      Error occurred while page erase. 
//      User can add here some code to deal with this error. 
//      PageError will contain the faulty page.
//    */
//    Error_Handler();
//  }

//  /* Program the user Flash area word by word
//    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

//  Address = FLASH_USER_START_ADDR;

//  while (Address < FLASH_USER_END_ADDR)
//  {
//    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
//    {
//      Address = Address + 4;
//    }
//    else
//    { 
//      /* Error occurred while writing data in Flash memory. 
//         User can add here some code to deal with this error */
//      /*
//        FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
//      */
//      Error_Handler();
//    }
//  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
//  HAL_FLASH_Lock(); 

  /* Check if the programmed data is OK 
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
//  Address = FLASH_USER_START_ADDR;
//  MemoryProgramStatus = 0x0;
//  
//  while (Address < FLASH_USER_END_ADDR)
//  {
//    data32 = *(__IO uint32_t*)Address;

//    if (data32 != DATA_32)
//    {
//      MemoryProgramStatus++;  
//    }

//    Address = Address + 4;
//  }  

//  /*Check if there is an issue to program data*/
//  if (MemoryProgramStatus == 0)
//  {
//    /* No error detected. Switch on LED2*/
//    BSP_LED_On(LED2);
//  }
//  else
//  {
//    /* Error detected. Switch off LED2*/
//    Error_Handler();
//  }
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Infinite loop */
  BSP_LED_Off(LED2);
  while(1)
  {
  }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
