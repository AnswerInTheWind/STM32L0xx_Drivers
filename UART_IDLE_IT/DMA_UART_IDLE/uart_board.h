/*
Description: Uart driver implementation, used for Uart.

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Derek Chen
*/
/**************************************************
 * @fn		HAL_UART_Init
 *
 * @brief 	initialize Uart
 *
 * @return	void
 */ 
#ifndef __UART_BOARD_H__
#define __UART_BOARD_H__

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_uart.h"

#define USARTx                            USART1
#define USARTx_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_6
#define USARTx_TX_GPIO_PORT              GPIOB  
#define USARTx_TX_AF                     GPIO_AF0_USART1
#define USARTx_RX_PIN                    GPIO_PIN_7
#define USARTx_RX_GPIO_PORT              GPIOB 
#define USARTx_RX_AF                     GPIO_AF0_USART1

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler


typedef struct UART
{
    uint8_t* p_rx_buf;
    uint16_t rx_len;
    uint8_t tx_len;
    uint8_t uart_TxDone_flag;
    uint8_t uart_RxDone_flag;
    void (*rx_complete_callback)(void);
    void (*tx_complete_callback)(void);
}uart_t;


/* Exported variable ------------------------------------------------------------*/
extern uart_t uart1;

/* Exported functions ------------------------------------------------------- */
#define HAL_UART_SendBytes(m,n)     HAL_DMA_UART_Send_Data(m,n)

extern void UART_Init(void);
extern void HAL_UART_MspInit(UART_HandleTypeDef *huart);
extern void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);
extern void HAL_UART_BandRateChange(uint32_t bandrate);
extern void HAL_DMA_UART_Send_Data(uint8_t* src,uint16_t len);


#endif //__UART_BOARD_H__
