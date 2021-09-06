//#include "stdio.h"
#include "ball.h"
#include "paddle.h"
#include "../framebf.h"
#include "../uart.h"
#include "block.h"

struct Ball newBall = {50, 250, 13, 2, 0};
//	struct Ball newBall2 = {400, 300, 13, 25, 25, 10, 10};

struct Paddle leftPaddle = {'A',20, 45, 20, 90, 50};
struct Paddle rightPaddle = {'B',780, 45, 20, 90, 50};

char input, keyDownA, keyDownB;
int countLoopA=0;
int countLoopB=0;
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
void check_collision_paddle1(struct Ball *ball, struct Paddle *pad){
    float dist_x= ball->x - pad->x - pad->width/2;
    float dist_y= ball->y - pad->y - pad->height/2;
    int flag_x=0, flag_y=0;
    // check x: ball hit left of block
        if (ball->x < pad->x - pad->width/2) {
            dist_x = (pad->x ) - ball->x - pad->width/2;
            flag_x = 1;
        }
        // check x: ball hit top or bottom of block
//        else if (ball->x >= pad->x  && ball->x <= pad->x  + pad->width) {
//            dist_x = 0;
//            flag_x = 2;
//        }
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
    float width_dist = dist_x*dist_x +dist_y*dist_y;
    int flag=flag_x+flag_y;
    if (width_dist <= (float)(ball->radius * ball->radius)){
        if (ball->x + ball->radius >= 800 || flag == 23) {
            ball->angle = 180 - ball->angle;
        }

        // ball hit left wall
        if (ball->x - ball->radius <= 0 || flag == 21) {
            ball->angle = 180 - ball->angle;
        }

        // ball hit bottom
        if (ball->y + ball->radius >= 600 || flag == 32) {
            ball->angle = 360 - ball->angle;
        }

        // ball hit top
        if (ball->y - ball->radius <= 0 || flag == 12) {
            ball->angle = 360 - ball->angle;
        }

        if(ball->angle >= 360){
            ball->angle -= 360;
        }

        // if hit top left corner
        if (flag == 11) {
            ball->angle = 225;
        }

        // if hit bottom left corner
        if (flag == 31) {
            ball->angle = 135;
        }

        // if hit top right corner
        if (flag == 13) {
            ball->angle = 315;
        }

        // if hit bottom right corner
        if (flag == 33) {
            ball->angle = 45;
        }
        draw_paddle_image(pad);
    }

}
void game_run() {
	int physicalWidth = 800;
	int physicalHeight = 600;
	int virtualWidth = 800;
	int virtualHeight = 600;
		setBGcolor(physicalWidth, physicalHeight, 0x00ffffff); // set BG to white

	// Background
	framebf_init(physicalWidth, physicalHeight, virtualWidth, virtualHeight);
	setBGcolor(physicalWidth, physicalHeight, 0x00); // set BG to white

	// Bricks

	int block_layout[][2] = {0};
	draw_map(block_layout);

	// Balls
	draw_ball(&newBall); // ball 1
    //	draw_ball(&newBall2); // ball 2
    //paddles
    draw_paddle(&leftPaddle);
    draw_paddle(&rightPaddle);
	while(1) {
	    input=getUart();
	    if(input!='\0' && countLoopA==0){
            if (input=='w'|| input=='s'){
	            move_paddle(&leftPaddle,input);
                keyDownA=input;
            }
	    }
	    if(input!='\0' && countLoopB==0){
             if(input=='i'|| input=='k'){
                move_paddle(&rightPaddle,input);
                keyDownB=input;
            }
	    }
        if(keyDownA!=0){
            countLoopA++;
            if (countLoopA==20){
                countLoopA=0;
                keyDownA=0;
            }
        }
        if(keyDownB!=0){
             countLoopB++;
             if (countLoopB==20){
                 countLoopB=0;
                 keyDownB=0;
             }
         }

        check_collision_paddle1(&newBall, &leftPaddle);
        check_collision_paddle1(&newBall, &rightPaddle);
		move_ball(&newBall, block_layout);
//		move_ball(&newBall2);
		wait_msec(5000);
	}
}


