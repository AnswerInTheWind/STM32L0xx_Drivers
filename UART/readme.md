###	STM32L05x UART 驱动程序及测试例程 ###

由于官方库中的cube库中的串口驱动在串口接收时只能接收定长的数据，在实际使用时经常感到不方便，所以在cube库的基础上，我做了一些修改，
修改如下：
	
	1.在初始化时开启接收中断，在之后的使用时不需要对串口在进行开关操作
	2.在串口中断的处理函数中增了帧的处理，当接收到符合条件的一帧数据是，标志位置1，这样可以处理不定长带帧格式的数据
	3.把串口的中断处理函数防止在uart_board.c文件中


	
数据帧格式如下：

	frame header | payload | frame tail
	------------------------------------
	0x24		 |		   |	0d 0a

此驱动代码是针对STM32L05x系列芯片的

串口的配置为：115200 8 N 1

驱动文件为：

	uart_board.c

	uart_board.h

本测试例程的测试方法为：
	
	向单片机发送数据
	pc -> MCU 24 01 02 03 04 05 06 07 08 09 0d 0a （hex格式）
	
	单片机将收到的数据原封不动地发送回来
	MCU -> PC 24 01 02 03 04 05 06 07 08 09 0d 0a （hex格式）

在测试时使用10ms的间隔PC->MCU发送数据，发送了4W+个字节，0%丢失错误。



