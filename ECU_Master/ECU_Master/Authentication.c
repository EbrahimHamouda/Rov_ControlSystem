/*
* Authentication.c
*
* Created: 2/22/2019 6:47:49 PM
*  Author: ebrah
*/
#include "Authentication.h"

//Static variables
static uint8 Input_PasswordBuffer[PASSWORD_LENGTH],buffer_index=0; // used in this file only
static uint32 Input_PasswordCs,confirm_passwordCs;
static uint8 num_S1=0;  // used to show seconds
static uint8 num_S2=0;	// used to show seconds
static uint8 handler;	// used in tmu to get the num_ID of event
static bool_t Change_PasswordSemphore=0; //
static Authentication_Status_t Au_status; // authentication center status

static uint32 Read_PasswordCs()
{
	uint32 Password_Cs=0;
	uint8 i = PASSWOR_CS_LOCATION;
	Password_Cs |= (uint32)EEPROM_read(i++);
	Password_Cs |= (uint32)(EEPROM_read(i++))<<8;
	Password_Cs |= (uint32)(EEPROM_read(i++))<<16;
	Password_Cs |= (uint32)(EEPROM_read(i))<<24;
	return Password_Cs;
}

static void Give_UserNewTry(uint8 temp) // this funcation called when password enterd wrong
{
	EEPROM_write(WRONG_TRIALS_LOCATION,temp);
	lcd_clear(&mylcd);	// clear lcd
	lcd_str(&mylcd,0,0,"password wronge*Number trials:"); // write massage
	lcd_chr(&mylcd,(temp|(0x30)));
	lcd_str(&mylcd,2,0,"Enter password*"); // write massage
	buffer_index=0;
	Input_PasswordCs=0;
	Keypad_semphore =1; // to start receive password again
	Au_status = _BUFFER_PASSWORD;
}

static void One_SecondFinish()	// this funcation called from TMU module when second finish
{
	lcd_goto(&mylcd,3,0); // to show seconds in lcd
	lcd_chr(&mylcd,(0x30|num_S2));
	lcd_goto(&mylcd,3,1);
	lcd_chr(&mylcd,(0x30|num_S1));
	if ((num_S2==6)&&(num_S1==0)) // that mean the minute finished
	{
		end_event(handler);// to end tmu event and
		TMU_DeInit(); // to stop ISR from timer
		TMU_semphore=0; // to free the tmu
		EEPROM_write(WRONG_TRIALS_LOCATION,0); //clear_WrongTrails
		Give_UserNewTry(0); // zero because i just cleared the num_trials
	}
	num_S1++;
	if (num_S1>9)
	{
		num_S2++;
		num_S1=0;
	}
}

static void Set_TimerOneMinute()
{
	Struct_EventConfg_t Authentication_Event ={1000,PEROIDEC,1,One_SecondFinish}; // new_event confg
	TMU_init(TMR0,_1MS);
	Creat_NewEvent(&Authentication_Event,&handler);
	TMU_semphore=1;
	num_S1=0;  // set to 00
	num_S2=0;
}

void Change_PasswordCs()
{
	uint8 i=PASSWOR_CS_LOCATION;
	EEPROM_write(i++,(uint8)(Input_PasswordCs));
	EEPROM_write(i++,(uint8)(Input_PasswordCs>>8));
	EEPROM_write(i++,(uint8)(Input_PasswordCs>>16));
	EEPROM_write(i++,(uint8)(Input_PasswordCs>>24));
}

static uint8 check_WrongTrials()
{
	uint8 temp; // counter , temp to hold the value from eeprom
	temp = EEPROM_read(WRONG_TRIALS_LOCATION); // read the current number of trial
	if (temp < THREESHOLD_TRIALS)// check numbers of trials wrong_password
	{
		return ++temp;
	}
	else
	{
		return 0;
	}
}

static void Check_Password()
{
	authentication_DispatcherSemphore=1;
	if (Input_PasswordCs==Read_PasswordCs())//to check if the buffer index == to buffer index
	{
		Au_status = _PASSWORD_RIGHT;
	}
	else
	{
		Au_status = _PASSWORD_WRONG;
	}
}

static void Buffer_Password(uint8 key)
{
	if ((key!='#')&&(buffer_index < PASSWORD_LENGTH)) // check user end the password
	{
		if (Change_PasswordSemphore)
		{
			Input_PasswordBuffer[buffer_index] = key;
		}
		lcd_chr(&mylcd,'*');
		Input_PasswordCs += (buffer_index*key);
		buffer_index++;
	}
	else
	{
		if(Change_PasswordSemphore)
		{
			if (confirm_passwordCs==0)
			{
				Au_status=_CONFRIM_PASSWORD;
			}
			else
			{
				Au_status=_CHECK_CHANGE_PASSWORD;
			}
		}
		else
		{
			Au_status=_CHECK_PASSWORD;
		}
		Keypad_semphore=0; // don't need any key else from user
		authentication_DispatcherSemphore=1;
	}
}

void Change_password()
{
	if (System_logged) // system not logged
	{
		lcd_clear(&mylcd);
		lcd_str(&mylcd,0,0,"enter New*Password please*");
		Au_status = _BUFFER_PASSWORD;
		confirm_passwordCs=0;
		Change_PasswordSemphore=1;
		authentication_DispatcherSemphore=1;
	}
	else
	{
		lcd_clear(&mylcd);
		lcd_str(&mylcd,0,0,"enter system*first please*");
		_delay_ms(1000);
		SYSTEM_OPTIONS_flag=1; // to open options one more time
	}
}

void Authentication_Dispatcher()
{
	authentication_DispatcherSemphore=0;
	switch(Au_status)
	{
		case _BUFFER_PASSWORD:
		{
			Input_PasswordCs=0;buffer_index=0;
			Change_CallBackfunction(Buffer_Password);
			Keypad_semphore=1;
			break;
		}
		case _CHECK_PASSWORD:
		{
			Check_Password();
			break;
		}
		case _PASSWORD_RIGHT:
		{
			lcd_clear(&mylcd); // clear
			lcd_str(&mylcd,0,0,"password Right"); // tell user it's password right
			_delay_ms(1000); // to make user see the message
			SYSTEM_OPTIONS_flag = 1;
			System_logged=1;	// enter logged mode
			Keypad_semphore = 0; //free keypad
			EEPROM_write(WRONG_TRIALS_LOCATION,0); // zero the counter of wrong times of password
			break;
		}
		case _PASSWORD_WRONG:
		{
			uint8 temp = check_WrongTrials();
			if(temp)
			{
				Give_UserNewTry(temp);
			}
			else
			{
				Keypad_semphore = 0; //free keypad
				lcd_clear(&mylcd);	// clear lcd
				lcd_str(&mylcd,0,0,"you exceeded*limit of Trials*wait a minute"); // write massage
				Set_TimerOneMinute();
			}
			break;
		}
		case _CONFRIM_PASSWORD:
		{
			confirm_passwordCs = Input_PasswordCs;
			Input_PasswordCs=0;
			buffer_index=0;
			lcd_goto(&mylcd,3,0);
			Keypad_semphore=1;
			break;
		}
		case _CHECK_CHANGE_PASSWORD:
		{
			SYSTEM_OPTIONS_flag=1;
			Change_PasswordSemphore=0;
			if (confirm_passwordCs==Input_PasswordCs)
			{
				System_logged=0;
				lcd_clear(&mylcd);
				Change_PasswordCs();
				lcd_str(&mylcd,0,0,"password*change*successfully");
				_delay_ms(1000);
			}
			else
			{
				lcd_clear(&mylcd);
				lcd_str(&mylcd,0,0,"password*Don't match*please Try*again");
				_delay_ms(1000);
			}
			break;
		}
	}
}

void System_LogIn()
{
	if (System_logged!=1) // system not logged
	{
		lcd_clear(&mylcd);
		lcd_str(&mylcd,0,0,"enter password*press'#'to*end-password*");
		Au_status = _BUFFER_PASSWORD;
		authentication_DispatcherSemphore=1;
	}
	else
	{
		lcd_clear(&mylcd);
		lcd_str(&mylcd,0,0,"system is logged");
		_delay_ms(500);
		SYSTEM_OPTIONS_flag=1; // to open options one more time
	}
}


