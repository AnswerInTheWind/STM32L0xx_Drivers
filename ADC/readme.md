###STM32L0xx ADC 驱动

此驱动程序一共包含两个API

	void adc_init(void);//ADC初始化，包括IO口初始化和ADC的配置

	uint32_t adc_getResult(void);//读取A/D结果，以uint32_t 结果返回，量程为 0-3300mV，单位为mv

注意：

	此例程使用的是ADC1，所用的GPIO口为PA0，即外部电压信号需接到PA0脚上才能测量。