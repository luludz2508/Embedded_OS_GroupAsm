#include "ball.h"
#include "../framebf.h"
#include "ballImage.h"

const double PI=3.14159;


void drawBall(struct Ball *self){
//	for (int i=0; i<600;i++){
//		for (int j=0; j<800;j++){
//			map[j][i]=0x00;
//		}
//	}
	for(int y= 0 ;y<self->heightSize;y++){
		for(int x= 0 ;x<self->widthSize;x++){
//			drawPixelARGB32(0+self->x- self->radius - self->speed, 0+self->y- self->radius, 0x00);
//			map[(int)(x+self->x- self->radius)][(int)(y+self->y- self->radius)]=ballImage[(int)(y*self->widthSize+x)];
			drawPixelARGB32(x+self->x- self->radius, y+self->y- self->radius, ballImage[y*self->widthSize+x]);
		}
	}
//	for (int i=0; i<600;i++){
//		for (int j=0; j<800;j++){
//			drawPixelARGB32(j, i, map[j][i]);
//		}
//	}
}
void moveBall(struct Ball *self){
	double radian = self->angle*(PI/180);
//		uart_dec((int)(cos(1.0472) * 1000));
//		uart_dec((int)(cos(radian) * 1000));

//	uart_puts("\n");
//	uart_dec((int)(cos(1.0472) * 1000));
//	uart_puts("\n");

//	self->x+=(self->speed) * cos((double)((double)self->angle * (PI/(double)180)));
//	self->y+=(self->speed) * sin((double)((double)self->angle * (PI/(double)180)));
	self->x+=self->speed;
	drawBall(self);

}
