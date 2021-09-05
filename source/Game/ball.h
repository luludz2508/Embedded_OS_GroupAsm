
// struct Ball ;
struct Ball {
	double x;
	double y;
	int radius;
	double speed;
	double angle;
};
void draw_ball(struct Ball *self);
void move_ball(struct Ball *self, int block_layout[][2]);
