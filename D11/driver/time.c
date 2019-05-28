#include "time.h"

void TIME_init(void){
  TIM1_DeInit();
  //16��Ƶ ��ʱ��16M��ʱ�� 1MHZ/s Ƶ�� ���ϼ�����������ɺ󴥷��жϣ�1/1000 = 1ms �Զ���װ��ֵΪ0
  //TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 1000, 0x00);
  //�޸ĳ�100ms����һ���ж�
  TIM1_TimeBaseInit(160, TIM1_COUNTERMODE_UP, 10000, 0x00);
  //�����Զ���װ��
  TIM1_ARRPreloadConfig(ENABLE);
  //���ݸ����ж�
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
  //TIM1_Cmd(ENABLE);
  
  //time2
  TIM2_DeInit();
  //100���봥��һ��
  TIM2_TimeBaseInit(TIM2_PRESCALER_256, 6250);
  //�����ж�
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  //
  TIM2_SetCounter(0x0000);
  
  //tim4
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_128,0x7d);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  
}