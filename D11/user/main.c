#include "led.h"
#include "key.h"
#include "finger.h"
#include "uart.h"

void interrupt_handle(void);
void UART1_RX_Handler(void);
uint8_t rxbuf[255];
uint16_t rxlen = 0;
void delay_us(uint16_t nCount)
{
    nCount *=3.2; 
    while(--nCount);
}
/**
  * @brief _delay_ms
  * @param nCount
  * @retval None
  */
void delay_ms(uint16_t nCount)
{
    while(nCount--)
    {
        delay_us(1000);
    }
}

void main(){
  //内部高速时钟为主时钟 频率16M
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  LED_init();
  KEY_init();
  FINGER_init();
  UART_init();
  __enable_interrupt();
  FINGER_on();
  delay_ms(300);
  //FINGRT_enroll_first();
  //FINGER_abort();
  
  while(1){
      /*
      LED_open(LED_RED);
      LED_close(LED_BLUE);
      delay_ms(1000);
      LED_close(LED_RED);
      LED_open(LED_BLUE);
      delay_ms(1000);
      */


  }
}

void interrupt_handle(void){
  FINGER_on();
  delay_ms(300);
  FINGRT_verify();
  
}

void FINGER_handler(uint8_t *buf,uint16_t length){
  uint16_t res;
  uint8_t i=3;
  res = (buf[2]<<8) | buf[3];
  switch (buf[1]){
    //指纹比对
    case CMD_MATCH_FP:
      if(res == ERR_SUCCESS){
        while(i){
          LED_close(LED_RED);
          LED_open(LED_BLUE);
          delay_ms(200);
          LED_close(LED_BLUE);
          delay_ms(200);
          i--;
        }
      }else {
        while(i){
          LED_open(LED_RED);
          LED_close(LED_BLUE);
          delay_ms(200);
          LED_close(LED_RED);
          delay_ms(200);
          i--;
        }
     }
    case CMD_ENROLL:
      if(res == ERR_SUCCESS){
        while(i){
          LED_open(LED_RED);
          LED_close(LED_BLUE);
          delay_ms(200);
          LED_close(LED_RED);
          LED_open(LED_BLUE);
          delay_ms(200);
          i--;
        }
      }else if ((res == ERR_ENROLL_CONTINUE )|| (res == ERR_NO_MOVE)){
        while(i){
          LED_close(LED_RED);
          LED_open(LED_BLUE);
          delay_ms(200);
          LED_close(LED_BLUE);
          delay_ms(200);
          i--;
        }
        FINGRT_enroll_again();
        
       }
   }
  //FINGER_off();
 }


void UART1_RX_Handler(void){
    static uint8_t i = 0;
    UART1_ClearITPendingBit(UART1_IT_RXNE);
    if (!UART1_GetFlagStatus(UART1_FLAG_RXNE)){
      rxbuf[i++] = UART1_ReceiveData8();
      if(rxbuf[0] != 0x3a){
        i = 0;
      }
      if(i == 6){
        rxlen = (rxbuf[4]<<8) | rxbuf[5];
      }
      if (i>6 &&(i == rxlen+8) ){
        i = 0;
        FINGER_handler(rxbuf,i);

      }
    }
    
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif