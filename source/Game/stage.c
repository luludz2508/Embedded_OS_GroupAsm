#include "stage.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "block.h"
#include "paddle.h"

struct Ball new_ball = {50, 250, 13, 2, 0};
//	struct Ball new_ball2 = {400, 300, 13, 25, 25, 10, 10};

struct Paddle left_paddle = {'A', 20, 45, 20, 90, 50};
struct Paddle right_paddle = {'B', 1004, 45, 20, 90, 50};

char input, key_down_A, key_down_B;
int count_loop_A = 0;
int count_loop_B = 0;

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
				draw_arrow(114,100,1,1);
				draw_arrow(114,300,0,1);
			} else {
				draw_arrow(114,100,0,1);
				draw_arrow(114,300,1,1);
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
			draw_arrow(135,setting*170+40,1,2);
			setting-=1;
			redraw = 1;
		}else if(key == 's' && setting < 3){
			draw_arrow(135,setting*170+40,1,2);
			setting+=1;
			redraw = 1;
		}else if(key == '\n'){
			*main = stage_idx[setting];
			setBGcolor(1024,768,0x0);
			return;
		}

		if(redraw){
			draw_arrow(135,setting*170+40,0,2);
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
				draw_arrow(75,100,1,0);
				draw_arrow(75,300,0,0);
			} else {
				draw_arrow(75,100,0,0);
				draw_arrow(75,300,1,0);
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
				draw_arrow(141,100,1,1);
				draw_arrow(141,300,0,1);
			} else {
				draw_arrow(141,100,0,1);
				draw_arrow(141,300,1,1);
			}
			redraw = 0;
		}
	}
}

void howto_stage(stage *main) {
	draw_button(300,BACK);
	draw_arrow(135,300,0,0);
	while(1){
		char key = uart_getc();
		if(key == '\n'){
			*main = SETTING;
			setBGcolor(1024,768,0x0);
			return;
		}
	}
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

void game_stage(stage *main, int block_layout[][2]) {

	count_down(block_layout);
//	struct Ball new_ball = {50, 50, 13, 25, 25, 0.5, 70};
//	struct Ball new_ball2 = {400, 300, 13, 25, 25, 2, 10};
//	struct Paddle left_paddle = {'A', 20, 45, 20, 90, 50};
//	struct Paddle right_paddle = {'B', 1004, 45, 20, 90, 50};

//	char input, key_down_A, key_down_B;
//	int count_loop_A = 0;
//	int count_loop_B = 0;

	// Draw map
	draw_map(block_layout);

	// Balls
	draw_ball(&new_ball); // ball 1
    //	draw_ball(&new_ball2); // ball 2

	//Paddles
    draw_paddle(&left_paddle);
    draw_paddle(&right_paddle);

	while(1){
		// Get player input
		input = getUart();

		// Player A (right) paddle control
		if(input != '\0' && count_loop_A == 0){
			if (input == 'w' || input == 's'){
				move_paddle(&left_paddle, input);
				key_down_A = input;
			}
		}

		// Player B (left) paddle control
		if(input != '\0' && count_loop_B == 0){
			 if(input == 'i' || input == 'k'){
				move_paddle(&right_paddle, input);
				key_down_B = input;
			}
		}

		// Player A button debounce
		if(key_down_A != 0){
			count_loop_A++;
			// Delay button press
			if (count_loop_A == 20){
				count_loop_A = 0;
				key_down_A =0;
			}
		}

		// Player B button debounce
		if(key_down_B != 0){
			 count_loop_B++;
			 // Delay button press
			 if (count_loop_B == 20){
				 count_loop_B = 0;
				 key_down_B = 0;
			 }
		 }

		if (input == 27) {
			*main = PAUSE;
			setBGcolor(1024,768,0x0);
			return;
		}

		check_collision_paddle1(&new_ball, &left_paddle);
		check_collision_paddle1(&new_ball, &right_paddle);

		// if ball hits walls
		if (!move_ball(&new_ball, block_layout)) {
			*main = RESULT;
			setBGcolor(1024,768,0x0);
			return;
		}
//		move_ball(&new_ball, block_layout);

		wait_msec(5000);
	}
}

void result_stage(stage *option, stage *main, int score1, int score2) {
//	if(score1 > score2)
//		drawString(250, 100, 0x00FFFF00, "player A wins");
//	else if (score1 == score2)
//		drawString(250, 100, 0x00FFFFFF, "tie game");
//	else drawString(250, 100, 0x00FFFF00, "player B wins");
//	drawString(250, 200, 0x00FFFFFF, "A  - B");
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
				draw_arrow(135,500,0,2);
				draw_arrow(135,300,1,2);
			} else {
				draw_arrow(135,500,1,2);
				draw_arrow(135,300,0,2);
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
				draw_arrow(135,300,0,1);
				draw_arrow(135,100,1,1);
			} else {
				draw_arrow(135,300,1,1);
				draw_arrow(135,100,0,1);
			}
		}
	}
}

void draw_arrow(int offsetX, int offsetY, int erase, int arrowIdx) {
	for (int y = 0; y < 130; y++ )
			for (int x = 0; x < 77; x++) {
				if (erase == 1)
					drawPixelARGB32(x+offsetX, y+offsetY, 0x0);
				else drawPixelARGB32(x+offsetX, y+offsetY, arrow[arrowIdx][y*77+x]);
			}
}

void draw_button(int offsetY,int buttonIdx){
	int width=button_width[buttonIdx];
	int offsetX = (1024-width)/2;
	for (int y = 0; y < 130; y++ )
		for (int x = 0; x < width; x++)
			drawPixelARGB32(x+offsetX, y+offsetY, button_array[buttonIdx][y*width+x]);
}
