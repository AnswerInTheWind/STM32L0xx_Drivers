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

  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

extern uint8_t uart1_rxBuf[200];
extern uint16_t uart1_Rxcount;
extern uint8_t rx_complete;//接收完成标志位 0 表示没有收到数据或者接收没有完成

extern UART_HandleTypeDef UartHandle;


extern void UART_Init(void);
extern void Error_Handler(void);

extern void HAL_UART_SendBytes(uint8_t * str,uint16_t count,uint32_t Timeout);
extern void Abnormal_handle(UART_HandleTypeDef *huart);
extern void Rx_handle(UART_HandleTypeDef *huart);

#endif //__UART_BOARD_H__
