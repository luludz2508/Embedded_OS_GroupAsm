#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "image.h"

// set up serial console
void image(){
	uart_init();

	// example code lab 5
	// mailbox data buffer: Read ARM frequency
	mbox[0] = 8*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)

	mbox[2] = 0x00048003; // Set physical screensize
	mbox[3] = 8; // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0; // REQUEST CODE = 0
	mbox[5] = 800; // Physical width
	mbox[6] = 600; // Physical height

	mbox[7] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");
		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");
		uart_puts("DATA: ARM clock rate = ");
		uart_dec(mbox[6]);
		uart_puts("DATA: ARM clock rate = ");
		uart_dec(mbox[11]);
		uart_puts("\n");
	} else {
		uart_puts("Unable to query!\n");
	}

	for (int y = 0; y < 600; y++) {
		for (int x = 0; x < 800; x++) {
			drawPixelARGB32(x, y, background[y*800 + x]);
		}
	}

	for (int y = 200; y < 400; y++) {
		for (int x = 250; x < 550; x++) {
			drawPixelARGB32(x, y, dog_and_cat[(y - 200) * 300 + (x - 250)]);
		}
	}

	char c = 0;
	int current_y = 0;
	int step = 40;

	uart_dec((int)(cos(1.0472) * 1000));

	while(1) {
		c = uart_getc();
		uart_sendc(c);
		uart_puts("\n");
		if (c == 's' && (current_y <= 1400 - 600 - step)) {
			current_y += step;
			uart_dec(current_y);
			for (int y = 0; y < 600; y++) {
				for (int x = 0; x < 800; x++) {
					drawPixelARGB32(x, y, background[(y + current_y)*800 + x]);
				}
			}
		}

		if (c == 'w' && (current_y >= step)) {
			current_y -= step;
			uart_dec(current_y);
			for (int y = 0; y < 600; y++) {
				for (int x = 0; x < 800; x++) {
					drawPixelARGB32(x, y, background[(y + current_y)*800 + x]);
				}
			}
		}

		for (int y = 200; y < 400; y++) {
			for (int x = 250; x < 550; x++) {
				drawPixelARGB32(x, y, dog_and_cat[(y - 200) * 300 + (x - 250)]);
			}
		}
	}
}