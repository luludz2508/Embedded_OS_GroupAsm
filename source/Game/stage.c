#include "stage.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "block.h"

void menu_stage(stage *option, stage *main) {
	draw_button(100,START);
	draw_button(300,OPTIONS);
	int redraw = 1;
	while(1){
		char key = uart_getc();
		if(key == 'w' && *option == SETTING){
			*option = GAME;
			redraw = 1;
		} else if(key == 's' && *option == GAME) {
			redraw = 1;
			*option = SETTING;
		} else if(key == '\n'){
			*main = *option;
			setBGcolor(1024,768,0x0);
			return;
		}
		if(redraw){
			if(*option == SETTING){
				draw_arrow(114,100,1);
				draw_arrow(114,300,0);
			} else {
				draw_arrow(114,100,0);
				draw_arrow(114,300,1);
			}
			redraw = 0;
		}
	}

}

void setting_stage(stage *main) {
	draw_button(40,SELECT);
	draw_button(210,LEVELS);
	draw_button(380, ABOUT);
	draw_button(550, HOME);
	int redraw = 1;
	int setting = 0;
	int stage_idx[4] = {PLAYER,DIFF,HOWTO,MENU};
	while(1){
		char key = uart_getc();
		if(key == 'w' && setting > 0){
			//erase option pointer
			draw_arrow(135,setting*170+40,1);
			setting-=1;
			redraw = 1;
		}else if(key == 's' && setting < 3){
			draw_arrow(135,setting*170+40,1);
			setting+=1;
			redraw = 1;
		}else if(key == '\n'){
			*main = stage_idx[setting];
			setBGcolor(1024,768,0x0);
			return;
		}

		if(redraw){
			draw_arrow(135,setting*170+40,0);
			redraw = 0;
		}
	}
}

void player_stage(int *mode, stage *main) {
	draw_button(100,ONE);
	draw_button(300,MULTI);
	int redraw = 1;
	while(1){
		char key = uart_getc();
		if(key == 'w' && *mode == 1){
			*mode = 0;
			redraw = 1;
		} else if(key == 's' && *mode == 0) {
			redraw = 1;
			*mode = 1;
		} else if(key == '\n'){
			*main = SETTING;
			setBGcolor(1024,768,0x0);
			return;
		}
		if(redraw){
			if(*mode == 1){
				draw_arrow(75,100,1);
				draw_arrow(75,300,0);
			} else {
				draw_arrow(75,100,0);
				draw_arrow(75,300,1);
			}
			redraw = 0;
		}
	}
}

void diff_stage(int *diff, stage *main) {
	draw_button(100,EASY);
	draw_button(300,HARD);
	int redraw = 1;
	while(1){
		char key = uart_getc();
		if(key == 'w' && *diff == 1){
			*diff = 0;
			redraw = 1;
		} else if(key == 's' && *diff == 0) {
			redraw = 1;
			*diff = 1;
		} else if(key == '\n'){
			*main = SETTING;
			setBGcolor(1024,768,0x0);
			return;
		}
		if(redraw){
			if(*diff == 1){
				draw_arrow(141,100,1);
				draw_arrow(141,300,0);
			} else {
				draw_arrow(141,100,0);
				draw_arrow(141,300,1);
			}
			redraw = 0;
		}
	}
}

void howto_stage(stage *main) {
	draw_button(300,BACK);
	draw_arrow(135,300,0);
	while(1){
		char key = uart_getc();
		if(key == '\n'){
			*main = SETTING;
			setBGcolor(1024,768,0x0);
			return;
		}
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
		setBGcolor(1024,768,0x0);
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
						setBGcolor(1024,768,0x0);
						return;
					}
					case 27: {
						*main = PAUSE;
						setBGcolor(1024,768,0x0);
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
	int redraw = 1;
	while(1) {
		char key = uart_getc();
		if(key == 'w' && *option == MENU){
			redraw = 1;
			*option = GAME;
		}else if(key == 's' && *option == GAME){
			redraw = 1;
			*option = MENU;
		}else if(key == '\n'){
			*main = *option;
			*option = GAME;
			setBGcolor(1024,768,0x0);
			return;
		}

		if(redraw){
			redraw = 0;
			if(*option == MENU){
				draw_arrow(135,500,0);
				draw_arrow(135,300,1);
			} else {
				draw_arrow(135,500,1);
				draw_arrow(135,300,0);
			}
		}
	}
}

void pause_stage(stage *option, stage *main){
	draw_button(100,RESUME);
	draw_button(300,EXIT);
	int redraw = 1;
	while(1) {
		char key = uart_getc();
		if(key == 'w' && *option == MENU){
			redraw = 1;
			*option = GAME;
		}else if(key == 's' && *option == GAME){
			redraw = 1;
			*option = MENU;
		}else if(key == '\n'){
			*main = *option;
			*option = GAME;
			setBGcolor(1024,768,0x0);
			return;
		}

		if(redraw){
			redraw = 0;
			if(*option == MENU){
				draw_arrow(135,300,0);
				draw_arrow(135,100,1);
			} else {
				draw_arrow(135,300,1);
				draw_arrow(135,100,0);
			}
		}
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
