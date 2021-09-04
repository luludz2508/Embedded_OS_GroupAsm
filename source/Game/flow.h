typedef enum {
	MENU, SETTING, GAME, RESULT
} stage;

//extern volatile stage cur_stage;

void flow();
void menu_stage(stage *option, stage *main);
void setting_stage(int *speed, stage *main);
void clear_fb(int w, int h);
