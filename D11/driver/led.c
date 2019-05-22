#include "led.h"
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
  case LED_BLUE:
      LED_blue_open();
  }
}

void LED_close(u8 led){
  switch(led){
    case LED_RED:
      LED_red_close();
  case LED_BLUE:
      LED_blue_close();
  }
}

void LED_red_open(){
  GPIO_WriteHigh(LED_GPIO_PORT, LED_RED_GPIO_PIN);
}

void LED_blue_open(){
  GPIO_WriteHigh(LED_GPIO_PORT, LED_BLUE_GPIO_PIN);
}

void LED_red_close(){
  GPIO_WriteLow(LED_GPIO_PORT, LED_RED_GPIO_PIN);
}

void LED_blue_close(){
  GPIO_WriteLow(LED_GPIO_PORT, LED_BLUE_GPIO_PIN);
}