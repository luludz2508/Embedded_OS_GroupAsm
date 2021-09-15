#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "Font/font.h"
#include "Image/image.h"
#include "Video/video.h"
#include "Game/game.h"

#define task 1

int main() {
    #if task == 1
        font();
    #elif task == 2
        video();
    #elif task == 3
        image();
    #elif task == 4
        game_run();
    #endif

    return 0;
}