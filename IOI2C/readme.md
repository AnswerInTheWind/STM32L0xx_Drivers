###	STM32L05x IOI2C 驱动程序及测试例程 ###

此驱动是利用GPIO口进行模拟。
原理上就是操作GOIO 口按照I2C的时序进行变化。
在实际使用中由于移植方便，故经常用于开发周期短的项目

注意事项：
	
	1.在用GPIO口模拟I2C的时候，需要在芯片的GPIO口接上拉电阻，否则会出现时序不对的情况。
	2.另外，需要将用于模拟的GPIO口配置成开漏输出方式。