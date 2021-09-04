//#include "stdio.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "stage.h"

struct Ball newBall = {50, 50, 13, 25, 25, 2, 70};
struct Ball newBall2 = {400, 300, 13, 25, 25, 2, 10};
//volatile stage cur_stage;

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
	int inputCountDown=50;
	char inputCharacter='\0';
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
				draw_map(block_layout);
				game_stage(&cur_stage);
				break;
			}
			case RESULT:{
				result_stage(&option, &cur_stage, 10, 5);
				break;
			}
		}
	}
}


