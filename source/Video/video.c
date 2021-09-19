#include "../framebf.h"
#include "../uart.h"
#include "../mbox.h"
#include "video.h"
#include "video_source.h"

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
void video(){
	// set up serial console
	 uart_init();
	 framebf_init(640,480, 640, 480);

	 int image=0;
	 while(1) {
		 for (int i=0; i<heightImage; i++){//height
			for (int j=0; j<widthImage; j++){ //width
				drawPixelARGB32(j, i, Image[image][i*widthImage+j]);
			}
		 }
		 image++;
		 if(image==42){
			 image=0;
		 }
		 wait_msec(100000);
	 }
}
