/*
 * uart.h
 *
 *  Created on: 10 nov. 2024
 *      Author: admin
 */
#ifndef UART_H_
#define UART_H_
#include "avr/io.h"
char u8len;
uint8_t u8RxBuffer[255];
void uart_init();
void uart_send_char(uint8_t data);
uint8_t uart_get_char();
void flashing_vidRxNotification(uint8_t* pu8Data , uint8_t u8len);
#endif /* UART_H_ */

