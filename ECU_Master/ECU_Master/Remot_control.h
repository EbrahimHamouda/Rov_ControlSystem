/*
 * Remot_control.h
 *
 * Created: 3/6/2019 8:53:25 AM
 *  Author: ebrah
 */ 


#ifndef REMOT_CONTROL_H_
#define REMOT_CONTROL_H_

#include "lcd.h"
#include "eeprom.h"
#include "key_pad.h"
#include <common/ebra_common.h>
#include <util/delay.h>
#include "time_mang_unit.h"
#include "USART.h"
#include "SPI.h"
#include "commRX.h"


//Global variables
extern struct_PinsCnfg_t mylcd;  // lcd_Conifguration to be used from other modules
extern	KeyPad_PinCnfg_t mykeypad; // keypad_Conifguration to be used from other modules
extern	bool_t Keypad_semphore; // semphore?? to idicate the availbity
extern	bool_t lcd_semphore; // semphore to idicate the availbity
extern	bool_t System_logged;//to know if user enter the password or not
extern  bool_t SYSTEM_OPTIONS_flag; // set it if u want to open option for user again
extern	bool_t TMU_semphore; //set it if i want use tmu
extern  bool_t Remote_ControlSemphore; // to make live control work
extern  bool_t CommRX_DisspatcherSemphore; // to make commrx work

typedef struct
{
	uint8 key ;
	uint8 power_level;
} Struct_Order_t;

typedef enum
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LIFT,
} Rov_state_t;
typedef enum
{
	LIVE_CONTROL,
	AUTTO_BUFFER_CONTROL,
} Enum_RemotControl_t;

void Remot_controlMode();
void Remot_controlDisspatcher();

#endif /* REMOT_CONTROL_H_ */