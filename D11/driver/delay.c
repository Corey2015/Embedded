#include "delay.h"



void delay_us(uint16_t nCount)
{
    nCount *=3.2; 
    while(--nCount);
}

void delay_ms(uint16_t nCount)
{
    while(nCount--)
    {
        delay_us(1000);
    }
}