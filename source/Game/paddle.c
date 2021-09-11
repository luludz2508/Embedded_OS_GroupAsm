#include "paddle.h"
#include "ball.h"
#include "../uart.h"
#include "../framebf.h"
#include "block.h"
#include "paddle_image.h"

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
				drawPixelARGB32((int)(x + self->x), (int)(y + self->y), 0x00);

	self->y+=(self->speed);
    draw_paddle_image(self);

	for (int y = -3; y < 4; y++){
		for (int x = -3; x < 4; x++){
			drawPixelARGB32(x + self->x, y + self->y, 0x0000FF);
		}
	}
}

void move_paddle(struct Paddle *self, char key){
    if ((key== 'w' || key== 'i') && (self->y - self->height/2 - absolute(self->speed) >= 0)){
        if (self->speed > 0){
            self->speed = -self->speed;
        }
    } else if ((key== 's' ||key== 'k') && (self->y + self->height/2 + absolute(self->speed) < 768)){
        if (self->speed < 0){
            self->speed = -self->speed;
        }
    } else {
    	return;
    }

	draw_paddle(self);
}

void init_paddles(struct Paddle *p1, struct Paddle *p2) {
	// Paddle 1
	p1->x = 20;
	p1->y = 45;
	p1->speed = 50;
	p1->score = 20;

	// Paddle 2
	p2->x = 1004;
	p2->y = 45;
	p2->speed = 50;
	p2->score = 20;
}

