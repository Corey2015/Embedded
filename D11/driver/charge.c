#include "charge.h"
#include "led.h"

void CHARGE_init(void){
  GPIO_Init(CHARGE_GPIO_PORT, CHARGE_CHRG_GPIO_PIN, GPIO_MODE_IN_PU_IT  );
  //GPIO_Init(CHARGE_GPIO_PORT, CHARGE_STDBY_GPIO_PIN, GPIO_MODE_IN_PU_IT  );
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB,EXTI_SENSITIVITY_FALL_ONLY);
}

u8 get_CHARGE_status(void){
  
  BitStatus  chrg_status,stdby_status;
  chrg_status = GPIO_ReadInputPin(CHARGE_GPIO_PORT, CHARGE_CHRG_GPIO_PIN);
  stdby_status = GPIO_ReadInputPin(CHARGE_GPIO_PORT,CHARGE_STDBY_GPIO_PIN);
  if (chrg_status == RESET && stdby_status != RESET ){
    return CHARGE_ON;
  } else if(chrg_status != RESET && stdby_status ==RESET){
    return CHARGE_FULL;
  }else{
    return CHARGE_OFF;
  }
  
}