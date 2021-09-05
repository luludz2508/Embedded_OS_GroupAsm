#include "ball.h"
#include "../uart.h"
#include <math.h>
#include "../framebf.h"
#include "ballImage.h"
#include "block.h"


const double PI=3.14159;

double sinx(double x, int n)
{
   double t = x;
   double sine = t;
   for ( int a=1; a<n; ++a)
   {
      double mult = -x*x/((2*a+1)*(2*a));
      t *= mult;
      sine += t;
   }
   return sine;
}
void draw_ball(struct Ball *self){
//	for (int i=0; i<600;i++){
//		for (int j=0; j<800;j++){
//			map[j][i]=0x00;
//		}
//	}
//	for(int y= 0 ;y<self->heightSize;y++){
//		for(int x= 0 ;x<self->widthSize;x++){
//			drawPixelARGB32(0+self->x- self->radius - self->speed, 0+self->y- self->radius, 0x00);
//			map[(int)(x+self->x- self->radius)][(int)(y+self->y- self->radius)]=ballImage[(int)(y*self->widthSize+x)];
//			drawPixelARGB32(x+self->x- self->radius, y+self->y- self->radius, ballImage[y*self->widthSize+x]);
			for(int y=-self->radius; y<=self->radius; y++)
				for(int x=-self->radius; x<=self->radius; x++)
					if(x*x+y*y <= self->radius*self->radius)
			        	drawPixelARGB32((int)(x+self->x- self->radius-(self->speed) * sinx(PI/2-self->angle*PI/180,20)), (int)(y+self->y- self->radius- (self->speed) * sinx(self->angle*PI/180,20)), 0x0);
//						map[(int)(x+self->x- self->radius -self->speed)][(int)(y+self->y- self->radius)]= 0x00;
			for(int y=-self->radius; y<=self->radius; y++)
				    for(int x=-self->radius; x<=self->radius; x++)
				        if(x*x+y*y <= self->radius*self->radius)
				        	drawPixelARGB32((int)(x+self->x- self->radius), (int)(y+self->y- self->radius), ballImage[(y+self->heightSize/2)*self->widthSize+x+self->widthSize/2]);
//				        	map[(int)(x+self->x- self->radius)][(int)(y+self->y- self->radius)]= ballImage[(y+self->heightSize/2)*self->widthSize+x+self->widthSize/2];
//		}
//	}
//	for (int i=0; i<600;i++){
//		for (int j=0; j<800;j++){
//			drawPixelARGB32(j, i, map[j][i]);
//		}
//	}
}

void move_ball(struct Ball *self, int block_layout[][2]){
	double radian = self->angle*PI/180;
//	uart_puts("Current x:");
//	uart_dec(self->x);
//	uart_puts("				");
//	uart_puts(" y:");
//	uart_dec(self->y);
//	uart_puts("\n");
//	uart_puts("Current angle:");
//	uart_dec(self->angle);
//	uart_puts("\n\n");

	self->x+=(self->speed) * sinx(PI/2-radian,20);
	self->y+=(self->speed) * sinx(radian,20);
//	self->x+=self->speed;
	draw_ball(self);

//	for (int i = 0; i < 24; i++) {
//		if (block_layout[i][0] > 0 && block_layout[i][1] > 0) {
//				uart_puts("\n");
//				uart_dec(i);
//				uart_puts("\n");
//				uart_dec(block_layout[i][0]);
//				uart_puts("\n");
//				uart_dec(block_layout[i][1]);
//		}
//	}

	// Check collision
	int flag = check_collision(self->x, self->y, self->radius, block_layout);

	// ball hit right wall
	if (self->x + self->radius >= 800 || flag == 23) {
		uart_puts("\nline 69\n");
		self->angle = 180 - self->angle;
	}

	// ball hit left wall
	if (self->x - self->radius <= 0 || flag == 21) {
		uart_puts("\nline 73\n");
		self->angle = 180 - self->angle;
	}

	// ball hit bottom
	if (self->y + self->radius >= 600 || flag == 32) {
		uart_puts("\nline 77\n");
		self->angle = 360 - self->angle;
	}

	// ball hit top
	if (self->y - self->radius <= 0 || flag == 12) {
		uart_puts("\nline 82\n");
		self->angle = 360 - self->angle;
	}

	if(self->angle >= 360){
		uart_puts("\nline 85\n");
		self->angle -= 360;
	}

	if (flag == 11) {

		self->angle = 45;
	}

	if (flag == 31) {
		self->angle = 45;
	}

	if (flag == 13) {
		self->angle = 45;
	}

	if (flag == 33) {
		self->angle = 45;
	}

}
