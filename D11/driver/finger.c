#include "finger.h"
uint8_t cmd_abort[] = {0x3a,0x00,0x00,0x00,0x00,0x00};
uint8_t cmd_fingernum[] = {0x3a,0x01,0x00,0x20,0x00,0x04,0x00,0x00,0x00,0x00,0x63};
uint8_t cmd_verify[] = {0x3a,0x04,0xa6,0xe0,0x00,0x04,0x00,0x00,0x00,0x00,0x63};
uint8_t cmd_enroll_first[] = {0x3a,0x13,0xb6,0xe0,0x00,0x06,0x00,0x00,0x00,0x00,0x63,0x06,0x06};
uint8_t cmd_enroll_again[] = {0x3a,0x13,0xb6,0x60,0x00,0x06,0x00,0x00,0x00,0x00,0x63,0x06,0x06};

void FINGER_init(void){
  GPIO_Init(FINGER_GPIO_PORT, FINGER_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}
void FINGER_on(void){
  GPIO_WriteHigh(FINGER_GPIO_PORT, FINGER_GPIO_PIN);
}
void FINGER_off(void){
  GPIO_WriteLow(FINGER_GPIO_PORT, FINGER_GPIO_PIN);
}

void cmd_pack(uint8_t buf[]){
  uint8_t *comm; 
  uint8_t i,tmp,SUM,XOR,len;
  SUM = 0;
  XOR = 0;
  len = buf[5];
  comm = buf;
  for(i=0;i<6;i++){
    comm[0] = 0x3a; 
    SUM +=comm[i];
    XOR ^=comm[i];
  }
  comm[i] = XOR;
  tmp = len+7;
  for(;i<tmp;i++){
    SUM += comm[i];
  }
  comm[i] = ~SUM;
  UART_send(comm,tmp+1);
}

void FINGER_abort(void){
  cmd_pack(cmd_abort);
  /*
  uint8_t *comm; 
  uint8_t i,tmp,SUM,XOR;
  SUM = 0;
  XOR = 0;
  uint16_t len = cmd_abort[4];
  comm = (uint8_t *)cmd_abort;
  for(i=0;i<6;i++){
    SUM +=comm[i];
    XOR ^=comm[i];
  }
  comm[i] = XOR;
  tmp = len+7;
  for(;i<tmp;i++){
    SUM += comm[i];
  }
  comm[i] = ~SUM;
  UART_send(comm,tmp+1);
  */
}
void FINGRT_getnum(void){
  cmd_pack(cmd_fingernum);
}
void FINGRT_verify(void){
  cmd_pack(cmd_verify);
}

void FINGRT_enroll_first(void){
  cmd_pack(cmd_enroll_first);
}

void FINGRT_enroll_again(void){
  cmd_pack(cmd_enroll_again);
}