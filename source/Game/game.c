//#include "stdio.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "stage.h"
#include "block.h"

//volatile stage cur_stage;

void game_run() {
	int physicalWidth = 800;
	int physicalHeight = 600;
	int virtualWidth = 800;
	int virtualHeight = 600;
	int speed = 0;
	// Background
	framebf_init(physicalWidth, physicalHeight, virtualWidth, virtualHeight);
	setBGcolor(physicalWidth, physicalHeight, 0x00); // set BG to white

	// Bricks
//	struct Block blocks[][];
	// Brick layout
	int block_layout[][2] = {0};
//	draw_map(block_layout);
//	uart_puts("\layout_index = ");
//	uart_dec(block_layout[0][0]);
//	uart_puts("\n");
//	uart_dec(block_layout[0][1]);
//		uart_puts("\n");

//	struct Block block = {block_layout[0][0], block_layout[0][1], 23, 83};
//	remove_block(&block);

//	draw_ball(&newBall2);
//	int inputCountDown=50;
//	char inputCharacter='\0';
//	while(1){
//
//		// Debouncing
//		char input=uart_getc();
//		if(input!='\0' && inputCharacter=='\0'){
//			inputCharacter=input;
//		}
//		if(inputCharacter!='\0'){
//			inputCountDown--;
//			if(inputCountDown==0){
//				uart_sendc(inputCharacter);
//				inputCharacter='\0';
//				uart_puts("\n");
//				inputCountDown=50;
//			}
//		}
//
////		setBGcolor(physicalWidth, physicalHeight, 0x0000); // set BG to white
//		move_ball(&newBall);
//		move_ball(&newBall2);
//		wait_msec(2000);
//	}
	stage cur_stage = MENU;
	stage option = GAME;
	while(1){
		switch(cur_stage) {
			case MENU: {
				menu_stage(&option, &cur_stage);
				break;
			}
			case SETTING: {
				setting_stage(&speed, &cur_stage);
				break;
			}
			case GAME: {
				game_stage(&cur_stage, block_layout);
				break;
			}
			case RESULT:{
				result_stage(&option, &cur_stage, 10, 5);
				break;
			}
		}
	}
}


