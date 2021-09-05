
// struct Ball ;
struct Ball {
	float x;
	float y;
	int radius;
	float speed;
	float angle;
};
void draw_ball(struct Ball *self);
void move_ball(struct Ball *self, int block_layout[][2]);
