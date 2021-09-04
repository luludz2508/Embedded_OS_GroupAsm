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
int check_collision(int ball_x, int ball_y, int radius, int block_layout[][2]);
//float square(float n, float i, float j);
//float find_sqrt(float n);
