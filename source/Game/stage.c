#include "stage.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "block.h"
#include "paddle.h"
#include "background_image.h"

void toast_winner(int winner);
void draw_streaks();

// [block index][0:x, 1:y]
int block_layout[64][2] = {{0}};

// Ball: x, y, radius, speed, angle, current player
struct Ball ball1 = {100, 100, 9, 0.5, 90, 0, 'A'};
struct Ball ball2 = {700, 100, 9, 0.5, 120, 0, 'B'};
 
// player name, x, y, width, height, speed, score
struct Paddle left_paddle = {'A', 34, 90, 20, 90, 25, 0};
struct Paddle right_paddle = {'B', 989, 100, 20, 90, 25, 0};

char input, key_down_A, key_down_B;
int count_loop_A = 0;
int count_loop_B = 0;

void draw_streaks() {
	for (int y = 0; y < 45; y++ )
		for (int x = 0; x < 28; x++){
			if (flame[y*28+x] != 0x00ffffff){
		    	drawPixelARGB32(x+200, y+10, flame[y*28+x]); //playerA
		    	drawPixelARGB32(x+800, y+10, flame[y*28+x]); //playerB
			}
		}
}

void count_down() {
	// draw background image
	for (int y = 0 ; y < 768 ; y++){
		for (int x = 0 ; x < 1024 ; x++ ){
			drawPixelARGB32(x, y, background_img[y*1024+x]);
		}
	}
	//Paddles
	draw_paddle(&left_paddle);
	draw_paddle(&right_paddle);
	//score
	draw_nums(right_paddle.score, 700, 20, 0);
	draw_nums(left_paddle.score, 100, 20, 0);
	//streak
	draw_streaks();
	draw_num(ball1.streak, 240, 20, 0);
	draw_num(ball2.streak, 840, 20, 0);
	//frame
	draw_frame(4);

	for(int c = 2; c >= 0; c--) {
		int offset = c*197;
		for (int y = 0 ; y < 230 ; y++)
			for (int x = offset ; x < offset+197 ; x++ ){
				if (count[y*591+x] == 0)
					drawPixelARGB32(400+x-offset, 270+y, background_img[(270+y)*1024+(400+x-offset)]);
				else drawPixelARGB32(400+x-offset, 270+y, count[y*591+x]);
			}
		wait_msec(500000);
	}
}

void menu_stage(stage *option, stage *main, int *diff) {
    // draw background image
    for (int y = 0 ; y < 768 ; y++){
        for (int x = 0 ; x < 1024 ; x++ ){
            drawPixelARGB32(x, y, background_img[y*1024+x]);
        }
    }
	draw_button(100,START);
	draw_button(300,OPTIONS);

	if(*option == SETTING){
		draw_arrow(114,100,1,1);
		draw_arrow(114,300,0,1);
	} else {
		draw_arrow(114,100,0,1);
		draw_arrow(114,300,1,1);
	}

	while(1){
		char key = uart_getc();
		if (key == 'w' && *option == SETTING) {
			*option = GAME;
		} else if (key == 's' && *option == GAME) {
			*option = SETTING;
		} else if (key == '\n') {
			// reset block layout
			init_blocks(block_layout);
			// Reset ball
			init_ball(&ball1, &ball2, *diff);
			// Reset paddles
			init_paddles(&left_paddle, &right_paddle);
			*main = *option;
			if (*option == GAME)
				count_down();
			return;
		}

		if(*option == SETTING){
			draw_arrow(114,100,1,1);
			draw_arrow(114,300,0,1);
		} else {
			draw_arrow(114,100,0,1);
			draw_arrow(114,300,1,1);
		}

	}

}

void setting_stage(stage *main) {
    // draw background image
    for (int y = 0 ; y < 768 ; y++){
        for (int x = 0 ; x < 1024 ; x++ ){
            drawPixelARGB32(x, y, background_img[y*1024+x]);
        }
    }
	int setting = 0;
	int stage_idx[4] = {PLAYER,DIFF,HOWTO,MENU};

	draw_button(40,SELECT);
	draw_button(210,LEVELS);
	draw_button(380, ABOUT);
	draw_button(550, HOME);
	draw_arrow(135,setting*170+40,0,2);

	while(1) {
		char key = uart_getc();
		if (key == 'w' && setting > 0) {
			//erase option pointer
			draw_arrow(135,setting*170+40,1,2);
			setting-=1;
		} else if (key == 's' && setting < 3) {
			draw_arrow(135,setting*170+40,1,2);
			setting+=1;
		} else if (key == '\n') {
			*main = stage_idx[setting];
			return;
		}
		draw_arrow(135,setting*170+40,0,2);
	}
}

void player_stage(int *mode, stage *main) {
    // draw background image
    for (int y = 0 ; y < 768 ; y++){
        for (int x = 0 ; x < 1024 ; x++ ){
            drawPixelARGB32(x, y, background_img[y*1024+x]);
        }
    }
	draw_button(100,ONE);
	draw_button(300,MULTI);
	if(*mode == 1){
		draw_arrow(75,100,1,0);
		draw_arrow(75,300,0,0);
	} else {
		draw_arrow(75,100,0,0);
		draw_arrow(75,300,1,0);
	}
	while (1) {
		char key = uart_getc();
		if (key == 'w' && *mode == 1) {
			*mode = 0;
		} else if (key == 's' && *mode == 0) {
			*mode = 1;
		} else if (key == '\n') {
			*main = SETTING;
			return;
		}
		if(*mode == 1){
			draw_arrow(75,100,1,0);
			draw_arrow(75,300,0,0);
		} else {
			draw_arrow(75,100,0,0);
			draw_arrow(75,300,1,0);
		}
	}
}

void diff_stage(int *diff, stage *main) {
    // draw background image
    for (int y = 0 ; y < 768 ; y++){
        for (int x = 0 ; x < 1024 ; x++ ){
            drawPixelARGB32(x, y, background_img[y*1024+x]);
        }
    }
	draw_button(100,EASY);
	draw_button(300,HARD);
	if(*diff == 1){
		draw_arrow(141,100,1,1);
		draw_arrow(141,300,0,1);
	} else {
		draw_arrow(141,100,0,1);
		draw_arrow(141,300,1,1);
	}
	while (1) {
		char key = uart_getc();
		if (key == 'w' && *diff == 1) {
			*diff = 0;
		} else if (key == 's' && *diff == 0) {
			*diff = 1;
		} else if (key == '\n') {
			*main = SETTING;
			return;
		}
		if(*diff == 1){
			draw_arrow(141,100,1,1);
			draw_arrow(141,300,0,1);
		} else {
			draw_arrow(141,100,0,1);
			draw_arrow(141,300,1,1);
		}
	}
}

void howto_stage (stage *main) {
    // draw background image
    for (int y = 0 ; y < 768 ; y++){
        for (int x = 0 ; x < 1024 ; x++ ){
            drawPixelARGB32(x, y, background_img[y*1024+x]);
        }
    }
	draw_button(300,BACK);
	draw_arrow(135,300,0,0);
	while (1) {
		char key = uart_getc();
		if (key == '\n') {
			*main = SETTING;
			return;
		}
	}
}

void game_stage(stage *main) {
    // Draw map
	draw_map(block_layout);

	while (1) {


		// Get player input
		input = getUart();

		// Player A (left) paddle control
		if (input != '\0' && count_loop_A == 0) {
			if (input == 'w' || input == 's') {
				move_paddle(&left_paddle, input);
				key_down_A = input;
			}
		}

		// Player B (right) paddle control
		if (input != '\0' && count_loop_B == 0) {
			 if (input == 'i' || input == 'k') {
				move_paddle(&right_paddle, input);
				key_down_B = input;
			}
		}

		// Player A button debounce
		if (key_down_A != 0) {
			count_loop_A++;
			// Delay button press
			if (count_loop_A == 10) {
				count_loop_A = 0;
				key_down_A =0;
			}
		}

		// Player B button debounce
		if (key_down_B != 0) {
			 count_loop_B++;
			 // Delay button press
			 if (count_loop_B == 10) {
				 count_loop_B = 0;
				 key_down_B = 0;
			 }
		 }

		// escape key
		if (input == 27) {
			*main = PAUSE;
			return;
		}

		// Check collision for 2 paddles
		check_collision_paddle(&ball1, &left_paddle);
		check_collision_paddle(&ball2, &left_paddle);

		check_collision_paddle(&ball1, &right_paddle);
		check_collision_paddle(&ball2, &right_paddle);

		// Draw 2 paddles
		draw_paddle_image(&left_paddle);
        draw_paddle_image(&right_paddle);

		// if one player loses
		if (!check_collision_block(&ball1, block_layout, &left_paddle, &right_paddle) || !check_collision_block(&ball2, block_layout, &left_paddle, &right_paddle)) {
			*main = RESULT;
			return;
		}

		wait_msec(2000);
	}
}

void result_stage(stage *option, stage *main, int *diff) {
    // draw background image
    for (int y = 0 ; y < 768 ; y++){
        for (int x = 0 ; x < 1024 ; x++ ){
            drawPixelARGB32(x, y, background_img[y*1024+x]);
        }
    }
    // draw result
	if(left_paddle.score > right_paddle.score){
		toast_winner(1);
		draw_nums(left_paddle.score,400,200,0);
		draw_nums(right_paddle.score,600,200,0);
	} else if (left_paddle.score == right_paddle.score){
		drawString(550, 100, 0x00FFFFFF, "tie game");
		draw_num(0,400,200,0);
		draw_num(0,600,200,0);
	} else {
		draw_nums(left_paddle.score,400,200,0);
		draw_nums(right_paddle.score,600,200,0);
		toast_winner(2);
	}

	draw_button(300,RESTART);
	draw_button(500,HOME);
	draw_arrow(135,500,1,2);
	draw_arrow(135,300,0,2);

	while (1) {
		char key = uart_getc();
		if (key == 'w' && *option == MENU) {
			*option = GAME;
		}else if (key == 's' && *option == GAME) {
			*option = MENU;
		}else if (key == '\n') {
			// reset block layout
			init_blocks(block_layout);
			// Reset ball
			init_ball(&ball1, &ball2, *diff);
			// Reset paddles
			init_paddles(&left_paddle, &right_paddle);
			*main = *option;
			*option = GAME;
			if(*main == GAME)
				count_down();
			return;
		}

		if (*option == MENU) {
			draw_arrow(135,500,0,2);
			draw_arrow(135,300,1,2);
		} else {
			draw_arrow(135,500,1,2);
			draw_arrow(135,300,0,2);
		}
	}
}

void pause_stage(stage *option, stage *main){
    // draw background image at buttons location
    for (int y = 75 ; y < 750 ; y++){
        for (int x = 135 ; x < 690 ; x++ ){
            drawPixelARGB32(x, y, background_img[y*1024+x]);
        }
    }
	draw_button(100,RESUME);
	draw_button(300,EXIT);
	draw_arrow(135,300,1,1);
	draw_arrow(135,100,0,1);

	while (1) {
		char key = uart_getc();
		if (key == 'w' && *option == MENU) {
			*option = GAME;
		} else if (key == 's' && *option == GAME) {
			*option = MENU;
		} else if (key == '\n') {
			for (int y = 100 ; y < 450 ; y++){
				for (int x = 135 ; x < 690 ; x++ ){
					drawPixelARGB32(x, y, background_img[y*1024+x]);
				}
			}
			*main = *option;
			*option = GAME;
			return;
		}
		if(*option == MENU){
			draw_arrow(135,300,0,1);
			draw_arrow(135,100,1,1);
		} else {
			draw_arrow(135,300,1,1);
			draw_arrow(135,100,0,1);
		}
	}
}

void draw_arrow(int offsetX, int offsetY, int erase, int arrowIdx) {
	for (int y = 0; y < 130; y++ )
        for (int x = 0; x < 77; x++) {
            if (erase)
                drawPixelARGB32(x+offsetX, y+offsetY, background_img[(y+offsetY)*1024+(x+offsetX)]);
            else {
            	if (arrow[arrowIdx][y*77+x] == 0)
            		drawPixelARGB32(x+offsetX, y+offsetY, background_img[(y+offsetY)*1024+(x+offsetX)]);
            	else drawPixelARGB32(x+offsetX, y+offsetY, arrow[arrowIdx][y*77+x]);
            }
        }
}

void draw_button(int offsetY,int buttonIdx){
	int width=button_width[buttonIdx];
	int offsetX = (1024-width)/2;
	for (int y = 0; y < 130; y++ )
		for (int x = 0; x < width; x++){
			if (button_array[buttonIdx][y*width+x] == 0)
				drawPixelARGB32(x+offsetX, y+offsetY, background_img[(y+offsetY)*1024+(x+offsetX)]);
			else drawPixelARGB32(x+offsetX, y+offsetY, button_array[buttonIdx][y*width+x]);
		}
}

void toast_winner(int winner) {
	if(winner == 1) {
		for (int y = 0; y < 40; y++ )
			for (int x = 0; x < 535; x++){
				if (p1[y*535+x] == 0)
					drawPixelARGB32(x+320, y+100, background_img[(y+100)*1024+(x+320)]);
				else drawPixelARGB32(x+320, y+100, 0x00ffffff);
			}
	} else {
		for (int y = 0; y < 40; y++ )
			for (int x = 0; x < 535; x++){
				if (p2[y*535+x] == 0)
					drawPixelARGB32(x+320, y+100, background_img[(y+100)*1024+(x+320)]);
				else drawPixelARGB32(x+320, y+100, 0x00ffffff);
			}
	}
	//draw trophy
	for (int y = 0 ; y < 85 ; y++){
		for (int x = 0 ; x <80 ; x++ ){
			if (trophy[y*80+x] == 0x00ffffff)
				drawPixelARGB32(x+190, y+80, background_img[(y+80)*1024+(x+190)]);
			else drawPixelARGB32(x+190, y+80, trophy[y*80+x]);
		}
	}
}
