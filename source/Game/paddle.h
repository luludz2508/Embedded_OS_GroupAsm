// struct Ball ;
struct Paddle {
    char name;
	float x;
	float y;
    float width;
    float height;
	float speed;
};
void draw_paddle(struct Paddle *self);

void move_paddle(struct Paddle *self, char key);

//void check_collision_paddle1(struct Ball *ball, struct Paddle *pad);