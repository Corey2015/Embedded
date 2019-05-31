#include "led.h"
#include "key.h"
#include "finger.h"
#include "uart.h"
#include "time.h"
#include "delay.h"  
#include "lockctr.h"
#include "charge.h"

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
//uart 正在处理
uint8_t uart_flag =0;

uint8_t enroll_verify = 0;
uint8_t delete_verify = 0;

uint16_t i =0;

//BitStatus POWER_chrg_status;
//BitStatus POWER_stdby_status;

void system_init(){
    //内部高速时钟为主时钟 频率16M
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  LED_init();
  KEY_init();
  FINGER_init();
  UART_init();
  TIME_init();
  FINGER_open();
  delay_ms(250);
  FINGER_set_finger_on_time();
  delay_ms(50);
  CHARGE_init();
  //UART1_Cmd(DISABLE);
  __enable_interrupt();
  FINGER_getadmin();
  //FINGER_timeout();
  u8 charge_status=get_CHARGE_status();
  if(charge_status == CHARGE_ON ||charge_status == CHARGE_FULL){
    TIM2_Cmd(ENABLE);
  }
  //FINGER_close();
  //删除所有手指
  //FINGER_delete();
}

void main(){
  
  system_init();
  
  while(1){
      /*
      LED_open(LED_RED);
      LED_close(LED_BLUE);
      delay_ms(10000);
      LED_close(LED_RED);
       LED_close(LED_BLUE);
      delay_ms(10000);
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

  TIM2_Cmd(DISABLE);
  LED_close(LED_BLUE);
  LED_close(LED_RED);
   
  //正在录入状态 不响应中断
  if (enroll_flag !=1 && verify_flag!=1){
    FINGER_open();
    delay_ms(250);
    GPIOC->CR2 &= ~(1<<7);
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
void FINGER_handler2(uint8_t *buf,uint16_t length){
  uint16_t res;
  res = (buf[2]<<8) | buf[3];
  switch (buf[1]){
    //指纹比对
    case CMD_MATCH_FP:
      if(res == ERR_SUCCESS){
        if(delete_verify){
          //admin_verify_flag=0;
          //delete_verify=0;
          FINGER_delete();
          GPIOC->CR2 |= (1<<7);
          break;
        }else if(enroll_verify){
          //enroll_verify = 0;
          enroll_flag=1;
          LED_blue_on();
          FINGER_enroll_first();
        }else{
          verify_flag = 0;
          LOCK_open();
          TIM4_Cmd(ENABLE);
          TIM2_Cmd(ENABLE);
          FINGER_close();
          LED_blue_on();
        }
        

      }else {
        if(admin_enrolled == 0){
          LOCK_open();
          LED_blue_on();
        }else{
          LED_red_on();
        }
        verify_flag=0;
        enroll_flag=0;
        TIM2_Cmd(ENABLE);
        FINGER_close();
        
     }
     GPIOC->CR2 |= (1<<7);
     break;
    //指纹删除
    case CMD_DELETE_FP:
      if(res == ERR_SUCCESS){
        LED_blue_on();
        admin_enrolled=0;
      }else{
        LED_red_on();
      }
      verify_flag=0;
      FINGER_close();
      TIM2_Cmd(ENABLE);
      GPIOC->CR2 |= (1<<7);
      break;  
      
      
    case CMD_ENROLL:
      if(res == ERR_SUCCESS){
        if(admin_enrolled ==0){
          admin_enrolled=1;
        }
        verify_flag =0;
        enroll_flag = 0;
        TIM2_Cmd(ENABLE);
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
          TIM2_Cmd(ENABLE);
        }
      GPIOC->CR2 |= (1<<7);
      break;
      
    //指纹查询
    case CMD_SYS_CONFIG:
      if(res == ERR_SUCCESS){
        //enroll_flag = 1;
        if(buf[8] == 1){
          //已录入管理员
          //LED_flash();
          admin_enrolled = 1;
          //enroll_verify = 1;
          //FINGER_verify_admin();
          //FINGER_enroll_first();
          GPIOC->CR2 |= (1<<7);
          break;
        }else{
          //未录入管理员
          //LED_blue_on();
          admin_enrolled = 0;
          //FINGER_enroll_admin_first();

        }
      }
      FINGER_close();
      GPIOC->CR2 |= (1<<7);
      break;
      
    default:
      GPIOC->CR2 |= (1<<7);
      break;
      
  }

  uart_flag=0;
}

/*

//指纹返回值处理函数
void FINGER_handler(uint8_t *buf,uint16_t length){
  uint16_t res;
  res = (buf[2]<<8) | buf[3];
  if(buf[1] == CMD_ABORT){
    return;
  }
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
        enroll_flag=1;
        LED_blue_on();
        FINGER_enroll_first();
      }else{
        verify_flag = 0;
        LOCK_open();
        TIM4_Cmd(ENABLE);
        TIM2_Cmd(ENABLE);
        FINGER_close();
        LED_blue_on();
      }
      

    }else {
      if(admin_enrolled == 0){
        LOCK_open();
        LED_blue_on();
      }else{
        LED_red_on();
      }
      verify_flag=0;
      enroll_flag=0;
      TIM2_Cmd(ENABLE);
      FINGER_close();
      
   }
   return;
  }
  //指纹录入
  if(buf[1] == CMD_ENROLL){
    if(res == ERR_SUCCESS){
      if(admin_enrolled ==0){
        admin_enrolled=1;
      }
      verify_flag =0;
      enroll_flag = 0;
      TIM2_Cmd(ENABLE);
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
        TIM2_Cmd(ENABLE);
      }
    return;
  }
  
  //删除手指
  if(buf[1] == CMD_DELETE_FP){
    //FINGER_close();
    
    if(res == ERR_SUCCESS){
      LED_blue_on();
      admin_enrolled=0;
    }else{
      LED_red_on();
    }
    verify_flag=0;
    FINGER_close();
    TIM2_Cmd(ENABLE);
    return;
  }
  
  //指纹查询
  if(buf[1] == CMD_SYS_CONFIG){
    if(res == ERR_SUCCESS){
      //enroll_flag = 1;
      if(buf[8] == 1){
        //已录入管理员
        //LED_flash();
        admin_enrolled = 1;
        //enroll_verify = 1;
        //FINGER_verify_admin();
        //FINGER_enroll_first();

        return;
      }else{
        //未录入管理员
        //LED_blue_on();
        admin_enrolled = 0;
        //FINGER_enroll_admin_first();

      }
    }
    FINGER_close();
  }

}
*/

//串口中断处理函数
void UART1_RX_Handler(void){
    UART1_ClearITPendingBit(UART1_IT_RXNE);
    static uint8_t i = 0;
    if (!UART1_GetFlagStatus(UART1_FLAG_RXNE)){
      rxbuf[i++] = UART1_ReceiveData8();
      if(rxbuf[0] != 0x3a){
        i = 0;
      }
      if(rxbuf[1] == 0x3a){
        i = 1;
      }
      if(i == 6){
        rxlen = (rxbuf[4]<<8) | rxbuf[5];
      }
      if (i>6 &&(i == rxlen+8) ){
        i = 0;
        if(uart_flag == 0){
          uart_flag=1;
          FINGER_handler2(rxbuf,i);
        }
      }
    }

    
}

//时钟中断函数 处理长按 短按
void TIME_handler(void){
  key_down_count++;
  //判断手指是否离开
  
  if(isKEYdown()){
    TIM2_Cmd(DISABLE);
    if (key_down_count >= 60 && key_down_count <=120){
      if(verify_flag ==1){
        FINGER_abort();
        verify_flag= 0;
      }
      key_down_short = 1;
      key_down_long = 0;
      LED_close(LED_RED);
      LED_open(LED_BLUE);
      //key_down = 0;
    }else if(key_down_count >120){
      
      key_down_short = 0;
      key_down_long = 1;
      LED_open(LED_RED);
      LED_close(LED_BLUE);
    }else{
      

      //enroll_flag=0;
      //verify_flag=0;
    }
    
  }else{
    TIM1_Cmd(DISABLE);
    //enroll_flag=0;
    //verify_flag=0;
    key_down_count =0;
    if(key_down_short){
      if(verify_flag){
        FINGER_abort();
        verify_flag= 0;
        //delay_ms(100);
      }
      FINGER_open();
      delay_ms(250);
      //FINGER_getadmin();
      if(admin_enrolled){
        LED_flash();
        enroll_verify = 1;
        verify_flag=1;
        FINGER_verify_admin();
      }else{
        LED_blue_on();
        enroll_flag=1;
        FINGER_enroll_admin_first();
      }
      key_down_short = 0;
    }
    if(key_down_long){

      FINGER_open();
      delay_ms(250);
      delete_verify = 1;
      verify_flag=1;
      FINGER_verify_admin();
      key_down_long = 0;
    }
    LED_close(LED_BLUE);
    LED_close(LED_RED);

  }
}

//充电中断
void CHARGE_handler(void){
  TIM2_Cmd(ENABLE);
}

//充电计时函数
void TIM2_handler(void){
  static uint16_t i = 0;
  i++;
  if (i == 1000){
    i=0;
    u8 charge_status = 0;
    charge_status=get_CHARGE_status();
    if(charge_status == CHARGE_ON){
      LED_reverse();
    }else if (charge_status == CHARGE_FULL){
      LED_open(LED_BLUE);
      LED_close(LED_RED);
      
    }else{
      LED_close(LED_RED);
      LED_close(LED_BLUE);
      TIM2_Cmd(DISABLE);
    }
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