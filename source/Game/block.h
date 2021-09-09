//typedef struct Block Block;

struct Block {
	double x;
	double y;
	int width;
	int height;
};

void draw_block(struct Block *self);
void draw_map(int block_layout[][2]);
void remove_block(struct Block *self);
int check_collision_block(int ball_x, int ball_y, int radius, int block_layout[][2], int *streaks);
float absolute(float n);
