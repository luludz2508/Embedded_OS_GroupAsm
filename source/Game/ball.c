#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "ball_image.h"
#include "block.h"
#include "background_image.h"

const float PI=3.14159;

void init_ball(struct Ball *self1, struct Ball *self2, int speed) {
	self1->current_player = 'A';
	self1->x = 300;
	self1->y = 100;
	self1->speed = speed+1;
	self1->angle = 120;
	self1->streak = 0;

	self2->current_player = 'B';
	self2->x = 700;
	self2->y = 100;
	self2->speed = speed+1;
	self2->angle = 120;
	self2->streak = 0;

}

float sinx(float x, int n){
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
    float x_velocity=(self->speed) * sinx(PI/2-radian,20);
    float y_velocity=(self->speed) * sinx(radian,20);
	for(int y = -r; y <= r ; y++){
		for(int x = -r; x <= r ; x++){
		    float next_x= x - x_velocity;
		    float next_y= y - y_velocity;
			if ( ( x*x + y*y < (r+1)*(r+1) ) && ( next_x*next_x + next_y*next_y > (r-2)*(r-2) ) ){
				drawPixelARGB32((int)(x + ball_x), (int)(y + ball_y), background_img[(int)(y + ball_y)*1024+(int)(x + ball_x)]);
            }
        }
    }
	self->x += x_velocity;
	self->y += y_velocity;

//	 mark center of ball in blue
	for(int y = -r; y <= r; y++)
		for(int x = -r; x <= r; x++)
			if(x*x + y*y <= r*r && ball_image[(y + r - 1)*(2*r - 1) + x + r - 1] !=0x00ffffff)
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), ball_image[(y + r - 1)*(2*r - 1) + x + r - 1]);

//	for (int y = -3; y < 4; y++){
//		for (int x = -3; x < 4; x++){
//			drawPixelARGB32(x + self->x, y + self->y, 0x0000FF);
//		}
//	}
}

