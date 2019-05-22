#ifndef __LED_H
#define __LED_H

/*includes*/
#include "stm8s.h"

/*defines*/
#define LED_GPIO_PORT      (GPIOD)
#define LED_RED_GPIO_PIN   (GPIO_PIN_2)
#define LED_BLUE_GPIO_PIN  (GPIO_PIN_3)
#define LED_RED            (1)
#define LED_BLUE           (2)

/*functions*/
void LED_init(void);

void LED_open(u8 led);
void LED_close(u8 led);
#endif