#include "../uart.h"
#include "../mbox.h"
#include "../framebf.h"
#include "image.h"

// set up serial console
void image(){
	framebf_init(800, 600, 800, 600);
	drawBackground();
}
