//#include "stdio.h"
#include "ball.c"
#include "../framebf.h"

void wait_msec(unsigned int n);
void gameRun(){

	int timeCount = 0;
	int physicalWidth = 800;
	int physicalHeight = 600;
	int virtualWidth = 800;
	int virtualHeight = 600;

	framebf_init(physicalWidth, physicalHeight, virtualWidth, virtualHeight);
	setBGcolor(physicalWidth, physicalHeight, 0x00ffffff); // set BG to white
	struct Ball newBall = {50, 50, 25, 50, 49, 0.5, 60};

	drawBall(&newBall);
	while(1){

		moveBall(&newBall);

		wait_msec(5000);

	}
}
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
