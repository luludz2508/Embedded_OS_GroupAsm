// ----------------------------------- framebf.h -------------------------------------
void framebf_init(int physicalWidth , int physicalHeight,int virtualWidth,int virtualHeight);
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLineARGB32(int x1, int y1, int x2, int y2, unsigned int attr);
void drawCircleARGB32(int xc, int yc, int r,unsigned int attr , int fill);
void setBGcolor(int width, int height,unsigned int attr);
void drawChar_lower(int offsetX, int offsetY,unsigned int attr,int charIndex);
void drawChar_upper(int offsetX, int offsetY,unsigned int attr,int charIndex);
void drawString(int offsetX, int offsetY,unsigned int attr, char* string);
