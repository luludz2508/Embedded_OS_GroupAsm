//typedef struct Ball{
//	int x=0;
//	int y=0;
//	int size=25;
//	int widthSize=50;
//	int heightSize=49;
//};
//void drawBall(struct Ball* self);

//typedef struct Ball ;
struct Ball {
	double x;
	double y;
	int radius;
	int widthSize;
	int heightSize;
	double speed;
	double angle;
};
void drawBall(struct Ball *self);
void moveBall(struct Ball *self);
