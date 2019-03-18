/*
* ECU_CONTROLLER.c
*
* Created: 3/6/2019 1:04:45 PM
* Author : ebrah
*/

#include "main.h"
uint16 adc_val;// value captured from adc in 0:1023
double power_value; // value captured from adc in 0:5 volt
uint8 handler; // hold timer_handler


uint8 powr_level=0; // take the value of adc converted 
volatile bool_t Status_flag=0,power_flag=0; // to make sure the complier don't optimize it  and it use to sent the updated value


volatile Enum_RovStatus_t _Rov_status_new=0,_Rov_status_old=STABLE;


uint8 Converte_adcVal(Channel_SelectType channel)
{
	adc_val=adc_read(channel);
	power_value = ((double)adc_val/1023);
	power_value *=5;
	return (uint8)power_value;
}


void adc_dispatcher()
{
	volatile uint8 temp;
	if (_Rov_status_new!=STABLE) // don't have let adc work if the rov is stable
	{
		temp=Converte_adcVal((_Rov_status_new));
		if (powr_level!=temp)
		{
			powr_level=temp;
			power_flag=1; // to send the new updated value
		}
	}
}

void Button_dispatcher()
{
	if (digitalRead(C0)) //forward
	{
		_Rov_status_new=FORWARD;
	}
	else if (digitalRead(C1))//backward
	{
		_Rov_status_new=BACKWORD;
	}
	else if (digitalRead(C2))//Right
	{
		_Rov_status_new=RIGHT;
	}
	else if (digitalRead(C3))//Left
	{
		_Rov_status_new=LEFT;
	}
	else
	{
		_Rov_status_new=STABLE;
	}
	
	if (_Rov_status_new != _Rov_status_old)
	{
		Status_flag=1; // set flag to indicate rov status changed 
		
		_Rov_status_old=_Rov_status_new;
	}
}

void input_dispatcher()
{
	Button_dispatcher();
	adc_dispatcher();
}

void system_init()
{
	//gpio initialize
	SetPinDirection(C0,INPUT); //For buttons
	SetPinDirection(C1,INPUT);
	SetPinDirection(C2,INPUT);
	SetPinDirection(C3,INPUT);
	gpio_uart_cng();
	Port_Write(A,0x00); // make port a input
	
	//uart_initialize
	Uart_init();
	
	//adc_initalize
	adc_init(AVCC,FREE_RUNNING_MODE,CLK_ADC_DIV_128); // init value for adc
	
	//time mangement unit init
	TMU_init(TMR0,_1MS);
	Struct_EventConfg_t My_event ={40,PEROIDEC,1,input_dispatcher}; // new_event confg
	Creat_NewEvent(&My_event,&handler);
}

int main(void)
{
	system_init();
	sei();
	while (1)
	{
		TMU_Dispatcher(); // to call input dispatcher
		if ((power_flag)||(Status_flag)) // Status changed
		{
			power_flag=0;
			Status_flag=0;
			switch (_Rov_status_new)
			{
				case STABLE :
				{
					Uart_SendByte(0);
					_delay_ms(1);
					Uart_SendByte(0);
					break;
				}
				case FORWARD:
				{
					Uart_SendByte('w');
					_delay_ms(1);
					Uart_SendByte(powr_level+48);
					break;
				}
				case BACKWORD:
				{
					Uart_SendByte('s');
					_delay_ms(1);
					Uart_SendByte(powr_level+48);
					break;
				}
				case RIGHT:
				{
					Uart_SendByte('d');
					_delay_ms(1);
					Uart_SendByte(powr_level+48);
					break;
				}
				case LEFT:
				{
					Uart_SendByte('a');
					_delay_ms(1);
					Uart_SendByte(powr_level+48);
					break;
				}
			}
		}
	}
}

