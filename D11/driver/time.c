#include "time.h"

void TIME_init(void){
  TIM1_DeInit();
  //16分频 主时钟16M的时候 1MHZ/s 频率 向上计数，计数完成后触发中断，1/1000 = 1ms 自动重装载值为0
  //TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 1000, 0x00);
  //修改成100ms触发一次中断
  TIM1_TimeBaseInit(160, TIM1_COUNTERMODE_UP, 10000, 0x00);
  //允许自动重装载
  TIM1_ARRPreloadConfig(ENABLE);
  //数据更新中断
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
  //TIM1_Cmd(ENABLE);
  
  //time2
  TIM2_DeInit();
  //100毫秒触发一次
  TIM2_TimeBaseInit(TIM2_PRESCALER_256, 6250);
  //配置中断
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  //
  TIM2_SetCounter(0x0000);
  
  //tim4
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_128,0x7d);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  
}