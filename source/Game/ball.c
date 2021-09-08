#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "ballImage.h"
#include "block.h"


const float PI=3.14159;

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
	int r = self->radius + 1;
    float ball_x= self -> x;
    float ball_y= self -> y;

	for(int y = -r; y <= r; y++)
		for(int x = -r; x <= r; x++)
			if(x*x + y*y <= r*r)
				drawPixelARGB32((int)(x + ball_x), (int)(y + ball_y), 0x0);
	self->x+=(self->speed) * sinx(PI/2-radian,20);
	self->y+=(self->speed) * sinx(radian,20);

//	 mark center of ball in blue
	for(int y = -r; y <= r; y++)
		for(int x = -r; x <= r; x++)
			if(x*x + y*y <= r*r && ballImage[(y + r - 1)*(2*r - 1) + x + r - 1]!=0x00ffffff)
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), ballImage[(y + r - 1)*(2*r - 1) + x + r - 1]);

	for (int y = -3; y < 4; y++){
		for (int x = -3; x < 4; x++){
			drawPixelARGB32(x + self->x, y + self->y, 0x0000FF);
		}
	}
}

void move_ball(struct Ball *self, int block_layout[][2]){
	draw_ball(self);

	// Check collision
	int flag = check_collision(self->x, self->y, self->radius, block_layout);

	// ball hit right wall
	if (self->x + self->radius >= 800 || flag == 23) {
		self->angle = 180 - self->angle;
	}

	// ball hit left wall
	if (self->x - self->radius <= 0 || flag == 21) {
		self->angle = 180 - self->angle;
	}

	// ball hit bottom
	if (self->y + self->radius >= 600 || flag == 32) {
		self->angle = 360 - self->angle;
	}

	// ball hit top
	if (self->y - self->radius <= 0 || flag == 12) {
		self->angle = 360 - self->angle;
	}

	if(self->angle >= 360){
		self->angle -= 360;
	}

	// if hit top left corner
	if (flag == 11) {
		self->angle = 225;
	}

	// if hit bottom left corner
	if (flag == 31) {
		self->angle = 135;
	}

	// if hit top right corner
	if (flag == 13) {
		self->angle = 315;
	}

	// if hit bottom right corner
	if (flag == 33) {
		self->angle = 45;
	}
}
