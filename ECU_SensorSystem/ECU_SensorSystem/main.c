/*
* ECU_SensorSystem.c
*
* Created: 4/27/2019 8:57:02 AM
* Author : ebrahim hamouda
*/
# define F_CPU 1000000UL

#include <avr/io.h>
#include "ultraSonicSensor.h"
#include "time_mang_unit.h"
#include "DHT11.h"
#include "RTCDS1307.h"
#include "commTx.h"
#include "Message_Sensors.h"


// variables
extern uint8 Sensors_Message[1000];
extern uint16 Message_length;

//volatile uint16 data=0;
volatile uint16 UltraSonic_arr[2];
UltraSonic_t ultrasnoic1={D4,EXT_INT0};
UltraSonic_t ultrasnoic2={D5,EXT_INT1};
rtc_t RTC1;
DHT11_t DHT11_1;

void Tx_Finish()
{
	comm_TxRest();
}

void PeroidecSensors_Check()
{
	TMU_Disable(); // make sure that i can finish whatever here
	UltraSonic_arr[0]=ultraSonic_Read(&ultrasnoic1);
	UltraSonic_arr[1]=ultraSonic_Read(&ultrasnoic2);
	DHT11_RequestData();
	DHT11_GetResponse();
	DHT11_ReceiveData(&DHT11_1);
	RTC_GetDateTime(&RTC1);
	Message_update();
	CommTx_DataCfgType Transmit_ex = {Tx_Finish,Sensors_Message,Message_length,UART0_CHANNEL};
	comm_TxInit(&Transmit_ex);
	TMU_Enable();
}


void Syst_init()
{
	Message_init();
	SetPinDirection(B1,OUTPUT);
	UltraSonic_Init(&ultrasnoic2);
	UltraSonic_Init(&ultrasnoic1);
	_delay_ms(50);
	RTC_Init();
	Struct_EventConfg_t Read_SensorsEvent={5000,PEROIDEC,1,PeroidecSensors_Check};
	TMU_init(TMR0,_1MS);
	creat_oneHandlerEvent(&Read_SensorsEvent);
}

int main(void)
{
	Syst_init();
	
	sei();

	while(1)
	{
		TMU_Dispatcher();
		comm_TxDisspatcher();
	}
}


