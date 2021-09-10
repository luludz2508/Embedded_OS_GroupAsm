#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "ball_image.h"
#include "block.h"
#include "background.h"

const float PI=3.14159;

void init_ball(struct Ball *self) {
	self->x = 700;
	self->y = 100;
	self->speed = 0.5;
	self->angle = 180;
	self->current_player = 'B';
}

float sinx(float x, int n)
{
   float t = x;
   float sine = t;
   for ( int a=1; a<n; ++a)
   {
      float mult = -x*x/((2*a+1)*(2*a));
      t *= mult;
      sine += t;
   }
   return sine;
}

void draw_ball(struct Ball *self){
	float radian = self->angle*PI/180;
	int r = self->radius;
    float ball_x= self -> x;
    float ball_y= self -> y;

	for(int y = -r; y <= r; y++)
		for(int x = -r; x <= r; x++)
			if(x*x + y*y <= r*r)
//				drawPixelARGB32((int)(x + ball_x), (int)(y + ball_y), 0x0);
				drawPixelARGB32((int)(x + ball_x), (int)(y + ball_y), background_img[(int)(y + ball_y)*1024+(int)(x + ball_x)]);

	self->x+=(self->speed) * sinx(PI/2-radian,20);
	self->y+=(self->speed) * sinx(radian,20);

//	 mark center of ball in blue
	for(int y = -r; y <= r; y++)
		for(int x = -r; x <= r; x++)
			if(x*x + y*y <= r*r && ball_image[(y + r - 1)*(2*r - 1) + x + r - 1] !=0x00ffffff)
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), ball_image[(y + r - 1)*(2*r - 1) + x + r - 1]);

	for (int y = -3; y < 4; y++){
		for (int x = -3; x < 4; x++){
			drawPixelARGB32(x + self->x, y + self->y, 0x0000FF);
		}
	}
}

int move_ball(struct Ball *self){
	draw_ball(self);
}
