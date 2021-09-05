#include "paddle.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "paddleImage.h"
#include "block.h"

void draw_paddle(struct Paddle *self){

	int halfWidth = self->width/2;
	int halfHeight = self->height/2;

	for(int y = -halfHeight; y <= halfHeight; y++)
		for(int x = -halfWidth; x <= halfWidth; x++)
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), 0x00);

	self->y+=(self->speed) ;

//	 mark center of paddle in blue
	for(int y = -halfHeight; y <= halfHeight; y++)
		for(int x = -halfWidth; x <= halfWidth; x++)
		    if (self->name=='A'){
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), leftPaddleImage[(y + halfHeight)*halfWidth*2 + x +halfWidth]);
            } else if (self->name=='B'){
                drawPixelARGB32((int)(x + self->x), (int)(y + self->y), rightPaddleImage[(y + halfHeight)*halfWidth*2 + x +halfWidth]);
            }

	for (int y = -3; y < 4; y++){
		for (int x = -3; x < 4; x++){
			drawPixelARGB32(x + self->x, y + self->y, 0x0000FF);
		}
	}
}

void move_paddle(struct Paddle *self, char key){
    if ((key=='w' ||key=='i') && (self->y - self->height/2 - absolute(self->speed)>= 0)){
        if (self->speed>0){
            self->speed = -self->speed;
        }
    } else if ((key=='s' ||key=='k') && (self->y + self->height/2 + absolute(self->speed)< 600)){
        if (self->speed<0){
            self->speed = -self->speed;
        }
    } else {
    return;
    }
	draw_paddle(self);
}

//void check_collision_paddle1(struct Ball *ball, struct Paddle *pad){
//    return;
//}