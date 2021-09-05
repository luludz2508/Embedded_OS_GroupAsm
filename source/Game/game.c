//#include "stdio.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "stage.h"
#include "block.h"

//volatile stage cur_stage;

void game_run() {
	int physicalWidth = 1024;
	int physicalHeight = 768;
	int virtualWidth = 1024;
	int virtualHeight = 768;
	// Background
	framebf_init(physicalWidth, physicalHeight, virtualWidth, virtualHeight);
	setBGcolor(physicalWidth, physicalHeight, 0x00); // set BG to white


	int block_layout[][2] = {0};
	stage cur_stage = MENU;
	stage option = GAME;
	int mode = 0, diff = 0;
	while(1){
		switch(cur_stage) {
			case MENU: {
				menu_stage(&option, &cur_stage);
				break;
			}
			case SETTING: {
				setting_stage(&cur_stage);
				break;
			}
			case GAME: {
				game_stage(&cur_stage, block_layout);
				break;
			}
			case PLAYER: {
				player_stage(&mode, &cur_stage);
				break;
			}
			case DIFF: {
				diff_stage(&diff, &cur_stage);
				break;
			}
			case HOWTO: {
				howto_stage(&cur_stage);
				break;
			}
			case RESULT:{
				result_stage(&option, &cur_stage, 10, 5);
				break;
			}
			case PAUSE: {
				pause_stage(&option, &cur_stage);
				break;
			}
		}
	}
}


