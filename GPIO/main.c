/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#define LED_GPIO_PORT  (GPIOD)
#define LED_RED_GPIO_PINS  (GPIO_PIN_2)
#define LED_BLUE_GPIO_PINS  (GPIO_PIN_3)
#define WAKE_GPIO_PORT    (GPIOC)
#define WAKE_GPIO_PIN    (GPIO_PIN_7)
void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {   
    nCount--;
  }
}
void main(void)
{
  

  /* Initialize I/Os in Output Mode */
  GPIO_Init(LED_GPIO_PORT, LED_RED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_GPIO_PORT, LED_BLUE_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(WAKE_GPIO_PORT, WAKE_GPIO_PIN, GPIO_MODE_IN_FL_IT );

  while (1)
  {
    BitStatus wake = SET;
    /* Toggles LEDs */
    //GPIO_WriteReverse(LED_GPIO_PORT, LED_RED_GPIO_PINS);
    //Delay(0xffff);
    wake = GPIO_ReadInputPin(WAKE_GPIO_PORT, WAKE_GPIO_PIN);
    if(RESET == wake){
      GPIO_WriteLow(LED_GPIO_PORT, LED_BLUE_GPIO_PINS);
      //Delay(0xffff);
    }else{
      GPIO_WriteHigh(LED_GPIO_PORT, LED_BLUE_GPIO_PINS);
      //Delay(0xffff);
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
