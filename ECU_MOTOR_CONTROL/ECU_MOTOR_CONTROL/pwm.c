/*
* pwm.c
*
* Created: 3/8/2019 7:10:30 PM
*  Author: ebrah
*/

#include<avr/io.h>
#include "pwm.h"

void pwm_init()
{
	/* Fast pwm
	* set when compare match & clear when reach bottom
	* fout = clck/256*64 =61 hz
	* i decrease frequency so i can simulate without "?????"
	* clear counter ragister
	* make pins OC0 output
	*/
	
	//timer0
	TCCR0 = (1<<6)|(1<<3)|(1<<5)|(3<<0);
	TCNT0 =0;
	SetPinDirection(B3,OUTPUT);
	
	//timer1
	TCCR1A = (1<<7)|(1<<5)|(1<<0); // use oc0, fast pwm mode
	TCCR1B = (1<<3)|(3<<0);
	TCNT1L =0;
	TCNT1H =0;
	SetPinDirection(D4,OUTPUT);
	SetPinDirection(D5,OUTPUT);
	
	//timer2
	TCCR2 = (1<<6)|(1<<3)|(1<<5)|(1<<2);
	TCNT2=0;
	SetPinDirection(D7,OUTPUT);
}

void pwm_stop(uint8 pin)
{
	switch(pin)
	{
		case B3:
		{
			TCCR0 &= ~(1<<5);	//disconnect oco
			break;
		}
		case D4:
		{
			TCCR1A &= ~(1<<5); // disconnect oc1b
			break;
		}
		case D5:
		{
			TCCR1A &= ~(1<<7); // disconnect oc1a
			break;
		}
		case D7:
		{
			TCCR2 &= ~(1<<5); // disconnect oc2
			break;
		}
	}
}
void pwm_generate(uint8 pin, int val)
{
	// We need to make sure the PWM output is enabled for those pins
	// that support it, as we turn it off when digitally reading or
	// writing with them.  Also, make sure the pin is in output mode
	// for consistently with Wiring, which doesn't require a pinMode
	// call for the analog output pins.
	//pinMode(pin, OUTPUT);
	/*if (val == 0)
	{
	pwm_stop(pin);
	digitalWrite(pin, LOW);
	}
	else if (val == 255)
	{
	pwm_stop(pin);
	digitalWrite(pin, HIGH);
	}
	else
	{*/
		switch(pin)
		{
			case B3:
			{
				OCR0=val;
				break;
			}
			case D4:
			{
				OCR1B=val;
				break;
			}
			case D5:
			{
				OCR1A=val;
				break;
			}
			case D7:
			{
				OCR2=val;
				break;
			}
		}
	}