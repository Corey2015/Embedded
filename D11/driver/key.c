#include "key.h"

void KEY_init(void){
  GPIO_Init(KEY_GPIO_PORT, KEY_GPIO_PIN, GPIO_MODE_IN_FL_IT );
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC,EXTI_SENSITIVITY_RISE_ONLY);
}

BitStatus isKEYdown(void){
  return GPIO_ReadInputPin(KEY_GPIO_PORT, KEY_GPIO_PIN);
}