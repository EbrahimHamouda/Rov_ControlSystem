/*
* CFile1.c
*
* Created: 3/6/2019 8:55:22 AM
*  Author: ebrah
*/
#include "Remot_control.h"
#include "F:\embedded\Work_space\2_Drivers_atmega32\Layres\0_HAL_test_drivers_\spi\spi2\SPI.h"

//static variables
static Enum_RemotControl_t Rov_ControlStatus=0;

static Struct_Order_t data_rx;
static uint8 counter=0;
Spi_Config my_spi={SPI_INTERRUPT_DISABLE,SPI_LSB_FIRST,SPI_MASTER_MODE,SPI_RAISING_EDGE,SPI_SAMPLE,SPI_FOSC_DIV16}; //  init value for spi


static void new_DataRx(uint8 data)
{
	if(counter) // counter==1 that mean it rx the key"W S D A"
	{
		counter=0;
		data_rx.power_level=data;
		Live_ControlSemphore=1;
	}
	else
	{
		data_rx.key=data;
		counter++;
	}
}

static void get_mode(uint8 key)
{
	Keypad_semphore=0; // to lock keypad
	if (key == '1')
	{
		Rov_ControlStatus = LIVE_CONTROL;
		Uart_init();   //channel used for live control it can be configured
		Uart_RxInterruptEnable(new_DataRx);
		spi_init(&my_spi);
		digitalWrite(12,SET_VALUE_LOW);
	}
	else if(key == '2')
	{
		Rov_ControlStatus = AUTTO_BUFFER_CONTROL;
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

void live_controlDisspatcher()
{
	Live_ControlSemphore=0;
	switch (data_rx.key)
	{
		case 'w':// forward
		{
			lcd_clear(&mylcd);
			lcd_str(&mylcd,0,0,"Rov Go forward*");
			spi_master_tx('w');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		case 's'://backward
		{
			lcd_clear(&mylcd);
			lcd_str(&mylcd,0,0,"Rov Go backword*");
			spi_master_tx('s');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		case 'd'://right
		{
			lcd_clear(&mylcd);
			lcd_str(&mylcd,0,0,"Rov Go right*");
			spi_master_tx('d');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		case 'a'://left
		{
			lcd_clear(&mylcd);
			lcd_str(&mylcd,0,0,"Rov Go left*");
			spi_master_tx('a');
			/*_delay_ms(1);*/
			spi_master_tx(data_rx.power_level);
			break;
		}
		default:
		{
			lcd_clear(&mylcd);
			spi_master_tx(0);
			spi_master_tx(0);
			lcd_str(&mylcd,0,0,"Rov stable*");
		}
	}
	lcd_str(&mylcd,1,0,"Power:");
	lcd_chr(&mylcd,data_rx.power_level);
}
