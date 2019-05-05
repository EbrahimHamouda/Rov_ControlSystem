/*
* Boot.c
*
* Created: 4/19/2019 11:07:10 AM
*  Author: Ebrahim Hamouda
*/

#include <avr/io.h>
#include <common/ebra_common.h>
#include <util/delay.h>
#define ENABLE_RWW (SPMCR |= (1<<RWWSRE)|(1<<SPMEN))

#define IS_RWW_BLOCKED (SPMCR&(1<<RWWSB)) // RWWSB=1 BLOCKED

#define IS_SPM_BUSY (SPMCR&(1<<SPMEN)) // SPMEN=1 BUSY

#define ENABLE_WORD2BUFFER (SPMCR |= (1<<SPMEN))

#define ENABLE_PAGE_WRITE (SPMCR |= (1<<SPMEN)|(1<<PGWRT))

#define ENABLE_PAGE_ERASE (SPMCR |= (1<<SPMEN)|(1<<PGERS))

static volatile unsigned char 	*r0	=	0x00;
static volatile unsigned char 	*r1	=	0x01;
static volatile unsigned int	*zPointer	=	0x1E;


void Page_Erase(uint8 Page_num)
{
	*zPointer=Page_num;
	ENABLE_PAGE_ERASE;
	_delay_ms(10);
	while(IS_SPM_BUSY);
	ENABLE_RWW;
}

void Fill_buffer(uint8 addrass,uint16 data) // fill temp buffer (cash)
{
	*r0 = (uint8)data;
	*r1 = data>>8;
	*zPointer=(addrass<<1);
	ENABLE_WORD2BUFFER;
	while(IS_SPM_BUSY);
	
}

void page_write(uint8 page_num)
{
	*zPointer =(page_num<<7);
	ENABLE_PAGE_WRITE;
	while(IS_SPM_BUSY);
}