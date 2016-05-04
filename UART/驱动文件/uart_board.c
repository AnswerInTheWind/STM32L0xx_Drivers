/*
(C)2015 NPLink

Description: Uart driver implementation, used for uart press.

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Robxr
*/
#include "uart_board.h"
#include <string.h>
#include <stdio.h>


UART_HandleTypeDef UartHandle;

uint8_t uart1_rxBuf[200];
uint16_t uart1_Rxcount;
uint8_t rx_complete = 0;//接收完成标志位 0 表示没有收到数据或者接收没有完成
uint8_t step = 0;//状态机状态变量，0表示还未收到帧头，正在等待帧头；1表示收到帧头，正在接收帧的负载

void UART_Init(void)
{
//  char *test_str = "\nUART_Init Done!\n";
	UartHandle.Instance        = USARTx;
	UartHandle.Init.BaudRate   = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{

	}

	#ifdef USE_DEBUG
	HAL_UART_SendBytes( (uint8_t *)test_str , strlen(test_str) );
	#endif
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

/******************************************  INT  ************************************************************/
	/* Enable the UART Parity Error Interrupt */
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_PE);

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_ERR);

	/* Enable the UART Data Register not empty Interrupt */
	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);

		
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

/**
  * @brief HAL_UART_SendBytes
  *          - send data through uart1
  * @param 	str: 			source of data addr
	*					count:		data count of source data
	*					Timeout:	timeout of transmit the data(ms)
  * @retval None
  */
void HAL_UART_SendBytes(uint8_t * str,uint16_t count,uint32_t Timeout)
{
  if(HAL_UART_Transmit(&UartHandle, str, count, Timeout)!= HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    while(1)
    {
    }
}

/**
  * @brief  This function is executed in case of abnormal occurrence.
  * @param  UART_HandleTypeDef *huart
  * @retval None
  */
void Abnormal_handle(UART_HandleTypeDef *huart)
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
	
	if(huart->ErrorCode != HAL_UART_ERROR_NONE)
  {
    HAL_UART_ErrorCallback(huart);
  }
}

/**
  * @brief  This function is executed in case of rx.
  * @param  UART_HandleTypeDef *huart
  * @retval None
  */
void Rx_handle(UART_HandleTypeDef *huart)
{
		uint8_t ch = 0;
	/* UART in mode Receiver ---------------------------------------------------*/
  if((__HAL_UART_GET_IT(&UartHandle, UART_IT_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&UartHandle, UART_IT_RXNE) != RESET))
  { 
//    UART_Receive_IT(&UartHandle);
		ch = (uint8_t)(USART1->RDR );
		switch(step)
		{
			case 0:
				if(ch == '$')
				{
					step++;
					uart1_Rxcount= 0;
					uart1_rxBuf[uart1_Rxcount] = ch;
					uart1_Rxcount++;
					uart1_rxBuf[uart1_Rxcount] = 0;//将下一个存储单元的内容在数据到来之前先清零	
				}
				break;
				
			case 1:
				uart1_rxBuf[uart1_Rxcount++] = ch;
				uart1_rxBuf[uart1_Rxcount] = 0;//将下一个存储单元的内容在数据到来之前先清零	
				if((uart1_rxBuf[uart1_Rxcount-1] == 0X0A)&& (uart1_rxBuf[uart1_Rxcount-2] == 0X0D))//接收到符合24 ..... 0d 0a 的数据，一帧数据接收完成
				{                        				
					step = 0;
					rx_complete = 1;

				}
				if(uart1_Rxcount >= 199)//如果接收到199个字节都没有接收完成，表示接收异常了，则重新开始等待下一帧数据
				{
					step = 0;
				}				
				break;
				
			default:
				step = 0;
				break;
		}
		
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
	
	Abnormal_handle(&UartHandle);
	
	Rx_handle(&UartHandle);
  
}

