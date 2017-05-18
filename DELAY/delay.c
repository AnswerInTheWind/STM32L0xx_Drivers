//使用 Sys 系统时钟来进行演示，不需要区分系统时钟是72M还是32M之类的。
//函数的单位是us。
void Delay_us(uint32_t Delay) 
{ 
  uint32_t tickstart = 0; 
  uint32_t tickNum = 0; 
  uint32_t tickMax = SysTick->LOAD;

  uint32_t delay_usvalue = (SysTick->LOAD / 1000) * Delay;

   tickstart = SysTick->VAL;

  while(1)
  {
   uint32_t cur_tick = SysTick->VAL;
      if (cur_tick > tickstart)  
      {
          tickNum = tickstart + (tickMax - cur_tick);
      }
      else
      {
          tickNum = tickstart - cur_tick;
      }

      if (tickNum > delay_usvalue)
      {
          break;
      }
  }
}