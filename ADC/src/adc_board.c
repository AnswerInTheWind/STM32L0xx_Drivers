#include "adc_board.h"
//#include "stm32l0xx_hal.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handle declaration */
ADC_HandleTypeDef        AdcHandle;

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef   sConfig;

/* Converted value declaration */
uint32_t                 uwConvertedValue;
/* Input voltage declaration */
uint32_t                 uwInputVoltage;

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);

/**
* @brief  ADC MSP Init
* @param  hadc : ADC handle
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __GPIOA_CLK_ENABLE();
  /* ADC1 Periph clock enable */
  __ADC1_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* ADC3 Channel8 GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
}


void adc_init()
{
	/* ### - 1 - Initialize ADC peripheral #################################### */
  /*
   *  Instance                  = ADC1.
   *  OversamplingMode          = Enabled
   *  .Oversample.Ratio         = x128
   *  .Oversample.RightBitShift = 3 bit shift
   *  .Oversample.TriggeredMode = single trigger (each trigger, all the oversampling series are performed).
   *  ClockPrescaler            = PCLK clock with no division.
   *  LowPowerAutoOff           = Disabled (For this exemple continuous mode is enabled with software start)
   *  LowPowerFrequencyMode     = Enabled (To be enabled only if ADC clock is lower than 2.8MHz) 
   *  LowPowerAutoWait          = Enabled (New conversion starts only when the previous conversion is completed)       
   *  Resolution                = 12 bit (increased to 16 bit with oversampler)
   *  SamplingTime              = 7.5 cycles od ADC clock.
   *  ScanDirection             = Upward 
   *  DataAlign                 = Right
   *  ContinuousConvMode        = Enabled
   *  DiscontinuousConvMode     = Enabled
   *  ExternalTrigConvEdge      = None (Software start)
   *  EOCSelection              = End Of Conversion event
   *  DMAContinuousRequests     = Disabled
   */

  AdcHandle.Instance = ADC1;
  
  AdcHandle.Init.OversamplingMode         = ENABLE;
  AdcHandle.Init.Oversample.Ratio         = ADC_OVERSAMPLING_RATIO_128;
  AdcHandle.Init.Oversample.RightBitShift = ADC_RIGHTBITSHIFT_3;
  AdcHandle.Init.Oversample.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV1;
  AdcHandle.Init.LowPowerAutoOff       = DISABLE;
  AdcHandle.Init.LowPowerFrequencyMode = ENABLE;
  AdcHandle.Init.LowPowerAutoWait      = ENABLE;
    
  AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  AdcHandle.Init.ScanDirection         = ADC_SCAN_DIRECTION_UPWARD;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIG_EDGE_NONE;
  AdcHandle.Init.EOCSelection          = EOC_SINGLE_CONV;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
 
  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
	
	 /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 3 - Channel configuration ######################################## */
  /* Select Channel 0 to be converted */
  sConfig.Channel = ADC_CHANNEL_0;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 4 - Start conversion ############################################# */
  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

uint32_t adc_getResult()
{  
    if (HAL_ADC_PollForConversion(&AdcHandle, 100) != HAL_OK)
    {
//      Error_Handler();
    }
    
    /* Read the converted value */
    uwConvertedValue = HAL_ADC_GetValue(&AdcHandle);

    /* Convert the result from 16 bit value to the voltage dimsension */
    /* Vref = 3.3 V */
    uwInputVoltage = (uwConvertedValue * 3300) / 0xFFF0; 
		
		return uwInputVoltage;
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Infinite loop */
  while(1)
  {
  }
}
