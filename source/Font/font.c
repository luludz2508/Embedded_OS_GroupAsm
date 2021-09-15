#include "framebf.h"
#include "font.h"

void font(){
	framebf_init(1024,768, 1024, 768);
	setBGcolor(1024,768, 0x00000000); // set BG to white
	drawString(50,64, 0x00ffffff,"Phuc Nguyen");
	drawString(50,128, 0x00ffffff,"Ha Phan");
	drawString(50,192, 0x00ffffff,"Luan Nguyen");
}
