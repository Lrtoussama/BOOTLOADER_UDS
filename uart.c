/*
 * uart.c
 *
 *  Created on: 10 nov. 2024
 *      Author: admin
 */




#include "uart.h"

#define IDLE 0
#define RUNNING 1

void uart_init(){
    UBRR0H = (103>>8);
    UBRR0L = (103);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}
void uart_send_char(uint8_t data){
  while (!(UCSR0A & (1<<UDRE0)));

    /* code */
    UDR0 = data;


}
uint8_t uart_get_char(){
  while (!(UCSR0A & (1<<RXC0)));
    return UDR0;


}
void uart_Handler(void) {
  static uint8_t RxState = IDLE;
  static uint8_t bufferIdx = 0;
  DDRD = 0xFF;  // Configure PORTD as output

  if (UCSR0A & (1 << RXC0)) {  // Check if data is received
    if (RxState == IDLE) {
      u8len = UDR0;
      // Read length
      RxState = RUNNING;

    } else {
      u8RxBuffer[bufferIdx] = UDR0;  // Store received byte
      bufferIdx++;

      if (bufferIdx == u8len) {      // Check if message is complete
           // Indicate end of message
        bufferIdx = 0;
        RxState = IDLE;
        flashing_vidRxNotification(u8RxBuffer, u8len);  // Notify message received
      }
    }
  } else {
    PORTD |= (1 << PORTD4);  // No data received

  }


}

