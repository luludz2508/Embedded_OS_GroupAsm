#include "../uart.h"
#include "../mbox.h"
#include "../framebf.h"
#include "image.h"

void display_image(){
	framebf_init(800, 600, 800, 600);
	drawImage();
}

void display_background() {
	framebf_init(800, 600, 800, 600);
    drawBackground();
}