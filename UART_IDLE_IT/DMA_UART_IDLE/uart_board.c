/*
Description: Uart driver implementation, used for uart press.

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Derek Chen 
*/
#include <string.h>
#include <stdio.h>
#include "uart_board.h"
#include "stm32l0xx_hal.h"

#define UART1_RX_BUF_SIZE  128
#define UART1_DMA_TX_CACHE  1024

UART_HandleTypeDef UartHandle;

uart_t uart1;
uint8_t uart1_buf[UART1_RX_BUF_SIZE] = {0};

/*****************************FUNCTION PROTOTYPE***********************************************************************/
static uint8_t HAL_USART_GET_FLAG(USART_TypeDef * usartx,uint32_t flag);

/*****************************LOCAL FUNCTION***************************************************************************/
void UART_Init(void)
{
    
    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;
    
	UartHandle.Instance        = USARTx;
	UartHandle.Init.BaudRate   = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;

	HAL_UART_Init(&UartHandle);
    
    while( HAL_USART_GET_FLAG(USART1,UART_FLAG_IDLE)== RESET);//等待串口空闲标志位，上电会产生一次串口空闲标志位

    __HAL_UART_CLEAR_IT(&UartHandle, UART_CLEAR_IDLEF);//清楚空闲标志位
    
		/* Enable the UART Parity Error Interrupt */
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_PE);

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_ERR);
    
    
	/* Enable the UART Idle line detection  Interrupt */
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_IDLE);
    
    /* NVIC configuration for DMA transfer complete interrupt (USART1_RX) */
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);   
    HAL_NVIC_EnableIRQ(USART1_IRQn);
        
    /* Enable DMA1 clock */
    __DMA1_CLK_ENABLE();  
    
    /*##-3- Configure the DMA streams ##########################################*/
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance                 = DMA1_Channel2;

    hdma_tx.Init.Request             = DMA_REQUEST_3;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_tx);  

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */
    hdma_rx.Instance = DMA1_Channel3;

    hdma_rx.Init.Request             = DMA_REQUEST_3;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;  
    HAL_DMA_Init(&hdma_rx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

    /*##-4- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (USART1_TX) */
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

    /* NVIC configuration for DMA transfer complete interrupt (USART1_RX) */
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);   
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    
    uart1.p_rx_buf = uart1_buf;//设置rx_buf的指针
    uart1.rx_len = 0;
    uart1.tx_len = 0;
    uart1.uart_RxDone_flag = 0;//没有收到数据
    uart1.uart_TxDone_flag = 1;//可以进入发包状态
    uart1.rx_complete_callback = NULL;
    uart1.tx_complete_callback = NULL;
    
    HAL_UART_Receive_DMA(&UartHandle, (uint8_t*)uart1.p_rx_buf, UART1_RX_BUF_SIZE);
}



void HAL_UART_DMA_RX_stop(void)
{
    UartHandle.Instance->CR3 &= ~USART_CR3_DMAR;

    /* Abort the UART DMA rx channel */
    if(UartHandle.hdmarx != NULL)
    {
      HAL_DMA_Abort(UartHandle.hdmarx);
    }    
    
    UartHandle.State = HAL_UART_STATE_READY;
}

void HAL_DMA_UART_Send_Data(uint8_t* src,uint16_t len)
{
    static uint8_t DMA_cache[UART1_DMA_TX_CACHE] = {0};
    uint32_t time = 0;
    memcpy(DMA_cache,src,len);//把要发送的内容保存在DMA缓存区
    
    time = HAL_GetTick();
    while((!uart1.uart_TxDone_flag) && ((HAL_GetTick()- time)<150));//等待DMA发送完成,并且设置超时时间，防止大数据量程序以外死掉
    uart1.uart_TxDone_flag= 0;
    HAL_UART_Transmit_DMA(&UartHandle, DMA_cache, len);
}

uint8_t HAL_USART_GET_FLAG(USART_TypeDef * usartx,uint32_t flag)
{
	if((usartx->ISR & flag) == flag)
		return SET;
	else
		return RESET;
}


/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
 GPIO_InitTypeDef  GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  /* Enable USART2 clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;//GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
}

void HAL_UART_BandRateChange( uint32_t bandrate )
{
	UartHandle.Instance        = USARTx;
	UartHandle.Init.BaudRate   = bandrate;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;

	HAL_UART_Init(&UartHandle);
}

void error_process(UART_HandleTypeDef *huart)
{
    /* UART parity error interrupt occurred ------------------------------------*/
    if((__HAL_UART_GET_IT(huart, UART_IT_PE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE) != RESET))
    {
          __HAL_UART_CLEAR_IT(huart, UART_CLEAR_PEF);

          huart->ErrorCode |= HAL_UART_ERROR_PE;
          /* Set the UART state ready to be able to start again the process */
          huart->State = HAL_UART_STATE_READY;
    }

    /* UART frame error interrupt occured --------------------------------------*/
    if((__HAL_UART_GET_IT(huart, UART_IT_FE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET))
    {
          __HAL_UART_CLEAR_IT(huart, UART_CLEAR_FEF);

          huart->ErrorCode |= HAL_UART_ERROR_FE;
          /* Set the UART state ready to be able to start again the process */
          huart->State = HAL_UART_STATE_READY;
    }

    /* UART noise error interrupt occured --------------------------------------*/
    if((__HAL_UART_GET_IT(huart, UART_IT_NE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET))
    {
          __HAL_UART_CLEAR_IT(huart, UART_CLEAR_NEF);

          huart->ErrorCode |= HAL_UART_ERROR_NE;
          /* Set the UART state ready to be able to start again the process */
          huart->State = HAL_UART_STATE_READY;
    }

    /* UART Over-Run interrupt occured -----------------------------------------*/
    if((__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET))
    {
          __HAL_UART_CLEAR_IT(huart, UART_CLEAR_OREF);

          huart->ErrorCode |= HAL_UART_ERROR_ORE;
          /* Set the UART state ready to be able to start again the process */
          huart->State = HAL_UART_STATE_READY;
    }

    /* Call UART Error Call back function if need be --------------------------*/
    if(huart->ErrorCode != HAL_UART_ERROR_NONE)
    {
          HAL_UART_ErrorCallback(huart);
    }

    /* UART Wake Up interrupt occured ------------------------------------------*/
    if((__HAL_UART_GET_IT(huart, UART_IT_WUF) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_WUF) != RESET))
    {
          __HAL_UART_CLEAR_IT(huart, UART_CLEAR_WUF);
          /* Set the UART state ready to be able to start again the process */
          huart->State = HAL_UART_STATE_READY;
          HAL_UARTEx_WakeupCallback(huart);
    }
    
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    uart1.uart_TxDone_flag = 1;
    if(uart1.tx_complete_callback != NULL)
    {
        uart1.tx_complete_callback();
    }
}

/******************************************************************************/
/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream 
  *         used for USART data transmission     
  */
void USART1_IRQHandler(void)
{
    UART_HandleTypeDef *huart = &UartHandle;

    /* UART_IT_IDLE interrupt occured ------------------------------------------*/
    if((__HAL_UART_GET_IT(huart, UART_IT_IDLE) != RESET) && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET))
    {
        __HAL_UART_CLEAR_IT(huart, UART_CLEAR_IDLEF);
          /* Set the UART state ready to be able to start again the process */
        huart->State = HAL_UART_STATE_READY;
      
        uart1.rx_len = UART1_RX_BUF_SIZE - huart->hdmarx->Instance->CNDTR; //保存收到的数据的长度
        
        uart1.uart_RxDone_flag = 1;
      
        HAL_UART_DMA_RX_stop();//关闭DMA接收

        HAL_UART_Receive_DMA(&UartHandle, (uint8_t*)uart1.p_rx_buf, UART1_RX_BUF_SIZE);//开启DMA接收  

        if(uart1.rx_complete_callback != NULL)//检查回调函数是否为空
        {
            uart1.rx_complete_callback();//回调函数
        }        
        
    }
    
    error_process(huart);//异常处理
    
}

/**
  * @brief  This function handles DMA interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream 
  *         used for USART data transmission     
  */
void DMA1_Channel2_3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UartHandle.hdmatx);
    HAL_DMA_IRQHandler(UartHandle.hdmarx); 
}

