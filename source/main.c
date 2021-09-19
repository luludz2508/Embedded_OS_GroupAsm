#include "uart.h"
#include "mbox.h"
#include "framebf.h"

#include "Font/font.h"
#include "Video/video.h"
#include "Image/image.h"
#include "Game/paddle.h"
#include "Game/ball.h"
#include "Game/game.h"

#define task 1

int main() {
    #if task == 1
        font();
    #elif task == 2
        video();
    #elif task == 3
        display_image();
    #elif task == 4
        display_background();
    #elif task == 5
        game_run();
    #endif

    return 0;
}
































//void video(){
//	// set up serial console
//	 uart_init();
//	 framebf_init(640,480, 640, 480);
//
//	 int index=0, image=0;
//	 while(1) {
//
//		 index++;
//		 if(index%30000000==0){
//			for (int i=0; i<heightImage; i++){//height
//				for (int j=0; j<widthImage; j++){ //width
//					drawPixelARGB32(j, i, Image[image][i*widthImage+j]);
//				}
//			}
//			 image++;
//			 if(image==42){
//	   				 image=0;
////				 break;
//			 }
//		 }
//	 }
//}
//void font(){
//	framebf_init(physicalWidth,physicalHeight, virtualWidth, virtualHeight);
//	setBGcolor(physicalWidth,physicalHeight, 0x00000000); // set BG to white
//	drawString(50,64, 0x00ffffff,"Phuc Nguyen");
//	drawString(50,128, 0x00ffffff,"Ha Phan");
//	drawString(50,192, 0x00ffffff,"Luan Nguyen");
//}
