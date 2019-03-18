/*
* ECU_MOTOR_CONTROL.c
* Created: 3/8/2019 1:51:39 PM
* Author : ebrah
*/

#include <avr/io.h>
#include "motor_driver.h"
#include "pwm.h"
#include "F:\embedded\Work_space\2_Drivers_atmega32\SDK_AVR_ATMEGA32\HAL\SPI\SPI.h"
#include <avr\interrupt.h>

Spi_Config my_spi={SPI_INTERRUPT_ENABLE,SPI_LSB_FIRST,SPI_SLAVE_MODE,SPI_RAISING_EDGE,SPI_SAMPLE,SPI_FOSC_DIV16};
volatile uint8 New_dataSemphore=0;
typedef struct
{
	uint8 key ;
	uint8 power_level;
} Struct_Order_t;

volatile static Struct_Order_t data_rx;
static uint8 counter=0;

void new_data(unsigned char data)
{
	if(counter) // counter==1 that mean it rx the key"W S D A"
	{
		New_dataSemphore=1;
		counter=0;
		data_rx.power_level=data;
	}
	else
	{
		data_rx.key=data;
		counter++;
	}
}


void system_init()
{
	motor_init();
	pwm_init();
	spi_init(&my_spi);
	gpio_spi_cng_slave();
	Spi_AssignCallBack(new_data);
}

int main(void)
{
	sei();
	system_init();

	while (1)
	{
		if (New_dataSemphore)
		{
			New_dataSemphore=0;
			switch(data_rx.key)
			{
				case 'w' :
				{
					Go_forward(data_rx.power_level-48);
					break;
				}
				case 's' :
				{
					Go_backward(data_rx.power_level-48);
					break;
				}
				case 'd' :
				{
					Go_Right(data_rx.power_level-48);
					break;
				}
				case 'a' :
				{
					Go_Left(data_rx.power_level-48);
					break;
				}
				default:
				{
					motor_init();
				}
			}
		}
	}
}


