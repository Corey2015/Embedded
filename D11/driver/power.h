#ifndef __POWER_H
#define __POWER_H

/*includes*/
#include "stm8s.h"

/*defines*/
#define POWER_GPIO_PORT      (GPIOB)
#define POWER_CHRG_GPIO_PIN   (GPIO_PIN_5)
#define POWER_STDBY_GPIO_PIN  (GPIO_PIN_4)
enum{
    CHARGE_OFF  = 0x00,
    CHARGE_ON   = 0x01,
    CHARGE_FULL = 0X02,
};

/*functions*/
void POWER_init(void);
u8 get_CHARGE_status(void);

#endif