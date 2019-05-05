/*
 * built_inBoot.c
 *
 * Created: 4/24/2019 8:24:57 AM
 *  Author: ebrah
 */ 
#include <avr/boot.h>
#include <avr/interrupt.h>


void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;
	uint16_t w;
	sreg = SREG ;
	// enter Critical section
	cli();

	boot_page_erase (page);
	boot_spm_busy_wait ();      // Wait until the memory is erased.

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		// Set up little-endian word.

		w = *buf++;			// first byte
		w += (*buf++) << 8; // second byte

		boot_page_fill (page + i, w); //
	}

	boot_page_write (page);     // Store buffer in flash page.
	boot_spm_busy_wait();       // Wait until the memory is written.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.

	boot_rww_enable ();

	// Re-enable interrupts (if they were ever enabled).

	SREG = sreg;
	sei();
}
