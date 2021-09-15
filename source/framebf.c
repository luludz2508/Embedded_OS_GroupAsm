//-----------------------------------framebf.c-------------------------------------
#include "Font/alphabet_lowercase.h"
#include "Font/alphabet_uppercase.h"
#include "Font/number.h"
#include "Image/image.h"
#include "mbox.h"
#include "uart.h"
#include "Game/background_image.h"

//Use RGBA32 (32 bits for each pixel)
#define COLOR_DEPTH 32
//Pixel Order: BGR in memory order (little endian --> RGB in byte order)
#define PIXEL_ORDER 0
//Screen info
unsigned int width, height, pitch;
/* Frame buffer address
* (declare as pointer of unsigned char to access each byte) */
unsigned char *fb;
/**
* Set screen resolution to 1024x768
*/
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);

void framebf_init(int physicalWidth , int physicalHeight,int virtualWidth,int virtualHeight)
{
	mbox[0] = 35*4; // Length of message in bytes
	mbox[1] = MBOX_REQUEST;
	mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
	mbox[3] = 8; // Value size in bytes
	mbox[4] = 0; // REQUEST CODE = 0
//	mbox[5] = 1024; // Value(width)
//	mbox[6] = 768; // Value(height)
	mbox[5] = physicalWidth; // Value(width)
	mbox[6] = physicalHeight; // Value(height)

	mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
	mbox[8] = 8;
	mbox[9] = 0;
//	mbox[10] = 1024;
//	mbox[11] = 768;
	mbox[10] = virtualWidth;
	mbox[11] = virtualHeight;
	mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
	mbox[13] = 8;
	mbox[14] = 0;
	mbox[15] = 0; // x offset
	mbox[16] = 0; // y offset
	mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth
	mbox[18] = 4;
	mbox[19] = 0;
	mbox[20] = COLOR_DEPTH; //Bits per pixel
	mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
	mbox[22] = 4;
	mbox[23] = 0;
	mbox[24] = PIXEL_ORDER;
	mbox[25] = MBOX_TAG_GETFB; //Get frame buffer
	mbox[26] = 8;
	mbox[27] = 0;
	mbox[28] = 16; //alignment in 16 bytes
	mbox[29] = 0; //will return Frame Buffer size in bytes
	mbox[30] = MBOX_TAG_GETPITCH; //Get pitch
	mbox[31] = 4;
	mbox[32] = 0;
	mbox[33] = 0; //Will get pitch value here
	mbox[34] = MBOX_TAG_LAST;
	// Call Mailbox
	if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ?
		&& mbox[20] == COLOR_DEPTH //got correct color depth ?
		&& mbox[24] == PIXEL_ORDER //got correct pixel order ?
		&& mbox[28] != 0 //got a valid address for frame buffer ?
	) {
		/* Convert GPU address to ARM address (clear higher address bits)
		* Frame Buffer is located in RAM memory, which VideoCore MMU
		* maps it to bus address space starting at 0xC0000000.
		* Software accessing RAM directly use physical addresses
		* (based at 0x00000000)
		*/
		mbox[28] &= 0x3FFFFFFF;
		// Access frame buffer as 1 byte per each address
		fb = (unsigned char *)((unsigned long)mbox[28]);
		uart_puts("Got allocated Frame Buffer at RAM physical address: ");
		uart_hex(mbox[28]);
		uart_puts("\n");
		uart_puts("Frame Buffer Size (bytes): ");
		uart_dec(mbox[29]);
		uart_puts("\n");
		width = mbox[5]; // Actual physical width
		height = mbox[6]; // Actual physical height
		pitch = mbox[33]; // Number of bytes per line

		uart_puts("Physical width: ");
		uart_dec(mbox[5]);
		uart_puts("\n");
		uart_puts("Physical height: ");
		uart_dec(mbox[6]);
		uart_puts("\n");
		uart_puts("Virtual width: ");
		uart_dec(mbox[10]);
		uart_puts("\n");
		uart_puts("Virtual height: ");
		uart_dec(mbox[11]);
		uart_puts("\n");
	} else {
		uart_puts("Unable to get a frame buffer with provided setting\n");
	}
}

void drawPixelARGB32(int x, int y, unsigned int attr)
{
	int offs = (y * pitch) + (COLOR_DEPTH/8 * x);
	/* //Access and assign each byte
	*(fb + offs ) = (attr >> 0 ) & 0xFF; //BLUE
	*(fb + offs + 1) = (attr >> 8 ) & 0xFF; //GREEN
	*(fb + offs + 2) = (attr >> 16) & 0xFF; //RED
	*(fb + offs + 3) = (attr >> 24) & 0xFF; //ALPHA
	*/
	//Access 32-bit together
	*((unsigned int*)(fb + offs)) = attr;
}
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill)
{
	for (int y = y1; y <= y2; y++ )
		for (int x = x1; x <= x2; x++) {
			if ((x == x1 || x == x2) || (y == y1 || y == y2))
			drawPixelARGB32(x, y, attr);
			else if (fill)
			drawPixelARGB32(x, y, attr);
		}
}
void drawLineARGB32(int x0, int y0, int x1, int y1, unsigned int attr)
{
	 int dy, dx, incrE, incrNE, d,x,y;
	dx = x1 - x0;
	dy = y1 - y0;
	d = 2 * dy - dx;
	incrE = 2*dy;
	incrNE = 2*(dy - dx);
	x = x0;
	y = y0;
	drawPixelARGB32(x, y, attr);
	while (x < x1)
	{
		if (d <= 0)
		{
			d += incrE;
			x++;
		}
			else
		{
			d += incrNE;
			x++;
			y++;
		}
		drawPixelARGB32(x, y, attr);
	}
}

void setBGcolor(int width, int height,unsigned int attr){
	for (int i=0; i<height;i++){
		for (int j=0; j<width;j++){
			drawPixelARGB32(j, i, attr);
		}
	}
}
void drawChar_lower(int offsetX, int offsetY,unsigned int attr,int charIndex){
	int width=alphabet_lowercase_width[charIndex];
	for(int x=0; x<width; x++){
		for (int y=0; y<64; y++){
			if(alphabet_lowercase[charIndex][y*width+x]==1){
				drawPixelARGB32(x+offsetX, y+offsetY, attr);
			}
		}
	}
}
void drawChar_upper(int offsetX, int offsetY,unsigned int attr,int charIndex){
	int width=alphabet_uppercase_width[charIndex];
	for(int x=0; x<width; x++){
		for (int y=0; y<64; y++){
			if(alphabet_uppercase[charIndex][y*width+x]==1){
				drawPixelARGB32(x+offsetX, y+offsetY, attr);
			}
		}
	}
}
void drawChar_number(int offsetX, int offsetY,unsigned int attr,int charIndex){
	int width=number_img_width[charIndex];
	for(int x=0; x<width; x++){
		for (int y=0; y<64; y++){
			if(number_img[charIndex][y*width+x]==1){
				drawPixelARGB32(x+offsetX, y+offsetY, attr);
			}
		}
	}
}
void draw_num(int num, int offsetX, int offsetY, int erase) {
	//31x32
	if (num > 0 && num < 6) {
		int offset = (num-1)*31;
		for(int x=offset; x<offset+31; x++){
			for (int y=0; y<32; y++){
				if(one2five[y*155+x] > 0) {
					if (erase)
						drawPixelARGB32(x-offset+offsetX, y+offsetY,  background_img[(int)(y+offsetY)*1024+(int)(x-offset+offsetX)]);
					else drawPixelARGB32(x-offset+offsetX, y+offsetY, 0x00FFFFFF);
				}
			}
		}
	} else {
		int offset = 4*31;
		if (num != 0)
			offset = (num-6)*31;
		for(int x=offset; x<offset+31; x++){
			for (int y=0; y<32; y++){
				if(six2zero[y*155+x] > 0xc0) {
					if (erase)
						drawPixelARGB32(x-offset+offsetX, y+offsetY,  background_img[(int)(y+offsetY)*1024+(int)(x-offset+offsetX)]);
					else drawPixelARGB32(x-offset+offsetX, y+offsetY, 0x00FFFFFF);
				}
			}
		}

	}
}

void draw_nums(int num, int offsetX, int offsetY, int erase) {
	if (num == 0){
		draw_num(0, offsetX, offsetY, erase);
		return;
	}
	int c = 0; /* digit position */
	int n = num;
	while (n != 0) {
	    n /= 10;
	    c++;
	}
	c -= 1;
	n = num;
	/* extract each digit */
	while (c>=0) {
	    int cur_num = n % 10;
	    draw_num(cur_num, offsetX+(31*c), offsetY, erase);
	    n /= 10;
	    c--;
	}
}
void drawString(int offsetX, int offsetY,unsigned int attr, char* string){
	int offsetWidth=offsetX;
	while(*string!='\0'){
		if(*string>='a'&&*string<='z'){
			drawChar_lower(offsetWidth,offsetY,attr,*string-'a');
			offsetWidth +=alphabet_lowercase_width[*string-'a']+7;
		} else if(*string>='A'&&*string<='Z'){
            drawChar_upper(offsetWidth,offsetY,attr,*string-'A');
            offsetWidth +=alphabet_uppercase_width[*string-'A']+7;
        } else if(*string>='0'&&*string<='9'){
            drawChar_number(offsetWidth,offsetY,attr,*string-'0');
            offsetWidth +=number_img_width[*string-'0']+7;
        } else if(*string==' '){
			offsetWidth += 7*3;
		}
		string++;

	}

}

void draw_frame(int score) {
	int color[] = {0x004bcc83, 0x00ad1342, 0x007700a6, 0x001f4e91, 0x00defe47};

	unsigned int attr = color[score%5];

	//top left bottom right
    drawRectARGB32(0, 0, 1024, 10, attr, 1);
    drawRectARGB32(0, 0, 10, 768, attr, 1);
    drawRectARGB32(0, 758, 1024, 768, attr, 1);
    drawRectARGB32(1014, 0, 1024, 768, attr, 1);

    //Drawline of score box
    drawLineARGB32(10, 55, 1014,55 ,attr);
    drawRectARGB32(0, 55 , 1024, 65, attr, 1);

}

void drawBackground() {
	for (int y = 0; y < 600; y++) {
		for (int x = 0; x < 800; x++) {
			drawPixelARGB32(x, y, background[y*800 + x]);
		}
	}

	for (int y = 200; y < 400; y++) {
		for (int x = 250; x < 550; x++) {
			drawPixelARGB32(x, y, dog_and_cat[(y - 200) * 300 + (x - 250)]);
		}
	}

	char c = 0;
	int current_y = 0;
	int step = 40;

	while(1) {
		c = uart_getc();
		if (c == 's' && (current_y <= 1400 - 600 - step)) {
			current_y += step;
			for (int y = 0; y < 600; y++) {
				for (int x = 0; x < 800; x++) {
					drawPixelARGB32(x, y, background[(y + current_y)*800 + x]);
				}
			}
		}

		if (c == 'w' && (current_y >= step)) {
			current_y -= step;
			for (int y = 0; y < 600; y++) {
				for (int x = 0; x < 800; x++) {
					drawPixelARGB32(x, y, background[(y + current_y)*800 + x]);
				}
			}
		}

		for (int y = 200; y < 400; y++) {
			for (int x = 250; x < 550; x++) {
				drawPixelARGB32(x, y, dog_and_cat[(y - 200) * 300 + (x - 250)]);
			}
		}
	}
}
