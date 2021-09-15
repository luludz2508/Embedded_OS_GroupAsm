// struct Ball ;
struct Paddle {
    char name;
	float x;
	float y;
    float width;
    float height;
	float speed;
	int score;
};
void draw_paddle_image(struct Paddle *self);

void draw_paddle(struct Paddle *self);

void move_paddle(struct Paddle *self, char key);

void init_paddles(struct Paddle *p1, struct Paddle *p2);
