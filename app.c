/*
 * app.c
 *
 *  Created on: 11 nov. 2024
 *      Author: admin
 */

#include "avr/eeprom.h"
#include "flashing_manager.h"
#include "uart.h"
#include "avr/io.h"
#include "util/delay.h"
#define F_CPU 16000000UL

char eedata[] EEMEM = {0x00,0x00};

int main(){
	uint8_t valid_app_val , reqFromApp ;
	valid_app_val = eeprom_read_byte(VALID_APP_ADDRESS);
	reqFromApp = eeprom_read_byte(REQ_FROM_APP_ADDRESS);

	if((valid_app_val != 1) | (reqFromApp == 1)){
		DDRB = 0xFF;
		PORTB = (1<<PB0);
		uart_init();
		if(reqFromApp == 1){
			falshing_mang_from_app();
			eeprom_update_byte(REQ_FROM_APP_ADDRESS,0);
		}
		while(1){
			uart_Handler();
			flashingMngr_vidMainTask();
		}
	}else{
		asm("jmp 0");
		PORTB = PORTB|(1<<PORTB2);
	}
	return 0;
}
