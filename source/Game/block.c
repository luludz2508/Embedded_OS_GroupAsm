#include "block.h"
#include "../uart.h"
#include "../framebf.h"
#include "block_image.h"
#include <math.h>

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
	for (int i = 130; i <= 650; i+=85) {
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
//			uart_puts("\nx = ");
//			uart_dec(j);
//			uart_puts("		-	y = ");
//			uart_dec(i);
//			uart_puts("\n");

			block_layout[layout_index][0] = j;
			block_layout[layout_index][1] = i;
			layout_index++;

			uart_puts("\nblock_layout = ");
			uart_dec(i);
			uart_puts("			j = ");
						uart_dec(j);
			uart_puts("\n");

			draw_block(&block);
		}
	}
}

int check_collision(int ball_x, int ball_y, int radius, int block_layout[][2]) {
	// Collision with blocks
	struct Block block = {0};

//	for (int i = 0; i < 24; i++) {
//		if (block_layout[i][0] > 0 && block_layout[i][1] > 0) {
//							uart_puts("\n");
//							uart_dec(i);
//			uart_puts("				");
//				uart_dec(block_layout[i][0]);
//				uart_puts("			");
//				uart_dec(block_layout[i][1]);
//		}
//	}

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
	0;
	int flag_x = 0;
	int flag_y = 0;

	for (int i = 0; i < 24; i++) {
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
			if (ball_x < block_layout[i][0] - 23/2) {
				dist_x = (block_layout[i][0] - 23/2) - ball_x;
				flag_x = 1;
			}

			// check x: ball hit top or bottom of block
			else if (ball_x >= block_layout[i][0] - 23/2 &&
					 ball_x <= block_layout[i][0] + 23/2) {
				dist_x = 0;
				flag_x = 3;
			}

			// check x: ball hit right wall of block
			else if (ball_x > block_layout[i][0] + 23/2) {
				dist_x = ball_x - (block_layout[i][0] + 23/2);
				flag_x = 2;
			}

			// check y: before block
			if (ball_y < block_layout[i][1] - 83/2) {
				dist_y = (block_layout[i][1] - 83/2) - ball_y;
				flag_y = 10;
			}

			// check y: middle of block
			else if (ball_y >= block_layout[i][1] - 83/2 &&
					ball_y <= block_layout[i][1] + 83/2) {
				dist_y = 0;
				flag_y = 20;
			}

			// check y: after block
			else if (ball_y > block_layout[i][1] + 83/2) {
				uart_puts("\nline 155\n");
				dist_y = ball_y - (block_layout[i][1] + 83/2);
				flag_y = 30;
			}

			// calculate distance
			float distance_squared = (float)(dist_x*dist_x + dist_y*dist_y);

			if (distance_squared <= (float)(radius*radius)) {
				// Create struct block to remove on screen
				block.x = block_layout[i][0];
				block.y = block_layout[i][1];
				block.width = 23;
				block.height = 83;
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

// Remove block
void remove_block(struct Block *self) {
	for (int y = 0; y < self->height; y++){
		for (int x = 0; x < self->width; x++){
			drawPixelARGB32(x + self->x - (self->width / 2), y + self->y - (self->height / 2), 0x00);
		}
	}
}

float absolute(float n) {
  return (n < 0) ? (n * -1.0) : n;
}

//float square(float n, float i, float j)
//{
//    float mid = (i + j) / 2;
//    float mul = mid * mid;
//
//    // If mid itself is the square root,
//    // return mid
//    if ((mul == n) || (absolute(mul - n) < 0.00001)) // max 5 error places
//        return mid;
//
//    // If mul is less than n, recur second half
//    else if (mul < n)
//        return square(n, mid, j);
//
//    // Else recur first half
//    else
//        return square(n, i, mid);
//}
//
//float find_sqrt(float n)
//{
//    float i = 1;
//
//    // While the square root is not found
//    int found = 0;
//    while (!found) {
//        // If n is a perfect square
//        if (i * i == n) {
//            return i;
//            found = 1;
//        }
//        else if (i * i > n) {
//
//            // Square root will lie in the
//            // interval i-1 and i
//            float res = square(n, i - 1, i);
//            return res;
//            found = 0;
//        }
//        i++;
//    }
//}
//
//float find_distance(int x1, int y1, int x2, int y2) {
//  float n = (float)((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1));
//  return find_sqrt(n);
//}
