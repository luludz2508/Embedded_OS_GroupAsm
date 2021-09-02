struct Block {
	double x;
	double y;
	int width;
	int height;
};

void draw_block(struct Block *self);
void draw_map(int block_layout[][2]);
void remove_block(struct Block *self);
