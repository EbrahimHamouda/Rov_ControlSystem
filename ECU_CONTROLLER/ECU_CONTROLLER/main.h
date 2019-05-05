/*
 * main.h
 *
 * Created: 3/17/2019 2:28:39 PM
 *  Author: ebrah
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include "USART.h"
#include "GPIO.h"
#include "time_mang_unit.h"
#include "ADC.h"
#include <util/delay.h>
#include <avr/interrupt.h>

typedef enum
{
	FORWARD=0,
	BACKWORD=1,
	RIGHT=2,
	LEFT=3,
	STABLE=4,
} Enum_RovStatus_t;



#endif /* MAIN_H_ */