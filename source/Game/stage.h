typedef enum {
	MENU, SETTING, GAME, RESULT, PAUSE, PLAYER, DIFF, HOWTO
} stage;

enum button {
	START, OPTIONS, SELECT, LEVELS, ABOUT, HOME, ONE, MULTI, EASY, HARD, BACK, RESUME, EXIT, RESTART
};

void flow();
void menu_stage(stage *option, stage *main, int *diff);
void setting_stage(stage *main);
void player_stage(int *mode, stage *main);
void diff_stage(int *diff, stage *main);
void howto_stage(stage *main);
void game_stage(stage *main);
void result_stage(stage *option, stage *main, int *diff);
void pause_stage(stage *option, stage *main);
void draw_arrow(int offsetX, int offsetY, int erase, int arrowIdx);
void draw_button(int offsetY,int buttonIdx);

