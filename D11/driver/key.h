#ifndef __KEY_H
#define __KEY_H

/*includes*/
#include "stm8s.h"

/*defines*/
#define KEY_GPIO_PORT  (GPIOC)
#define KEY_GPIO_PIN  (GPIO_PIN_7)

/*functions*/
void KEY_init(void);
BitStatus isKEYdown(void);
#endif