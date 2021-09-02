//#include "stdio.h"
#include "ball.h"
#include "../framebf.h"
	struct Ball newBall = {50, 50, 13, 25, 25, 10, 10};
	struct Ball newBall2 = {400, 300, 13, 25, 25, 10, 10};

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

void gameRun(){
	int physicalWidth = 800;
	int physicalHeight = 600;
	int virtualWidth = 800;
	int virtualHeight = 600;

	framebf_init(physicalWidth, physicalHeight, virtualWidth, virtualHeight);
	setBGcolor(physicalWidth, physicalHeight, 0x00); // set BG to white

	drawBall(&newBall);

	drawBall(&newBall2);
	while(1){
//		setBGcolor(physicalWidth, physicalHeight, 0x0000); // set BG to white
		moveBall(&newBall);
		moveBall(&newBall2);
		wait_msec(10000);
	}
}


