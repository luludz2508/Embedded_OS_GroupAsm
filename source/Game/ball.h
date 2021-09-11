// struct Ball
struct Ball {
	float x;
	float y;
	int radius;
	float speed;
	float angle;
	int streak;
    char current_player;
};

void draw_ball(struct Ball *self);
void init_ball(struct Ball *self1, struct Ball *self2);
