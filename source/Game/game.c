//#include "stdio.h"
#include "ball.h"
#include "../framebf.h"
#include "block.h"

	struct Ball newBall = {50, 250, 13, 2, 20};
//	struct Ball newBall2 = {400, 300, 13, 25, 25, 10, 10};

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

	while(1) {
//		setBGcolor(physicalWidth, physicalHeight, 0x0000); // set BG to white
		move_ball(&newBall, block_layout);
//		move_ball(&newBall2);
		wait_msec(5000);
	}
}


