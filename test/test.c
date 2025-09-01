#include <gccore.h>
#include <stdlib.h>
#include <ogcsys.h>
#include <wiiuse/wpad.h>
#include <malloc.h>
#include "../source/globals.h"
#include "../source/UGX.h"

int main(int argc, char* argv[]) {
    UGX_init();
    
    f32 x = 100.0f;
    f32 y = 100.0f;
    int width = 20;

    while (1) {
        // Input checking, changes x and/or y coordinate and closes when home button is pressed
        UGX_inputScan();
        UGX_WPADMovement(&x, &y);
        if (UGX_WPAD_home() == UGX_TRUE) break;
        // Bounds checking
        if (x < 0) x = 0;
        if (x > rmode->xfbHeight - 20) x = rmode->xfbHeight - 20;
        if (y < 0) y = 0;
        if (y > rmode->fbWidth - 20) y = rmode->fbWidth - 20;

        // Refresh the frame buffer and draw to the screen
        UGX_drawSquare(&x, &y, &width);
        UGX_refreshFrame();
    }

    return 0;
}