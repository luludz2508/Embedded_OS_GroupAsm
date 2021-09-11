void wait_msec(unsigned int n);
void check_collision_paddle(struct Ball *ball, struct Paddle *pad);
int check_collision_block(struct Ball *ball, int block_layout[][2], struct Paddle *padA, struct Paddle *padB);
void game_run();

