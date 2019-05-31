#include "led.h"
#include "delay.h"

void LED_red_open(void);
void LED_blue_open(void);
void LED_red_close(void);
void LED_blue_close(void);

void LED_init(){
  GPIO_Init(LED_GPIO_PORT, LED_RED_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_GPIO_PORT, LED_BLUE_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}
void LED_open(u8 led){
  switch(led){
    case LED_RED:
      LED_red_open();
      break;
  case LED_BLUE:
      LED_blue_open();
      break;
  }
}

void LED_close(u8 led){
  switch(led){
    case LED_RED:
      LED_red_close();
      break;
  case LED_BLUE:
      LED_blue_close();
      break;
  }
}

void LED_red_open(void){
  GPIO_WriteHigh(LED_GPIO_PORT, LED_RED_GPIO_PIN);
}

void LED_blue_open(void){
  GPIO_WriteHigh(LED_GPIO_PORT, LED_BLUE_GPIO_PIN);
}

void LED_red_close(void){
  GPIO_WriteLow(LED_GPIO_PORT, LED_RED_GPIO_PIN);
}

void LED_blue_close(void){
  GPIO_WriteLow(LED_GPIO_PORT, LED_BLUE_GPIO_PIN);
}

void LED_blue_on(void){
  LED_blue_open();
  delay_ms(200);
  LED_blue_close();
}

void LED_red_on(void){
  LED_red_open();
  delay_ms(200);
  LED_red_close();
}

void LED_reverse(void){
  GPIO_WriteReverse(LED_GPIO_PORT,LED_RED_GPIO_PIN);  
  GPIO_WriteReverse(LED_GPIO_PORT,LED_BLUE_GPIO_PIN);
}

void LED_flash(void){
  u8 i =3;
  while(i){
    LED_blue_open();
    LED_red_close();
    delay_ms(50);
    LED_red_open();
    LED_blue_close();
    delay_ms(50);
    i--;
  }
  LED_red_close();
  LED_blue_close();
  
}

void LED_blue_flash(void){
  u8 i =3;
  while(i){
    LED_blue_open();
    delay_ms(50);
    LED_blue_close();
    delay_ms(50);
    i--;
  }
  
}