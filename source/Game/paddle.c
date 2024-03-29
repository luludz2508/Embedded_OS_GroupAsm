#include "paddle.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "block.h"
#include "paddle_image.h"
#include "background_image.h"

void init_paddles(struct Paddle *p1, struct Paddle *p2) {
	// Paddle 1
	p1->x = 22;
	p1->y = 312;
	p1->speed = 50;
	p1->score = 5;

	// Paddle 2
	p2->x = 1002;
	p2->y = 312;
	p2->speed = 50;
	p2->score = 5;
}

void draw_paddle_image(struct Paddle *self){
	int half_width = self->width/2;
	int half_height = self->height/2;

	for(int y = -half_height; y <= half_height; y++)
		for(int x = -half_width; x <= half_width; x++)
		    if (self->name=='A'){
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), left_paddle_image[(y + half_height)*half_width*2 + x +half_width]);
            } else if (self->name=='B'){
                drawPixelARGB32((int)(x + self->x), (int)(y + self->y), right_paddle_image[(y + half_height)*half_width*2 + x +half_width]);
            }
}

void draw_paddle(struct Paddle *self){
	int half_width = self->width/2;
	int half_height = self->height/2;

	for(int y = -half_height; y <= half_height; y++)
		for(int x = -half_width; x <= half_width; x++)
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y),  background_img[(int)(y + self->y)*1024+(int)(x + self->x)]);

	self->y+=(self->speed);
    draw_paddle_image(self);
}

void move_paddle(struct Paddle *self, char key){
    if ((key== 'w' || key== 'i') && (self->y - self->height/2 - absolute(self->speed) >= 55)){
        if (self->speed > 0){
            self->speed = -self->speed;
        }
    } else if ((key== 's' ||key== 'k') && (self->y + self->height/2 + absolute(self->speed) <= 767)){
        if (self->speed < 0){
            self->speed = -self->speed;
        }
    } else {
    	return;
    }

	draw_paddle(self);
}
