//#include "stdio.h"
#include "ball.h"
#include "paddle.h"
#include "../framebf.h"
#include "../uart.h"
#include "block.h"

struct Ball newBall = {50, 250, 13, 2, 76};
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
//	struct Block blocks[4][5];
	// Brick layout
	int block_layout[][2] = {0};
	draw_map(block_layout);
//	uart_puts("\layout_index = ");
//	uart_dec(block_layout[0][0]);
//	uart_puts("\n");
//	uart_dec(block_layout[0][1]);
//		uart_puts("\n");

//	struct Block block = {block_layout[0][0], block_layout[0][1], 23, 83};
//	remove_block(&block);

	// Balls
	draw_ball(&newBall); // ball 1
//	draw_ball(&newBall2); // ball 2

    //paddles
    draw_paddle(&leftPaddle);
    draw_paddle(&rightPaddle);
	while(1) {
	    input=uart_getc();
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

//        check_collision_paddle1(&newBall, &leftPaddle);
//        check_collision_paddle1(&newBall, &rightPaddle);
		move_ball(&newBall, block_layout);
//		move_ball(&newBall2);
		wait_msec(5000);
	}
}


