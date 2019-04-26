/*
* CFile1.c
*
* Created: 3/6/2019 8:55:22 AM
*  Author: ebrah
*/
#include "Remot_control.h"


//static variables
static Enum_RemotControl_t Rov_ControlStatus=0;
static Struct_Order_t data_rx;
static uint8 counter=0;
Spi_Config my_spi={SPI_INTERRUPT_DISABLE,SPI_LSB_FIRST,SPI_MASTER_MODE,SPI_RAISING_EDGE,SPI_SAMPLE,SPI_FOSC_DIV16}; //  init value for spi
static uint8 handler=0,Auto_BufferSize=0,Auto_BufferIndex=0; // for timer
static uint8* ptr_AutoBufferCommands;
static uint8 First_digit,Second_digit;
volatile static uint8 time_Seconds; // 0>>99

static void new_DataRx(uint8 data) // from controller
{
	if(counter) // counter==1 that mean it rx the key"W S D A"
	{
		counter=0;
		data_rx.power_level=data;
		Remote_ControlSemphore=1;
	}
	else
	{
		data_rx.key=data;
		counter++;
	}
}

static void Command_TimerFinish()
{
	if (Auto_BufferIndex < Auto_BufferSize )
	{ // the form of command i suppose will be [Dirction_8bit,Power_8bit,Time_16bit]
		data_rx.key= ptr_AutoBufferCommands[Auto_BufferIndex++]; // take the first mean dirction and increament
		data_rx.power_level = ptr_AutoBufferCommands[Auto_BufferIndex++]; // the scond should be power and increament
		time_Seconds  = 0;
		time_Seconds  = (ptr_AutoBufferCommands[Auto_BufferIndex++]-48)*10; // the second digit
		time_Seconds += (ptr_AutoBufferCommands[Auto_BufferIndex++]-48); // the first digit
		First_digit=0;
		Second_digit=0;
		Remote_ControlSemphore=1;
		TMU_semphore=1;
	}
	else
	{
		// Commands finished
		end_event(0);
		TMU_semphore=0;
		lcd_clear(&mylcd);
		lcd_str(&mylcd,0,0,"Commands Finish");
	}
}

static void OneSecondFinish()
{
	if ((--time_Seconds)) // decreament the seconds 
	{
		Remote_ControlSemphore=1;
		First_digit++;
		if (First_digit>9)
		{
			Second_digit++;
			First_digit=0;
		}
	}
	else
	{
		Auto_BufferIndex++;
		Command_TimerFinish();
	}
}



static void Commands_AutoBufferArrive(uint8* ptr_Data,uint16 Size)
{
	CommRX_DisspatcherSemphore=0; // Close the Disspatcher of work Becuase i received what i need
	Remote_ControlSemphore=1; // work the controller motor
	Auto_BufferSize=Size-1;
	ptr_AutoBufferCommands=ptr_Data;
	Command_TimerFinish();
	TMU_init(TMR0,_1MS); // set timer to start working

}

static void get_mode(uint8 key)
{
	Keypad_semphore=0; // to lock keypad
	spi_init(&my_spi);
	if (key == '1')
	{
		Rov_ControlStatus = LIVE_CONTROL;
		Uart_init();   //channel used for live control it can be configured
		Uart_RxInterruptEnable(new_DataRx);
		digitalWrite(12,SET_VALUE_LOW);
	}
	else if(key == '2')
	{
		lcd_clear(&mylcd);
		lcd_str(&mylcd,0,0,"Wait Commands");
		Rov_ControlStatus = AUTTO_BUFFER_CONTROL;
		RX_cfg_t Buffer_CommandsConfg={Commands_AutoBufferArrive,UART0,'*'}; // no need for static because it's already saved in static variable in the CommRx.c
		Comm_RxInit(&Buffer_CommandsConfg);
		CommRX_DisspatcherSemphore=1;
		Struct_EventConfg_t NewCommand_Event ={1000,PEROIDEC,1,OneSecondFinish}; // new_event confg  after 1 second will call OneSecondFinish
		creat_oneHandlerEvent(&NewCommand_Event);
	}
	else
	{
		lcd_clear(&mylcd);
		lcd_str(&mylcd,0,0,"wrong choice");
		_delay_ms(700);
		SYSTEM_OPTIONS_flag=1;
	}
}

void Remot_controlMode()
{
	lcd_clear(&mylcd);
	if (System_logged)
	{
		lcd_str(&mylcd,0,0,"enter mode*1-live control*2-auto buffer*");
		kpad_init(&mykeypad,get_mode);
		Keypad_semphore=1;
	}
	else
	{
		lcd_str(&mylcd,0,0,"enter system*first please*");
	}
}

void Remot_controlDisspatcher()
{
	Remote_ControlSemphore=0;
	lcd_clear(&mylcd);
	switch (data_rx.key)
	{
		case 'w':// forward
		{
			lcd_str(&mylcd,0,0,"Rov Go forward*");
			spi_master_tx('w');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		case 's'://backward
		{
			lcd_str(&mylcd,0,0,"Rov Go backword*");
			spi_master_tx('s');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		case 'd'://right
		{
			lcd_str(&mylcd,0,0,"Rov Go right*");
			spi_master_tx('d');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		case 'a'://left
		{
			lcd_str(&mylcd,0,0,"Rov Go left*");
			spi_master_tx('a');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		default:
		{
			spi_master_tx(0);
			spi_master_tx(0);
			lcd_str(&mylcd,0,0,"Rov stable*");
		}
	}
	lcd_str(&mylcd,1,0,"Power:");
	lcd_chr(&mylcd,data_rx.power_level);
	if (Rov_ControlStatus==AUTTO_BUFFER_CONTROL)
	{
		lcd_str(&mylcd,2,0,"Time:");
		lcd_chr(&mylcd,Second_digit+48);
		lcd_chr(&mylcd,First_digit+48);
	}
}
