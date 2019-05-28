#ifndef __LOCKCTR_H
#define __LOCKCTR_H

/*includes*/
#include "stm8s.h"

/*defines*/
#define LOCK_CTR_PORT  (GPIOC)
#define LOCK_CTR_PIN  (GPIO_PIN_4)

/*functions*/
void LOCK_CTR_init(void);
void LOCK_open(void);
void LOCK_close(void);
#endif