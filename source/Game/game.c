//#include "stdio.h"
#include "ball.h"
#include "paddle.h"
#include "../framebf.h"
#include "../uart.h"
#include "stage.h"
#include "block.h"
#include "background.h"


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
    if (width_dist <= (float)(ball->radius * ball->radius)){
        float ratio= (ball->y - pad->y)/(pad->height/2);
        int modify_angle=50;
        uart_puts("\nAngle Before: ");
        uart_dec((int)( ball->angle));
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

        // ball hit left wall
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
        if (ball->y + ball->radius >= 768 || flag == 32) {
            ball->angle = 360 - ball->angle;
        }

        // ball hit top
        if (ball->y - ball->radius <= 0 || flag == 12) {
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
void game_run() {
	int physical_width = 1024;
	int physical_height = 768;
	int virtual_width = 1024;
	int virtual_height = 768;

	// Background
	// Set background color
	setBGcolor(physical_width, physical_height, 0x00ffffff); // set BG to white



	// Init framebuffer
	framebf_init(physical_width, physical_height, virtual_width, virtual_height);
	// Set background color
	setBGcolor(physical_width, physical_height, 0x00); // set BG to white

	// Lay out bricks
//	int block_layout[][2] = {0}; // positions of top left point of bricks

	// Initialize state
	stage cur_stage = MENU;
	stage option = GAME;
	int mode = 0, diff = 0;

	// Balls
//	draw_ball(&new_ball); // ball 1
    //	draw_ball(&new_ball2); // ball 2

	//Paddles
//    draw_paddle(&left_paddle);
//    draw_paddle(&right_paddle);

	while(1) {
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


