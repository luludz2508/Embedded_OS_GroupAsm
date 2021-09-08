#include "block.h"
#include "../uart.h"
#include "../framebf.h"
#include "block_image.h"

const int brick_width = 23;
const int brick_height = 83;
const int max_blocks = 64;

// Draw brick brick_widthxbrick_height
void draw_block(struct Block *self) {
	for (int y = 0; y < self->height; y++){
		for (int x = 0; x < self->width; x++){
			drawPixelARGB32(x + self->x, y + self->y, block_image[y * self->width + x]);
		}
	}
}

// Remove block
void remove_block(struct Block *self) {
	for (int y = 0; y < self->height; y++){
		for (int x = 0; x < self->width; x++){
			drawPixelARGB32(x + self->x, y + self->y, 0x00);
		}
	}

	uart_puts("remove block");
}

// Init block layout
void init_blocks(int block_layout[][2]) {
	for (int i = 0; i < max_blocks; i++){
		block_layout[i][0] = 0;
		block_layout[i][1] = 0;
	}
}

// Draw map
void draw_map(int block_layout[][2]) {

	struct Block block;
	int layout_index = 0;

	// y-axis
	for (int i = 100; i <= 690; i+=83) {
		// x-axis
		for (int j = 408; j <= 573; j+=23) {
						uart_puts("block layout[0]=");
						uart_dec(block_layout[layout_index][0]);
						uart_puts(",	block layout[1]=");
						uart_dec(block_layout[layout_index][1]);
						uart_puts(",	layout index=");
						uart_dec(layout_index);
						uart_puts("\n");

			if (block_layout[layout_index][0] > -1) {
				block.x = j;
				block.y = i;
				block.width = brick_width;
				block.height = brick_height;

				block_layout[layout_index][0] = j;
				block_layout[layout_index][1] = i;

				draw_block(&block);
			}

			layout_index++;
		}
	}
}

int check_collision(int ball_x, int ball_y, int radius, int block_layout[][2]) {
	// Collision with blocks
	struct Block block = {0};

	/*
	X < Xa -> dist(X) = Xa - X
	Xa < x < Xb -> dist(X) = 0
	X > Xb -> dist(X) = X - Xb

	Y < Ya -> dist(Y) = Ya - Y
	Ya < Y < Yd -> dist(Y) = 0
	Y > Yd -> dist(Y) = Y - Yd

	dist = sqrt(dist(X)^2 + dist(Y)^2) > R => collide
	 */

	int dist_x = 0;
	int dist_y = 0;

	int flag_x = 0;
	int flag_y = 0;

	for (int i = 0; i < max_blocks; i++) {
		if (block_layout[i][0] > -1 && block_layout[i][1] > -1) {
			// check x: ball hit left wall of block
//			uart_dec(ball_x);
//			uart_puts("				");
//			uart_dec(block_layout[i][0]);
//			uart_puts("					");
//			uart_dec(ball_y);
//			uart_puts("				");
//			uart_dec(block_layout[i][1]);
//			uart_puts("\n");

			// check x: ball hit left of block
			if (ball_x < block_layout[i][0]) {
				dist_x = (block_layout[i][0]) - ball_x;
				flag_x = 1;
			}

			// check x: ball hit top or bottom of block
			else if (ball_x >= block_layout[i][0] &&
					 ball_x <= block_layout[i][0] + brick_width) {
				dist_x = 0;
				flag_x = 2;
			}

			// check x: ball hit right wall of block
			else if (ball_x > block_layout[i][0] + brick_width) {
				dist_x = ball_x - (block_layout[i][0] + brick_width);
				flag_x = 3;
			}

			// check y: before block
			if (ball_y < block_layout[i][1]) {
				dist_y = (block_layout[i][1]) - ball_y;
				flag_y = 10;
			}

			// check y: middle of block
			else if (ball_y >= block_layout[i][1] &&
					ball_y <= block_layout[i][1] + brick_height) {
				dist_y = 0;
				flag_y = 20;
			}

			// check y: after block
			else if (ball_y > block_layout[i][1] + brick_height) {
//				uart_puts("\nline 155\n");
				dist_y = ball_y - (block_layout[i][1] + brick_height);
				flag_y = 30;
			}

			// calculate distance
			float distance_squared = (float)(dist_x*dist_x + dist_y*dist_y);

			if (distance_squared <= (float)(radius*radius)) {
//				uart_puts("i =");
//															uart_dec(i);
//				uart_puts(", r =");
//											uart_dec(radius);
//				uart_puts(" ball_x = ");
//							uart_dec(ball_x);
//							uart_puts(", brick_x = ");
//							uart_dec(block_layout[i][0]);
//							uart_puts(" - ball_y = ");
//							uart_dec(ball_y);
//							uart_puts(", brick_y = ");
//							uart_dec(block_layout[i][1]);
//							uart_puts(" flag = ");
//								uart_dec(flag_x+flag_y);
//							uart_puts("\n");

				// Create struct block to remove on screen
				block.x = block_layout[i][0];
				block.y = block_layout[i][1];
				block.width = brick_width;
				block.height = brick_height;
				// Delete from block layout
				block_layout[i][0] = -1;
				block_layout[i][1] = -1;
				// Remove on screen
				remove_block(&block);

				return flag_x + flag_y;
			}
		}
	}

	return 0;
}

float absolute(float n) {
  return (n < 0) ? (n * -1.0) : n;
}
