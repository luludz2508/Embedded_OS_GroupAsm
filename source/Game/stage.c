#include "stage.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"

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
void wait_msec(unsigned int n)
{
    register unsigned long f, t, r;
    // Get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate expire value for counter
    t += ((f/1000)*n)/1000;
    do {
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    } while(r < t);

}
void game_stage(stage *main) {
	struct Ball newBall = {50, 50, 13, 25, 25, 2, 70};
	struct Ball newBall2 = {400, 300, 13, 25, 25, 2, 10};
	int inputCountDown=50;
	char inputCharacter='\0';
	while(1){
		// Debouncing
		char input=uart_getc();
		if(input!='\0' && inputCharacter=='\0'){
			inputCharacter=input;
		}
		if(inputCharacter!='\0'){
			inputCountDown--;
			if(inputCountDown==0){
				switch(inputCharacter) {
					case '\n': {
						*main = RESULT;
						clear_fb(800,600);
						return;
					}
					default:{
						uart_sendc(inputCharacter);
						uart_sendc('\n');
					}
				}
				inputCharacter='\0';
				inputCountDown=50;
			}
		}

//		setBGcolor(physicalWidth, physicalHeight, 0x0000); // set BG to white
		move_ball(&newBall);
		move_ball(&newBall2);
		wait_msec(2000);
	}
}

void result_stage(stage *option, stage *main, int score1, int score2) {
	if(score1 > score2)
		drawString(250, 100, 0x00FFFF00, "player A wins");
	else if (score1 == score2)
		drawString(250, 100, 0x00FFFFFF, "tie game");
	else drawString(250, 100, 0x00FFFF00, "player B wins");
	drawString(250, 200, 0x00FFFFFF, "A  - B");

	char key = uart_getc();
	if(key == 'w' && *option == MENU)
		*option = GAME;
	else if(key == 's' && *option == GAME)
		*option = MENU;
	else if(key == '\n'){
		*main = *option;
		*option = GAME;
		clear_fb(800,600);
		return;
	}
	if(*option == MENU){
		drawChar_lower(200, 400, 0x00FFFF00, 14);
		drawChar_lower(200, 300, 0x0, 14);
		drawString(250, 300, 0x00FFFFFF, "play again");
		drawString(250, 400, 0x00FFFF00, "menu");
	} else {
		drawChar_lower(200, 300, 0x00FFFF00, 14);
		drawChar_lower(200, 400, 0x0, 14);
		drawString(250, 300, 0x00FFFF00, "play again");
		drawString(250, 400, 0x00FFFFFF, "menu");
	}
}

void clear_fb(int w, int h) {
	for (int y = 0; y < h; y++ )
		for (int x = 0; x <= w; x++) {
			drawPixelARGB32(x, y, 0x0);
		}
}
