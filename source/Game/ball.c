#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "ball_image.h"
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
	int r = self->radius;
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
			if(x*x + y*y <= r*r)
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), ball_image[(y + r - 1)*(2*r - 1) + x + r - 1]);

	for (int y = -3; y < 4; y++){
		for (int x = -3; x < 4; x++){
			drawPixelARGB32(x + self->x, y + self->y, 0x0000FF);
		}
	}
}

int move_ball(struct Ball *self, int block_layout[][2]){
	draw_ball(self);

	// Check collision
	int flag = check_collision(self->x, self->y, self->radius, block_layout);

	// ball hit right of block
	if (flag == 23) {
		self->angle = 180 - self->angle;
	}

	// ball hit left of block
	if (flag == 21) {
		self->angle = 180 - self->angle;
	}

	// ball hit right wall => lose
	if (self->x + self->radius >= 1024) {
		self->angle = 180 - self->angle;
		uart_puts("you lose\n");
		return 0;
	}

	// ball hit left wall => lose
	if (self->x - self->radius <= 0) {
		self->angle = 180 - self->angle;
		uart_puts("you lose\n");
		return 0;
	}

	// ball hit bottom
	if (self->y + self->radius >= 768 || flag == 32) {
		self->angle = 360 - self->angle;
	}

	// ball hit top
	if (self->y - self->radius <= 0 || flag == 12) {
		self->angle = 360 - self->angle;
	}

	if(self->angle >= 360){
		self->angle -= 360;
	}

	// change angle if hit top left corner
	if (flag == 11) {
		self->angle = 225;
	}

	// change angle if hit bottom left corner
	if (flag == 31) {
		self->angle = 135;
	}

	// change angle if hit top right corner
	if (flag == 13) {
		self->angle = 315;
	}

	// change angle if hit bottom right corner
	if (flag == 33) {
		self->angle = 45;
	}

	return 1;
}
