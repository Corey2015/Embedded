#include "lockctr.h"

void LOCK_CTR_init(void){
  GPIO_Init(LOCK_CTR_PORT, LOCK_CTR_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}

void LOCK_open(void){
 GPIO_WriteHigh(LOCK_CTR_PORT, LOCK_CTR_PIN);
}

void LOCK_close(void){
 GPIO_WriteLow(LOCK_CTR_PORT, LOCK_CTR_PIN);
}