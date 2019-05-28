#include "led.h"
#include "key.h"
#include "finger.h"
#include "uart.h"
#include "time.h"
#include "delay.h"
#include "power.h"  
#include "lockctr.h"

void Wake_handle(void);
void UART1_RX_Handler(void);

/*  global variable*/
uint8_t rxbuf[255];
uint16_t rxlen = 0;
uint16_t key_down_count = 0;
//按键长按
uint8_t key_down_long = 0;
//按键短按
uint8_t key_down_short = 0;
//管理员是否录入flag
uint8_t admin_enrolled = 0;
//录入状态
uint8_t enroll_flag = 0;
//管理员验证状态
uint8_t verify_flag=0;
uint8_t enroll_verify = 0;
uint8_t delete_verify = 0;

uint16_t i =0;

//BitStatus POWER_chrg_status;
//BitStatus POWER_stdby_status;


void main(){
  //内部高速时钟为主时钟 频率16M
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  LED_init();
  KEY_init();
  FINGER_init();
  UART_init();
  TIME_init();
  POWER_init();
  
  __enable_interrupt();
  //删除所有手指
  //FINGER_delete();

  while(1){
      /*
      LED_open(LED_RED);
      LED_close(LED_BLUE);
      delay_ms(1000);
      LED_close(LED_RED);
      LED_open(LED_BLUE);
      delay_ms(1000);
      */
    /*
    POWER_chrg_status = GPIO_ReadInputPin(POWER_GPIO_PORT, POWER_CHRG_GPIO_PIN);
    POWER_stdby_status = GPIO_ReadInputPin(POWER_GPIO_PORT, POWER_STDBY_GPIO_PIN);
    if (POWER_chrg_status) {
      LED_blue_on();
    }
    if(POWER_stdby_status){
      LED_red_on();
    }
    */
  }
}

//触摸中断处理函数
void Wake_handler(void){
  FINGER_open();
  delay_ms(300);
  //正在录入状态 不响应中断
  if (enroll_flag !=1 && verify_flag!=1){
    verify_flag =1;
    FINGER_verify();
    enroll_verify = 0;
    delete_verify=0;
    key_down_long = 0;
    key_down_short = 0;
    TIM1_Cmd(ENABLE);
  }

  
  
}

//指纹返回值处理函数
void FINGER_handler(uint8_t *buf,uint16_t length){
  uint16_t res;
  res = (buf[2]<<8) | buf[3];
  //指纹比对处理
  if (buf[1] == CMD_MATCH_FP){
    if(res == ERR_SUCCESS){
      if(delete_verify){
        //admin_verify_flag=0;
        //delete_verify=0;
        FINGER_delete();
        return;
      }else if(enroll_verify){
        //enroll_verify = 0;
        LED_blue_on();
        FINGER_enroll_first();
      }else{
        LOCK_open();
        TIM4_Cmd(ENABLE);
        verify_flag = 0;
        FINGER_close();
        LED_blue_on();
      }
      

    }else {
      verify_flag=0;
      enroll_flag=0;
      LED_red_on();
      FINGER_close();
      
   }
   return;
  }
  //指纹录入
  if(buf[1] == CMD_ENROLL){
    if(res == ERR_SUCCESS){
        verify_flag =0;
        enroll_flag = 0;
        FINGER_close();
        LED_blue_on();
      
      }else if ((res == ERR_ENROLL_CONTINUE )|| (res == ERR_NO_MOVE)){
        
        LED_blue_flash();
        //判断是否录入管理员
        if(admin_enrolled == 1){
          FINGER_enroll_again();  
        }else{
          FINGER_enroll_admin_again();
        }

      }else if (res == ERR_FP_FULL || res == ERR_TIMEOUT || res == ERR_ENROLL_FAIL){
        verify_flag=0;
        enroll_flag = 0;
        LED_red_on();
        FINGER_close();
      }
    return;
  }
  
  //删除手指
  if(buf[1] == CMD_DELETE_FP){
    //FINGER_close();
    
    if(res == ERR_SUCCESS){
      LED_blue_on();
    }else{
      LED_red_on();
    }
    verify_flag=0;
    FINGER_close();
    return;
  }
  
  //指纹查询
  if(buf[1] == CMD_SYS_CONFIG){
    if(res == ERR_SUCCESS){
      enroll_flag = 1;
      if(buf[8] == 1){
        //已录入管理员
        LED_flash();
        admin_enrolled = 1;
        enroll_verify = 1;
        FINGER_verify_admin();
        //FINGER_enroll_first();

        return;
      }else{
        //未录入管理员
        LED_blue_on();
        admin_enrolled = 0;
        FINGER_enroll_admin_first();

      }
    }
  }

}

//串口中断处理函数
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

//时钟中断函数
void TIME_handler(void){
  key_down_count++;
  //判断手指是否离开
  if(isKEYdown()){
    if (key_down_count >= 30 &&key_down_count <=60){
      key_down_short = 1;
      key_down_long = 0;
      LED_close(LED_RED);
      LED_open(LED_BLUE);
      //key_down = 0;
    }else if(key_down_count >=60){
      key_down_short = 0;
      key_down_long = 1;
      LED_open(LED_RED);
      LED_close(LED_BLUE);
    }
    
  }else{
    TIM1_Cmd(DISABLE);
    key_down_count =0;
    if(key_down_short){
      FINGER_open();
      delay_ms(300);
      FINGER_getadmin();
      
    }
    if(key_down_long){
      FINGER_open();
      delay_ms(300);
      delete_verify = 1;
      verify_flag=1;
      FINGER_verify_admin();
      
    }
    LED_close(LED_BLUE);
    LED_close(LED_RED);

  }
}

void CHARGE_handler(void){
  u8 charge_status ;
  charge_status = get_CHARGE_status();
  if (charge_status == CHARGE_ON){
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_Cmd(ENABLE);
  }
}

void TIM2_handler(void){
  u8 charge_status ;
  charge_status = get_CHARGE_status();
  if (charge_status == CHARGE_ON){
    LED_red_on();
  }else if(charge_status == CHARGE_FULL){
    LED_blue_on();
  }else if (charge_status ==CHARGE_OFF){
    LED_close(LED_BLUE);
    LED_close(LED_RED);
    TIM2_Cmd(DISABLE);

  }
}

void TIM4_handler(void){
  i++;
  if(i >1000){
    i = 0;
    LOCK_close();
    TIM4_Cmd(DISABLE);

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