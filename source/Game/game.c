//#include "stdio.h"
#include "ball.h"
#include "paddle.h"
#include "../framebf.h"
#include "../uart.h"
#include "stage.h"
#include "block.h"
#include "background.h"

#ifndef BRICK_WIDTH
#define BRICK_WIDTH 23
#endif
#ifndef BRICK_HEIGHT
#define BRICK_HEIGHT 83
#endif
#ifndef MAX_BLOCKS
#define MAX_BLOCKS 64
#endif

int check_collision_edge(struct Ball *ball, struct Paddle *padA, struct Paddle *padB, int flag);
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

void check_collision_paddle(struct Ball *ball, struct Paddle *pad){
    float dist_x = ball->x - pad->x - pad->width/2;
    float dist_y = ball->y - pad->y - pad->height/2;
    int flag_x = 0, flag_y = 0;

    // check x: ball hit left of block
    if (ball->x < pad->x - pad->width/2) {
    	dist_x = (pad->x ) - ball->x - pad->width/2;
        flag_x = 1;
    }
    // check x: ball hit top or bottom of block
    else if (ball->x >= pad->x - pad-> width/2 && ball->x <= pad->x  + pad->width/2) {
        dist_x = 0;
        flag_x = 2;
    }

    // check x: ball hit right wall of block
    else if (ball->x > pad->x  + pad->width/2) {
    	dist_x = ball->x - (pad->x  + pad->width/2);
    	flag_x = 3;
    }


     // check y: before block
     if (ball->y <= pad->y - pad->height/2) {
    	 dist_y = (pad->y) - ball->y - pad->height/2;
         flag_y = 10;
     }

     // check y: middle of block
     else if (ball->y >= pad->y - pad->height/2&&
         ball->y <= pad->y + pad->height/2) {
         dist_y = 0;
         flag_y = 20;
     }

     // check y: after block
     else if (ball->y > pad->y + pad->height/2) {
    	 dist_y = ball->y - (pad->y + pad->height/2);
         flag_y = 30;

     }
    float width_dist = dist_x*dist_x + dist_y*dist_y;
    int flag = flag_x + flag_y;

    // Check id ball collided to paddle
    if (width_dist <= (float)(ball->radius * ball->radius)) {
        float ratio= (ball->y - pad->y)/(pad->height/2);
        int modify_angle=50;
        uart_puts("\nAngle Before: ");
        uart_dec((int)( ball->angle));

        // Change current player when ball hit paddle
        if (pad->name != ball->current_player){
        	ball->current_player = pad->name;
			// Reset streaks
			ball->streak = 0;
        }

        // ball hit right side of paddle
        if ( flag == 23) {
            ball->x = pad->x + pad->width/2 + ball->radius;
            if (ball->angle<=180){
                    ball->angle = 180 - ball->angle + ratio*modify_angle ;
                }
            else{
                    ball->angle =540 - ball->angle + ratio*modify_angle ;
                }
            uart_puts(" - Angle After: ");
            uart_dec((int)( ball->angle));
        }

        // ball hit left side of paddle
        if ( flag == 21) {
            ball->x = pad->x -pad->width/2-ball->radius;
            if (ball->angle<=180)
                ball->angle = 180 - ball->angle - ratio*modify_angle ;
            else
                ball->angle =540 - ball->angle - ratio*modify_angle ;

            uart_puts(" - Angle After: ");
            uart_dec((int)( ball->angle));
        }

        // ball hit bottom
        if (flag == 32) {
            ball->angle = 360 - ball->angle;
        }

        // ball hit top
        if (flag == 12) {
            ball->angle = 360 - ball->angle;
        }

        if(ball->angle >= 360){
            ball->angle -= 360;
        }

        // change angle if hit top left corner
        if (flag == 11) {
            ball->angle = 225;
        }

        // change angle if hit bottom left corner
        if (flag == 31) {
            ball->angle = 135;
        }

        // change angle if hit top right corner
        if (flag == 13) {
            ball->angle = 315;
        }

        // change angle if hit bottom right corner
        if (flag == 33) {
            ball->angle = 45;
        }
        draw_paddle_image(pad);
    }

}

int check_collision_block(struct Ball *ball, int block_layout[][2], struct Paddle *padA, struct Paddle *padB) {
	// Ball x and y
	int ball_x = ball->x;
	int ball_y = ball->y;
	int radius = ball->radius;

	// Collision with blocks
	struct Block block = {0};

	int dist_x = 0;
	int dist_y = 0;

	int flag_x = 0;
	int flag_y = 0;
	int flag = 0;

	for (int i = 0; i < MAX_BLOCKS; i++) {
		if (block_layout[i][0] > -1 && block_layout[i][1] > -1) {
			// check x: ball hit left of block
			if (ball_x < block_layout[i][0]) {
				dist_x = (block_layout[i][0]) - ball_x;
				flag_x = 1;
			}

			// check x: ball hit top or bottom of block
			else if (ball_x >= block_layout[i][0] &&
					 ball_x <= block_layout[i][0] + BRICK_WIDTH) {
				dist_x = 0;
				flag_x = 2;
			}

			// check x: ball hit right wall of block
			else if (ball_x > block_layout[i][0] + BRICK_WIDTH) {
				dist_x = ball_x - (block_layout[i][0] + BRICK_WIDTH);
				flag_x = 3;
			}

			// check y: before block
			if (ball_y < block_layout[i][1]) {
				dist_y = (block_layout[i][1]) - ball_y;
				flag_y = 10;
			}

			// check y: middle of block
			else if (ball_y >= block_layout[i][1] &&
					ball_y <= block_layout[i][1] + BRICK_HEIGHT) {
				dist_y = 0;
				flag_y = 20;
			}

			// check y: after block
			else if (ball_y > block_layout[i][1] + BRICK_HEIGHT) {
//				uart_puts("\nline 155\n");
				dist_y = ball_y - (block_layout[i][1] + BRICK_HEIGHT);
				flag_y = 30;
			}

			// calculate distance
			float distance_squared = (float)(dist_x*dist_x + dist_y*dist_y);

			if (distance_squared <= (float)(radius*radius)) {
				// Create struct block to remove on screen
				block.x = block_layout[i][0];
				block.y = block_layout[i][1];
				block.width = BRICK_WIDTH;
				block.height = BRICK_HEIGHT;
				// Delete from block layout
				block_layout[i][0] = -1;
				block_layout[i][1] = -1;
				// Remove on screen
				remove_block(&block);

				// Scoring
				ball->streak += 1;
				if (ball->current_player == 'A') {
			        //erase then draw score
			        draw_nums(padA->score, 100, 20, 1);
					padA->score += ball->streak;
					draw_nums(padA->score, 100, 20, 0);
				} else{
					draw_nums(padB->score, 800, 20, 1);
					padB->score += ball->streak;
					draw_nums(padB->score, 800, 20, 0);
				}

				flag = flag_x + flag_y;
			}
		}
	}

	// Change ball angle
	// ball hit right of block
	if (flag == 23) {
		ball->angle = 180 - ball->angle;
	}

	// ball hit left of block
	if (flag == 21) {
		ball->angle = 180 - ball->angle;
	}

	if (!check_collision_edge(ball, padA, padB, flag))
		return 0;

	if(ball->angle >= 360){
		ball->angle -= 360;
	}

	// change angle if hit top left corner
	if (flag == 11) {
		ball->angle = 225;
	}

	// change angle if hit bottom left corner
	if (flag == 31) {
		ball->angle = 135;
	}

	// change angle if hit top right corner
	if (flag == 13) {
		ball->angle = 315;
	}

	// change angle if hit bottom right corner
	if (flag == 33) {
		ball->angle = 45;
	}

	draw_ball(ball);

	return 1;
}

int check_collision_edge(struct Ball *ball, struct Paddle *padA, struct Paddle *padB, int flag) {
	// ball hit right wall => B loses 3 points
		if (ball->x + ball->radius >= 1024) {
			ball->streak = 0;
			if (ball->angle<=180)
				ball->angle = 180 - ball->angle;
			else ball->angle =540 - ball->angle;

			draw_nums(padB->score, 800, 20, 1);
			padB->score-=3;
			draw_nums(padB->score, 800, 20, 0);
			if (padB->score <= 0)
				return 0;
		}

		// ball hit left wall => A loses 3 points
		if (ball->x - ball->radius <= 0) {
			ball->streak = 0;
			if (ball->angle<=180)
				ball->angle = 180 - ball->angle;
			else ball->angle =540 - ball->angle;

			draw_nums(padA->score, 100, 20, 1);
			padA->score-=3;
			draw_nums(padA->score, 100, 20, 0);
			if (padA->score <= 0)
				return 0;
		}

		// ball hit bottom
		if (ball->y + ball->radius >= 768 || flag == 32) {
			ball->angle = 360 - ball->angle;
		}

		// ball hit top
		if (ball->y - ball->radius <= 55 || flag == 12) {
			ball->angle = 360 - ball->angle;
		}
		return 1;
}

void game_run() {
	int physical_width = 1024;
	int physical_height = 768;
	int virtual_width = 1024;
	int virtual_height = 768;

	// Init framebuffer
	framebf_init(physical_width, physical_height, virtual_width, virtual_height);
	// Set background color
	setBGcolor(physical_width, physical_height, 0x00); // set BG to black

	// Initialize state
	stage cur_stage = MENU;
	stage option = GAME;
	int mode = 0, diff = 0;

	while (1) {
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
				game_stage(&cur_stage);
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
				result_stage(&option, &cur_stage);
				break;
			}
			case PAUSE: {
				pause_stage(&option, &cur_stage);
				break;
			}
		}
	}
}


