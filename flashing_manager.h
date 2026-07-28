
/*
 * flashing_manager.h
 *
 *  Created on: 11 nov. 2024
 *      Author: admin
 */


#ifndef FLASHING_MANAGER_H_
#define FLASHING_MANAGER_H_

void flashingMngr_vidMainTask() ;
void flashingMngr_vidHandleReqFromApp(void);
#define VALID_APP_ADDRESS			((uint8_t*) 0x00)
#define REQ_FROM_APP_ADDRESS	((uint8_t*) 0x01)

#endif /* FLASHING_MANAGER_H_ */
