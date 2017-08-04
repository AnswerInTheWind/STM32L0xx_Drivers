# UART_IDLE_IT #
空闲中断是STM32L0xx有提供的一种判断数据接收完成的中断

当有接收到数据，然后RX波形停止10bit之后，会产生一次空闲中断
Rx没有收到数据，并不会产生空闲中断.

## DMA_UART_IDLE ##
DMA 方式结合 UART_IDLE_IT方式

DMA做数据收发，
UART 空闲中断用来判断数据接收完成
CPU基本不需要参数串口数据的收发工作

可以有效减少大叔两两的情况下的CPU的负载，使其更好地工作。

## IT_UART_IDLE ##
采用中断接收和非中断发送的方式来进行串口才做

相较DMA方式，代码更加简单，而且逻辑也清晰

## 结构体分析 ##
```c
	typedef struct UART
	{
	    uint8_t* p_rx_buf;
	    uint16_t rx_len;
	    uint16_t tx_len;
	    uint8_t uart_TxDone_flag;
	    uint8_t uart_RxDone_flag;
	    void (*rx_complete_callback)(void);
	    void (*tx_complete_callback)(void);
	}uart_t;

```
其中:

	- p_rx_buf指向uart的RX的存储区域,
	- rx_len指rx收到的数据的长度,
	- tx_len指tx收到的数据的长度,
	- uart_TxDone_flag指uart tx发送完成之后的flag,
	- uart_RxDone_flag指uart Rx接收完成之后的flag,
	-rx_complete_callback 是uart rx接收完成之后的回调函数，若有注册，即会调用注册的回调函数
	-tx_complete_callback 是uart tx发送完成之后的回调函数，若有注册，即会调用注册的回调函数
