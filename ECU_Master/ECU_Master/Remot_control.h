/*
 * Remot_control.h
 *
 * Created: 3/6/2019 8:53:25 AM
 *  Author: ebrah
 */ 


#ifndef REMOT_CONTROL_H_
#define REMOT_CONTROL_H_

#include "F:\embedded\Work_space\2_Drivers_atmega32\SDK_AVR_ATMEGA32\ECUS\LCD\lcd.h"
#include "eeprom.h"
#include "F:\embedded\Work_space\2_Drivers_atmega32\SDK_AVR_ATMEGA32\ECUS\KEYPAD\key_pad.h"
#include <common/ebra_common.h>
#include <util/delay.h>
#include "F:\embedded\Work_space\2_Drivers_atmega32\Layres\2_service layer\time_mange_unit\time_mange_unit\time_mang_unit.h"
#include "F:\embedded\Work_space\2_Drivers_atmega32\SDK_AVR_ATMEGA32\HAL\UART\USART.h"

//Global variables
extern struct_PinsCnfg_t mylcd;  // lcd_Conifguration to be used from other modules
extern	KeyPad_PinCnfg_t mykeypad; // keypad_Conifguration to be used from other modules
extern	bool_t Keypad_semphore; // semphore to idicate the availbity
extern	bool_t lcd_semphore; // semphore to idicate the availbity
extern	bool_t System_logged;//to know if user enter the password or not
extern  bool_t SYSTEM_OPTIONS_flag; // set it if u want to open option for user again
extern	bool_t TMU_semphore; //set it if i want use tmu
extern  bool_t Live_ControlSemphore; // to make live control work


typedef struct
{
	uint8 key ;
	uint8 power_level;
} Struct_Order_t;


typedef enum
{
	LIVE_CONTROL,
	AUTTO_BUFFER_CONTROL,
} Enum_RemotControl_t;

void Remot_controlMode();
void live_controlDisspatcher();

#endif /* REMOT_CONTROL_H_ */