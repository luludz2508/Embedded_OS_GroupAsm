// struct Ball
struct Ball {
	float x;
	float y;
	int radius;
	float speed;
	float angle;
    char current_player;
};

void draw_ball(struct Ball *self);
int move_ball(struct Ball *self);
void init_ball(struct Ball *self);
