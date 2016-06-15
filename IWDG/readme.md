###IWDG_DRIVER

此程序是STM32L0xx的独立看门狗的驱动

共有两个API，如下：

	void IWDG_Init(uint16_t counter);//32.768KHZ 128分频 单次计数耗时约0.00390625s  
	
	void IWDG_Feed(void);


在初始化设置技术之之后，只需要在未达到计数值时喂狗就不会重启，否则MCU将重启。
更详细的配置参考源码，如有问题，欢迎联系。
