/*
 * flashing_manager.c
 *
 *  Created on: 11 nov. 2024
 *      Author: admin
 */





#include "flashing_manager.h"
//#include "uart.h"
#include "avr/boot.h"
#include "avr/delay.h"
#include "util/crc16.h"
#include "avr/pgmspace.h"
#define TRUE ((uint8_t)0x01)
#define FALSE ((uint8_t)0x00)

#define SESSION_CONTROL (0x10)
#define PROGRAMMING_SESSION (0X03)
#define DOWNLOAD_REQUEST (0X34)
#define TRANSFER_DATA (0X36)
#define TRANSFER_EXIT (0X37)
#define CHECK_CRC (0X31)
#define MAX_CODE_SIZE (0X3800)
#define PAGE_SIZE (128)


typedef enum {
	waiting_ProgrammingSession ,
	waiting_DowmloadRequest ,
	waiting_TransferData ,
	waiting_TransferExit,
	waiting_ChekCrc

}DownloadState;

static void LOC_vidSendNegResp(void);
static void LOC_vidSendPosResp(void);
void falshing_mang_from_app(void);
void boot_program_page (uint16_t page, uint8_t *buff);
static uint8_t LOC_vidCheckFlashCRC(uint16_t u16StartAdd, uint16_t u16EndAdd, uint16_t u16CRC);
static uint8_t bIsRequestReceived = FALSE  ;
static uint8_t u8SID;
static uint8_t u8ReqLen;
static uint8_t *pu8ReqData ;
static DownloadState enuDownloadState;
extern void uart_init();
extern void uart_send_char(uint8_t data);
extern uint8_t uart_get_char();
extern void flashing_vidRxNotification(uint8_t* pu8Data , uint8_t u8len);

void boot_program_page (uint16_t page, uint8_t *buff){
	uint16_t i;
	uint8_t sreg;
	uint32_t adress;
	uint16_t word;
	sreg = SREG ;
	asm("cli");
	adress = page * SPM_PAGESIZE;
	boot_page_erase_safe(adress);

	for(i=0 ; i < SPM_PAGESIZE ;i+=2 ){
		word = *buff++;
		word += (*buff++)<<8;
		boot_page_fill_safe(adress+i , word);
	}
	boot_page_write_safe(adress);
	boot_rww_enable_safe();;
	sreg = SREG ;

};
void falshing_mang_from_app(){
	u8SID = 0x10;
	LOC_vidSendPosResp();
	enuDownloadState = waiting_DowmloadRequest;
}
void flashingMngr_vidMainTask() {
	DDRB = 0xFF;                 // Configure tous les bits de PORTD comme sortie
	static uint16_t u16codesize , u16reicevedlen = 0;
	static uint8_t u8PageNo = 0 ;
	uint8_t bValidReq = FALSE , bValidCRC;
	uint16_t u16ReceivedCRC;
	if(bIsRequestReceived){
		switch (u8SID) {
		    case SESSION_CONTROL:
		        if (pu8ReqData[0] == PROGRAMMING_SESSION && (u8ReqLen == 2) && (enuDownloadState == waiting_ProgrammingSession)) {
		            LOC_vidSendPosResp();
		            enuDownloadState = waiting_DowmloadRequest;
		        } else {
		            LOC_vidSendNegResp();
		            enuDownloadState == waiting_ProgrammingSession;
		        }

		        break;
		    case DOWNLOAD_REQUEST:
		        if ((enuDownloadState == waiting_DowmloadRequest) && (u8ReqLen == 3)) {
		            u16codesize = (pu8ReqData[0] << 8) | (pu8ReqData[1]);
		            if (pu8ReqData[1] < MAX_CODE_SIZE) {
		                LOC_vidSendPosResp();
		                enuDownloadState = waiting_TransferData;
		                bValidReq = TRUE;
		            }
		        }
		        if (bValidReq != TRUE) {
		            LOC_vidSendNegResp();
		            enuDownloadState == waiting_ProgrammingSession;
		        }
		        break;

		    case TRANSFER_DATA:
		    {
		        if ((enuDownloadState == waiting_TransferData) && (u8ReqLen == PAGE_SIZE +1)) {
		        	PORTB |= (1<<PB2);
		        	boot_program_page(u8PageNo,&pu8ReqData[0]);
		        	LOC_vidSendPosResp();
		            u8PageNo ++ ;
		            u16reicevedlen += PAGE_SIZE;
		            if(u16reicevedlen == u16codesize){
		            	enuDownloadState = waiting_TransferExit;
		            }else{

		            }
		        }else{
		        	enuDownloadState = waiting_ProgrammingSession;
		        	LOC_vidSendNegResp();
		        	PORTB |= (1<<PB3);
		        }
		    }break;

		    case TRANSFER_EXIT:
		    	if ((enuDownloadState == waiting_TransferExit) && (u8ReqLen == 1)) {
		    	    LOC_vidSendPosResp();
		    	    enuDownloadState=waiting_ChekCrc;
		    	}else{
		    		enuDownloadState = waiting_ProgrammingSession;
		    		LOC_vidSendNegResp();
		    	}
		    	break;
		    case CHECK_CRC :
// 				if ((enuDownloadState == waiting_TransferExit) && (u8ReqLen == 3))
		    	if ((enuDownloadState == waiting_ChekCrc) && (u8ReqLen == 1)) {
		    		//u16ReceivedCRC = pu8ReqData[0]<<8 |(pu8ReqData[1]);
		    		bValidCRC = 1;
		    		if(bValidCRC){
		    			LOC_vidSendPosResp();
		    			eeprom_update_byte(VALID_APP_ADDRESS,1);
		    			_delay_ms(100);
		    			enuDownloadState = waiting_ProgrammingSession;
		    			asm("jmp 0");
		    		}
		    	}else{
		    		LOC_vidSendNegResp();
		    		enuDownloadState = waiting_ProgrammingSession;
		    	}


		    	break ;
		    default :
		    	break ;



		}
		bIsRequestReceived = FALSE ;

	}
}

extern void flashing_vidRxNotification(uint8_t* pu8Data , uint8_t u8len){
	DDRB = 0xFF;                 // Configure tous les bits de PORTD comme sortie
	PORTB |= (1 << PB1);
	bIsRequestReceived = TRUE;
	u8SID = pu8Data[0];
	u8ReqLen = u8len;
	pu8ReqData = &pu8Data[1];
}
uint8_t LOC_vidCheckFlashCRC(uint16_t u16StartAdd, uint16_t u16EndAdd, uint16_t u16CRC){
	uint16_t addr;
	uint8_t u8Byte;
	uint16_t ucrc16 =0xFFFF;
	for(addr = u16StartAdd ;addr < u16EndAdd ; addr++){
		u8Byte = pgm_read_byte(addr);
		ucrc16 = _crc16_update(ucrc16,u8Byte);
	}
	if(u16CRC != ucrc16 ){
		return 0;
	}else {
		return 1;
	}
}

void LOC_vidSendNegResp(){
	uart_send_char(0x7F);
}

void LOC_vidSendPosResp(){
	uart_send_char(u8SID + 0x40);
}
