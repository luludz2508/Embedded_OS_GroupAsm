#include "block.h"
#include "../uart.h"
#include "../framebf.h"
#include "block_image.h"
#include "background.h"

#ifndef BRICK_WIDTH
#define BRICK_WIDTH 23
#endif
#ifndef BRICK_HEIGHT
#define BRICK_HEIGHT 83
#endif
#ifndef MAX_BLOCKS
#define MAX_BLOCKS 64
#endif
//int BRICK_WIDTH = 23;
//int BRICK_HEIGHT = 83;
//int MAX_BLOCKS = 64;

// Draw brick BRICK_WIDTHxBRICK_HEIGHT
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
//			drawPixelARGB32(x + self->x, y + self->y, 0x00);
			drawPixelARGB32(x + self->x, y + self->y, background_img[(int)(x + self->x)*1024+(int)(y + self->y)]);
		}
	}

	uart_puts("remove block");
}

// Init block layout
void init_blocks(int block_layout[][2]) {
	for (int i = 0; i < MAX_BLOCKS; i++){
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
//						uart_puts("block layout[0]=");
//						uart_dec(block_layout[layout_index][0]);
//						uart_puts(",	block layout[1]=");
//						uart_dec(block_layout[layout_index][1]);
//						uart_puts(",	layout index=");
//						uart_dec(layout_index);
//						uart_puts("\n");

			if (block_layout[layout_index][0] > -1) {
				block.x = j;
				block.y = i;
				block.width = BRICK_WIDTH;
				block.height = BRICK_HEIGHT;

				block_layout[layout_index][0] = j;
				block_layout[layout_index][1] = i;

				draw_block(&block);
			}

			layout_index++;
		}
	}
}

float absolute(float n) {
  return (n < 0) ? (n * -1.0) : n;
}
