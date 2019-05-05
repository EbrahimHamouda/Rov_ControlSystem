/*
 * stringaddPlus.c
 *
 * Created: 5/4/2019 9:55:15 AM
 *  Author: ebrah
 */ 
#include <common/ebra_common.h>

void StrAdd(uint8* dest,uint8* src,uint16 Pos,uint8 N)
{
	uint8 i=0;
	for (i=0;i<N;i++)
	{
		dest[Pos++] = src[i];
	}
}