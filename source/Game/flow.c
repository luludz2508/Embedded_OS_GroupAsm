#include "../uart.h"
#include "../framebf.h"
#include "flow.h"


void menu_stage(stage *option, stage *main) {
	char key = uart_getc();
	if(key == 'w' && *option == SETTING)
		*option = GAME;
	else if(key == 's' && *option == GAME)
		*option = SETTING;
	else if(key == '\n'){
		*main = *option;
		clear_fb(800,600);
		return;
	}
	if(*option == SETTING){
		drawChar_lower(200, 200, 0x00FFFF00, 14);
		drawChar_lower(200, 100, 0x0, 14);
		drawString(250, 100, 0x00FFFFFF, "play game");
		drawString(250, 200, 0x00FFFF00, "setting");
	} else {
		drawChar_lower(200, 100, 0x00FFFF00, 14);
		drawChar_lower(200, 200, 0x0, 14);
		drawString(250, 100, 0x00FFFF00, "play game");
		drawString(250, 200, 0x00FFFFFF, "setting");
	}
}

void setting_stage(int *speed, stage *main) {
	drawString(250, 100, 0x00FFFFFF, "speed");
	char key = uart_getc();
		if(key == 'w' && *speed > 0){
			//erase option pointer
			drawChar_lower(200, (*speed+2)*100, 0x0, 14);
			*speed-=1;
		}else if(key == 's' && *speed < 2){
			drawChar_lower(200, (*speed+2)*100, 0x0, 14);
			*speed+=1;
		}else if(key == '\n'){
			*main = MENU;
			clear_fb(800,600);
			return;
		}
		//draw option pointer
		drawChar_lower(200, (*speed+2)*100, 0x00FFFF00, 14);
		if(*speed == 0){
			drawString(250, 200, 0x00FFFF00, "easy");
			drawString(250, 300, 0x00FFFFFF, "medium");
			drawString(250, 400, 0x00FFFFFF, "hard");
		} else if (*speed == 1){
			drawString(250, 200, 0x00FFFFFF, "easy");
			drawString(250, 300, 0x00FFFF00, "medium");
			drawString(250, 400, 0x00FFFFFF, "hard");
		} else {
			drawString(250, 200, 0x00FFFFFF, "easy");
			drawString(250, 300, 0x00FFFFFF, "medium");
			drawString(250, 400, 0x00FFFF00, "hard");
		}
}

void clear_fb(int w, int h) {
	for (int y = 0; y < h; y++ )
		for (int x = 0; x <= w; x++) {
			drawPixelARGB32(x, y, 0x0);
		}
}
