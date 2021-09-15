struct Block {
	double x;
	double y;
	int width;
	int height;
};

void draw_block(struct Block *self, int color);
void draw_map(int block_layout[][2]);
void remove_block(struct Block *self);
float absolute(float n);
void init_blocks(int block_layout[][2]);
