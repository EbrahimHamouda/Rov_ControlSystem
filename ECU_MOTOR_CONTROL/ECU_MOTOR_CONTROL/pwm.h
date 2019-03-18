/*
 * pwm.h
 *
 * Created: 3/8/2019 8:48:43 PM
 *  Author: ebrah
 */ 


#ifndef PWM_H_
#define PWM_H_
#include "F:\embedded\Work_space\2_Drivers_atmega32\SDK_AVR_ATMEGA32\HAL\GPIO\GPIO.h"


void pwm_init();

void pwm_generate(uint8 pin, int val);

void pwm_stop(uint8 pin);

#endif /* PWM_H_ */