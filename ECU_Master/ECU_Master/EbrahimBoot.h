/*
 * EbrahimBoot.h
 *
 * Created: 4/19/2019 2:30:18 PM
 *  Author: ebrah
 */ 


#ifndef EBRAHIMBOOT_H_
#define EBRAHIMBOOT_H_

void Page_Erase(uint8 Page_num);
void Fill_buffer(uint8 addrass,uint16 data) ;// fill temp buffer (cash)
void page_write(uint8 page_num);


#endif /* EBRAHIMBOOT_H_ */