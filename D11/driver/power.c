#include "power.h"
#include "led.h"

void POWER_init(void){
  //GPIO_Init(POWER_GPIO_PORT, POWER_CHRG_GPIO_PIN, GPIO_MODE_IN_PU_IT  );
  // GPIO_Init(POWER_GPIO_PORT, POWER_STDBY_GPIO_PIN, GPIO_MODE_IN_PU_IT  );
  //EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB,EXTI_SENSITIVITY_FALL_ONLY);
}

u8 get_CHARGE_status(void){
  BitStatus  POWER_chrg_status,POWER_stdby_status;
  POWER_chrg_status = GPIO_ReadInputPin(POWER_GPIO_PORT, POWER_CHRG_GPIO_PIN);
  //POWER_stdby_status = GPIO_ReadInputPin(POWER_GPIO_PORT, POWER_STDBY_GPIO_PIN);
  if (POWER_chrg_status == RESET ){
    return CHARGE_ON;
  } else if(POWER_chrg_status == SET ){
    return CHARGE_OFF;
  } 
  
}