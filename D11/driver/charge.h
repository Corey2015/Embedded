#ifndef __CHARGE_H
#define __CHARGE_H

/*includes*/
#include "stm8s.h"

/*defines*/
#define CHARGE_GPIO_PORT      (GPIOB)
#define CHARGE_CHRG_GPIO_PIN   (GPIO_PIN_5)
#define CHARGE_STDBY_GPIO_PIN  (GPIO_PIN_4)
enum{
    CHARGE_OFF  = 0x00,
    CHARGE_ON   = 0x01,
    CHARGE_FULL = 0X02,
};

/*functions*/
void CHARGE_init(void);
u8 get_CHARGE_status(void);

#endif