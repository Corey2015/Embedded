#include "uart.h"

void UART_init(void){
  UART1_DeInit();
  UART1_Init((u32)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  //÷–∂œ…Ë÷√
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  UART1_Cmd(ENABLE);
  
}

void UART_send_byte(uint8_t data){
  UART1_SendData8(data);
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}

void UART_send(uint8_t *buf,uint16_t length){
  uint16_t i = 0;
  for(;i<length;i++){
    UART_send_byte(buf[i]);
  }
}