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
    int height = 20;
    
    UGX_colorF32 backgroundF32 = {.rgb = {0.0f, 0.0f, 1.0f}};
    UGX_colorU8 backgroundU8;
    UGX_convertColorF32ToColorU8(&backgroundF32, &backgroundU8);
    UGX_setCopyClear(backgroundU8.rgba);

    while (1) {
        UGX_inputScan();

        UGX_WPADMovement(&x, &y);
        if (UGX_WPAD_home()) break;
        
        if (x < 0) x = 0;
        if (x > rmode->fbWidth - width) x = rmode->fbWidth - width;
        if (y < 0) y = 0;
        if (y > rmode->xfbHeight - height) y = rmode->xfbHeight - height;

        UGX_drawTriangle(x, x + width, y, height, UGX_RED_U8);
        UGX_refreshFrame();
    }

    return 0;
}