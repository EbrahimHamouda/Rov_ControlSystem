/*
* Message_Sensors.c
*
* Created: 5/4/2019 7:25:07 PM
*  Author: ebrah
*/
#include <common/ebra_common.h>
#include <string.h>
#include <stdlib.h>
#include "NewStrings.h"
#include "Message_Sensors.h"
#include "DHT11.h"
#include "RTCDS1307.h"

// Gloable variables
uint8 Sensors_Message[1000];
uint16 Message_length;

//extern variables
extern uint16 UltraSonic_arr[2];
extern rtc_t RTC1;
extern DHT11_t DHT11_1;

// static variables
static const uint8 beginNend_message[] ="****************************************";
static const uint8 Word_date[]="Date:";
static const uint8 Word_time[]="Time:";
static const uint8 Word_Temperture[]="Temperture:";
static const uint8 Word_Humedity[]="Humedity:";
static const uint8 Word_HerizontalDistance[]="Horizontal_Distance:";
static const uint8 Word_VerticalDistance[]="Vertical_Distance:";

static uint8 Date_pos;
static uint8 Time_pos;
static uint8 Temp_pos;
static uint8 Humd_pos;
static uint8 HDist_pos;
static uint8 VDist_pos;
static uint8 Date_Buffer[]={"NN/NN/NNNN"};// 2/5/2019
static uint8 Time_Buffer[]={"SS:MM:HH"};//
static uint8 Temp_Buffer[]={"NNC"};
static uint8 Humd_Buffer[]={"NN%"};
static uint8 Vdist_Buffer[]={"NNNcm"};
static uint8 Hdist_Buffer[]={"NNNcm"};



void Message_init()
{
	strcat(Sensors_Message,beginNend_message);
	
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	strcat(Sensors_Message,Word_date);
	Date_pos = strlen(Sensors_Message);
	strcat(Sensors_Message,Date_Buffer);
	
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	strcat(Sensors_Message,Word_time);
	Time_pos = strlen(Sensors_Message);
	strcat(Sensors_Message,Time_Buffer);
	
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	strcat(Sensors_Message,Word_Temperture);
	Temp_pos = strlen(Sensors_Message);
	strcat(Sensors_Message,Temp_Buffer);
	
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	strcat(Sensors_Message,Word_Humedity);
	Humd_pos = strlen(Sensors_Message);
	strcat(Sensors_Message,Humd_Buffer);
	

	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	strcat(Sensors_Message,Word_VerticalDistance);
	VDist_pos= strlen(Sensors_Message);
	strcat(Sensors_Message,Vdist_Buffer);
	
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	strcat(Sensors_Message,Word_HerizontalDistance);
	HDist_pos= strlen(Sensors_Message);
	strcat(Sensors_Message,Hdist_Buffer);
	
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	strcat(Sensors_Message,beginNend_message);
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	Sensors_Message[strlen(Sensors_Message)]=0x0D; // add new line
	Message_length = strlen(Sensors_Message);
}

void Conver_Rtc()
{

	Sensors_Message[Time_pos++]=(((RTC1.sec)>>4)&0x0f)+'0';
	Sensors_Message[Time_pos++]=((RTC1.sec)&0x0f)+'0';
	Time_pos++; // for ( : )
	Sensors_Message[Time_pos++]=(((RTC1.min)>>4)&0x0f)+'0';
	Sensors_Message[Time_pos++]=((RTC1.min)&0x0f)+'0';
	Time_pos++; // for ( : )
	Sensors_Message[Time_pos++]=(((RTC1.hour)>>4)&0x0f)+'0';
	Sensors_Message[Time_pos++]=((RTC1.hour)&0x0f)+'0';
	Time_pos -= 8;
	
	Sensors_Message[Date_pos++]=(((RTC1.date)>>4)&0x0f)+'0';
	Sensors_Message[Date_pos++]=((RTC1.date)&0x0f)+'0';
	Date_pos++; // for ( / )
	Sensors_Message[Date_pos++]=(((RTC1.month)>>4)&0x0f)+'0';
	Sensors_Message[Date_pos++]=((RTC1.month)&0x0f)+'0';
	Date_pos++; // for ( / )
	Sensors_Message[Date_pos++] = '2';
	Sensors_Message[Date_pos++] = '0'; 
	Sensors_Message[Date_pos++]=(((RTC1.year)>>4)&0x0f)+'0';
	Sensors_Message[Date_pos++]=((RTC1.year)&0x0f)+'0';
	Date_pos -= 10;	
}
void Message_update()
{
	// ultra sonics
	itoa(UltraSonic_arr[0],Hdist_Buffer,10);
	itoa(UltraSonic_arr[1],Vdist_Buffer,10);
	if (UltraSonic_arr[0]<=99)
	{
		StrAdd(Sensors_Message,Hdist_Buffer,HDist_pos,2);
		Sensors_Message[HDist_pos+2]=0;
	}
	else
	{
		StrAdd(Sensors_Message,Hdist_Buffer,HDist_pos,3);
	}

	if (UltraSonic_arr[1]<=99)
	{
		StrAdd(Sensors_Message,Vdist_Buffer,VDist_pos,2);
		Sensors_Message[VDist_pos+2]=0;
	}
	else
	{
		StrAdd(Sensors_Message,Vdist_Buffer,VDist_pos,3);
	}
	// dht11
	itoa(DHT11_1.I_Temp,Temp_Buffer,10);
	itoa(DHT11_1.I_RH,Humd_Buffer,10);
	StrAdd(Sensors_Message,Temp_Buffer,Temp_pos,2);
	StrAdd(Sensors_Message,Humd_Buffer,Humd_pos,2);
	Conver_Rtc();
}