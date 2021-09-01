#include "ball.h"
#include "framebf.h"
#include "ballImage.h"

typedef struct Ball{
	double x;
	double y;
	int size;
	int widthSize;
	int heightSize;
	double speed;
	double angle;
};

void drawBall(struct Ball *self){
	for(int y= 0 ;y<self->heightSize;y++){
		for(int x= 0 ;x<self->widthSize;x++){
			drawPixelARGB32(x+self->x- self->size, y+self->y- self->size, ballImage[y*self->widthSize+x]);
		}
	}
}
void moveBall(struct Ball *self){
	double radian= self->angle*(PI/180);
//	uart_puts("\n");
//	uart_dec((int)(cos(radian) * 1000));
//	uart_puts("\n");
//	self->x+=(self->speed)*cos(1.0472);
//	self->y+=(self->speed)*sin(radian);
	self->x+=self->speed;
	drawBall(self);

}
