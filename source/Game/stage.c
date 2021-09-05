#include "stage.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "block.h"

void menu_stage(stage *option, stage *main) {
	draw_button(100,START);
	draw_button(300,OPTIONS);
	while(1){
		char key = uart_getc();
		if(key == 'w' && *option == SETTING)
			*option = GAME;
		else if(key == 's' && *option == GAME)
			*option = SETTING;
		else if(key == '\n'){
			*main = *option;
			clear_fb();
			return;
		}
		if(*option == SETTING){
			draw_arrow(60,100,1);
			draw_arrow(60,300,0);
		} else {
			draw_arrow(60,100,0);
			draw_arrow(60,300,1);
		}
	}

}

void setting_stage(int *speed, stage *main) {
	draw_button(40,SELECT);
	draw_button(210,LEVELS);
	draw_button(380, INSTRUCTION);
	draw_button(550, HOME);
	while(1){
		char key = uart_getc();
		if(key == 'w' && *speed > 0){
			//erase option pointer
			draw_arrow(135,*speed*170+40,1);
			*speed-=1;
		}else if(key == 's' && *speed < 3){
			draw_arrow(135,*speed*170+40,1);
			*speed+=1;
		}else if(key == '\n'){
			*main = MENU;
			clear_fb();
			return;
		}
		//draw option pointer
		draw_arrow(135,*speed*170+40,0);
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

void count_down(int layout[][2]) {
	for(int c = 0; c < 3; c++) {
		draw_map(layout);
		drawChar_upper(380, 170, 0x00FFFF00, c);
		wait_msec(10000);
		clear_fb();
	}
	draw_map(layout);
}

void game_stage(stage *main, int layout[][2]) {
	count_down(layout);
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
						clear_fb();
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
	draw_button(300,RESTART);
	draw_button(500,HOME);

	while(1) {
		char key = uart_getc();
		if(key == 'w' && *option == MENU)
			*option = GAME;
		else if(key == 's' && *option == GAME)
			*option = MENU;
		else if(key == '\n'){
			*main = *option;
			*option = GAME;
			clear_fb();
			return;
		}
		if(*option == MENU){
			draw_arrow(135,500,0);
			draw_arrow(135,300,1);
		} else {
			draw_arrow(135,500,1);
			draw_arrow(135,300,0);
		}
	}

}

void clear_fb() {
	for (int y = 0; y < 768; y++ )
		for (int x = 0; x <= 1024; x++) {
			drawPixelARGB32(x, y, 0x0);
		}
}

void draw_arrow(int offsetX, int offsetY, int erase) {
	for (int y = 0; y < 130; y++ )
			for (int x = 0; x < 77; x++) {
				if (erase == 1)
					drawPixelARGB32(x+offsetX, y+offsetY, 0x0);
				else drawPixelARGB32(x+offsetX, y+offsetY, arrow[y*77+x]);
			}
}

void draw_button(int offsetY,int buttonIdx){
	int width=button_width[buttonIdx];
	int offsetX = (1024-width)/2;
	for (int y = 0; y < 130; y++ )
		for (int x = 0; x < width; x++)
			drawPixelARGB32(x+offsetX, y+offsetY, button_array[buttonIdx][y*width+x]);
}
