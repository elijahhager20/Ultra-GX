#include <gccore.h>
#include <stdlib.h>
#include <ogcsys.h>
#include <wiiuse/wpad.h>
#include <malloc.h>
#include "../source/UGX.h"

int main(int argc, char* argv[]) {
    UGX_init();
    UGX_drawInit();
    
    f32 x = 100.0f;
    f32 y = 100.0f;
    UGX_colorU8 color = {.rgba = {255, 0, 0, 127}};
    
    UGX_colorF32 backgroundF32 = {.rgba = {0.0f, 0.0f, 1.0f, 1.0f}};
    UGX_colorU8 backgroundU8;
    UGX_convertColorF32ToColorU8(&backgroundF32, &backgroundU8);
    UGX_setCopyClear(backgroundU8.rgba);

    while (1) {
        UGX_inputScan();

        UGX_WPADMovement(&x, &y);
        if (UGX_WPAD_home()) break;
        
        if (x < 0) x = 0;
        if (x > UGX_VIDEO_WIDTH) x = UGX_VIDEO_WIDTH;
        if (y < 0) y = 0;
        if (y > UGX_VIDEO_HEIGHT) y = UGX_VIDEO_HEIGHT;

        UGX_drawTriangleRGBA(x, y, x + 20, y, x, y + 20, color.rgba);
        UGX_refreshFrame();
    }

    return 0;
}