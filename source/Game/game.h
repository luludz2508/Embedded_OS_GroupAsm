void wait_msec(unsigned int n);
void check_collision_paddle(struct Ball *ball, struct Paddle *pad, int *streaks);
int check_collision_block(struct Ball *ball, int block_layout[][2], int *streaks);
void score(struct Paddle *padA, struct Paddle *padB, char pad_name, int *streaks);
void game_run();

