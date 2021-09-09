// struct Ball ;
struct Paddle {
    char name;
	float x;
	float y;
    float width;
    float height;
	float speed;
};
void draw_paddle_image(struct Paddle *self);

void draw_paddle(struct Paddle *self);

void move_paddle(struct Paddle *self, char key);
