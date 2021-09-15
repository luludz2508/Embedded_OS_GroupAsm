#include "../framebf.h"
#include "font.h"

void font(){
	framebf_init(1024,768, 1024, 768);
	setBGcolor(1024,768, 0); // set BG to black
	drawString(50,64, 0x000080ff,"Nguyen Phuoc Nhu Phuc");
	drawString(50,128, 0x00c095ee,"Phan Khanh Ha");
	drawString(50,192, 0x00f88379,"Nguyen Thanh Luan");
	drawString(50,256, 0x00FADA5E,"0123456789");
	drawString(50,320, 0x003fe0d0,"!\"#$%&'()*+,-./:;<=>?@[\\]_{|}~");
}
