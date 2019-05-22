#ifndef __UART_H
#define __UART_H

/*includes*/
#include "stm8s.h"

/*defines*/


/*functions*/
void UART_init(void);
void UART_send_byte(uint8_t data);
void UART_send(uint8_t *buf,uint16_t length);

#endif