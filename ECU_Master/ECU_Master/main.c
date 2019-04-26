/*
* Master_ECU.c
*
* Created: 2/19/2019 3:44:39 PM
* Author : ebrahim hamouda
*/

#include <avr/io.h>
#include <common/ebra_common.h>
#include "Authentication.h"
#include <avr/interrupt.h>
#include "Remot_control.h"
 
// Golabl variables it's
struct_PinsCnfg_t mylcd={{A0,A1,A2,A3},A4,A5};  // lcd_Conifguration to be used from other modules
KeyPad_PinCnfg_t mykeypad={{C2,C3,C4,C5},{C6,C7,D2}}; // keypad_Conifguration to be used from other modules
bool_t Keypad_semphore = 0; // semphore to idicate the availbity
bool_t lcd_semphore =0 ; // semphore to idicate the availabilty
bool_t System_logged=0;  //to know if user enter the password or not
bool_t SYSTEM_OPTIONS_flag=0; // to show the options again
bool_t TMU_semphore=0; // semphore to indicate the use ability
bool_t authentication_DispatcherSemphore=0; // to make dispatcher of authentication work
bool_t Remote_ControlSemphore=0; // to make live control work
bool_t CommRX_DisspatcherSemphore=0;// to make commrxwork

void Get_option(uint8 num)
{
	Keypad_semphore = 0; // to make it free to use from other modules
	switch(num)
	{
		case '1' :
		{   //authentication
			System_LogIn();
			break;
		}
		case '2' :
		{	// bootloadr
			break;
		}
		case '3' :
		{	//remot control
			Remot_controlMode();
			break;
		}
		case '4' :
		{	// change passwor "authentication"
			Change_password();
			break;
		}
	}
}

void system_init()
{
	kpad_init(&mykeypad,Get_option);
	lcd_init(&mylcd);
	gpio_spi_cng_master();
	gpio_uart_cng();
	/* WelCome Message
	lcd_goto(&mylcd,0,4);
	lcd_str(&mylcd,"welcome");
	lcd_goto(&mylcd,1,4);
	lcd_str(&mylcd,"to vehicle");
	lcd_goto(&mylcd,2,4);
	lcd_str(&mylcd,"Control");
	lcd_goto(&mylcd,3,4);
	lcd_str(&mylcd,"System ^^");
	*/
}

void system_options()
{
	lcd_clear(&mylcd);
	lcd_str(&mylcd,0,0,"1-Enter system*2-Update system*3-Remote control*4-settings"); // authentication
	Change_CallBackfunction(Get_option); // change call back funcation
	Keypad_semphore = 1;  // run the Keypad for get option
}

int main(void)
{
	system_init();
	system_options();
	sei(); // open global interrupt for timer
	while (1)
	{
		if (Keypad_semphore)
		{
			keyPad_dispatcher(&mykeypad);
		}
		if (SYSTEM_OPTIONS_flag)
		{
			SYSTEM_OPTIONS_flag=0;
			system_options();
		}
		if (TMU_semphore)
		{
			TMU_Dispatcher();
		}
		if (authentication_DispatcherSemphore)
		{
			Authentication_Dispatcher();
		}
		if (Remote_ControlSemphore)
		{
			Remot_controlDisspatcher();
		}
		if (CommRX_DisspatcherSemphore)
		{
			RX_com_dispatch();
		}
	}
}

