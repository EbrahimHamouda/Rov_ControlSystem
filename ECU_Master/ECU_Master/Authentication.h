/*
 * Authentication.h
 *
 * Created: 2/22/2019 6:47:09 PM
 *  Author: ebrahim
 */ 


#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

#include "lcd.h"
#include "key_pad.h"
#include "eeprom.h"
#include <common/ebra_common.h>
#include <util/delay.h>
#include "time_mang_unit.h"

//Global variables
extern struct_PinsCnfg_t mylcd;  // lcd_Conifguration to be used from other modules
extern	KeyPad_PinCnfg_t mykeypad; // keypad_Conifguration to be used from other modules
extern	bool_t Keypad_semphore; // semphore to idicate the availbity
extern	bool_t lcd_semphore; // semphore to idicate the availbity
extern	bool_t System_logged;//to know if user enter the password or not
extern  bool_t SYSTEM_OPTIONS_flag; // set it if u want to open option for user again
extern	bool_t TMU_semphore; //set it if i want use tmu
extern  bool_t authentication_DispatcherSemphore;

// CONSTANTS
#define WRONG_TRIALS_LOCATION 16
#define THREESHOLD_TRIALS 3
#define PASSWORD_LENGTH 9
#define PASSWOR_CS_LOCATION 9



typedef enum
{
	_BUFFER_PASSWORD, // change passowrd
	_CALC_PASSWORD_CS,
	_CHECK_PASSWORD, 
	_PASSWORD_RIGHT,
	_PASSWORD_WRONG, 
	_CHANGE_PASSWORD,
	_CONFRIM_PASSWORD,
	_CHECK_CHANGE_PASSWORD, 	
} Authentication_Status_t;


//this funcation use to initalize the form to enter the password
void Authentication_Dispatcher();

void System_LogIn();

void Change_password();

void Change_PasswordCs();
#endif /* AUTHENTICATION_H_ */


/*
EEPROM_write(0,'1');
EEPROM_write(1,'2');
EEPROM_write(2,'3');
EEPROM_write(3,'4');
EEPROM_write(4,'5');
EEPROM_write(5,'6');
EEPROM_write(6,'7');
EEPROM_write(7,'8');
EEPROM_write(8,'9');
*/