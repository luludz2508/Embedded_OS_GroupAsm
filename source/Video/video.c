#include "../framebf.h"
#include "../uart.h"
#include "../mbox.h"
#include "video.h"
#include "video_source.h"
void video(){
	// set up serial console
	 uart_init();
	 framebf_init(640,480, 640, 480);

	 int index=0, image=0;
	 while(1) {

		 index++;
		 if(index%30000000==0){
			for (int i=0; i<heightImage; i++){//height
				for (int j=0; j<widthImage; j++){ //width
					drawPixelARGB32(j, i, Image[image][i*widthImage+j]);
				}
			}
			 image++;
			 if(image==42){
	   				 image=0;
//				 break;
			 }
		 }
	 }
}
