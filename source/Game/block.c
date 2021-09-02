#include "block.h"
#include "../uart.h"
#include "../framebf.h"
#include "block_image.h"

// Draw brick 19x51
void draw_block(struct Block *self) {
	for (int y = 0; y < self->height; y++){
		for (int x = 0; x < self->width; x++){
			drawPixelARGB32(x + self->x - (self->width / 2), y + self->y - (self->height / 2), block_image[y * self->width + x]);
		}
	}
}

// Draw map
void draw_map(int block_layout[][2]) {
//	int block_layout[8][2] = {	{320, 100},
//								{370, 100},
//								{420, 100},
//								{470, 100},

//								{320, 180},
//								{370, 180},
//								{420, 180},
//								{470, 180}
//	};

//	for (int i = 0, j = 100; i < 8; i++, j+= 200) {
//
//		blocks[i].x = block_layout[i][0];
//		blocks[i].y = block_layout[i][1];
//		blocks[i].width = 19;
//		blocks[i].height = 51;
//
//		draw_block(&blocks[i]);
//	}
	struct Block block;
	int layout_index = 0;

	// y-axis
	for (int i = 160; i <= 520; i+=85) {
		// x-axis
		for (int j = 320; j <= 470; j+=50) {
			block.x = j;
			block.y = i;
			block.width = 23;
			block.height = 83;
//			blocks[block_index].x = j;
//						blocks[block_index].y = i;
//						blocks[block_index].width = 23;
//						blocks[block_index].height = 83;
//			block_index++;
			uart_puts("\nx = ");
			uart_dec(j);
			uart_puts("		-	y = ");
			uart_dec(i);
			uart_puts("\n");
			block_layout[layout_index][0] = j;
			block_layout[layout_index][1] = i;
			layout_index++;
//			uart_puts("\layout_index = ");
//			uart_dec(layout_index);
//			uart_puts("\n");

			draw_block(&block);
		}
	}
}

// Remove block
void remove_block(struct Block *self) {
	for (int y = 0; y < self->height; y++){
		for (int x = 0; x < self->width; x++){
			drawPixelARGB32(x + self->x - (self->width / 2), y + self->y - (self->height / 2), 0x00);
		}
	}
}
