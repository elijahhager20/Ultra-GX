#pragma once

#include <gccore.h>
#include <stdlib.h>
#include <ogcsys.h>
#include <wiiuse/wpad.h>
#include <malloc.h>
#include "globals.h"

int UGX_init(){
    // Basic initilization, pretty readable on its own
    VIDEO_Init();
    WPAD_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(0);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();
    gp_fifo = memalign(32, DEFAULT_FIFO_SIZE);
    GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);
    
    // Projection Matrix
    guOrtho(ortho, 0, rmode->fbWidth, 0, rmode->xfbHeight, -1, 1);
    GX_LoadProjectionMtx(ortho, GX_ORTHOGRAPHIC);
    
    // Vertex descriptions and attribute formats
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

    return SUCCESS;
}

int UGX_drawSquare(f32* x, f32* y, int* width){
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        // Top-left
        GX_Position3f32(*x, *y, 0.0f);
        // Top-right
        GX_Position3f32(*x + *width, *y, 0.0f);
        // Bottom-left
        GX_Position3f32(*x + *width, *y + *width, 0.0f);
        // Bottom-right
        GX_Position3f32(*x, *y + *width, 0.0f);
    GX_End();

    return SUCCESS;
}

int UGX_drawTriangle(f32* left, f32* right, f32* top, int* width, int* height){
    GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
        // Left 
        GX_Position3f32(*left, *top + *height, 0.0f);
        // Right 
        GX_Position3f32(*right, *top + *height, 0.0f);
        // Top 
        GX_Position3f32((*left + *right) / 2.0f, *top, 0.0f);
    GX_End();
    
    return SUCCESS;
}

// Used at the end of every loop (Usually)
int UGX_refreshFrame(){
    GX_DrawDone();
    GX_CopyDisp(xfb, GX_TRUE);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_Flush();
    VIDEO_WaitVSync();

    return SUCCESS;
}

// Clear the screen with the color passed in
int UGX_setCopyClear(const u8 color[]){
    GX_SetCopyClear((GXColor){color[0], color[1], color[2], 255}, 0x00FFFFFF);

    return SUCCESS;
}

// Basically WASD movement but for the DPAD
int UGX_WPADMovement(f32* x, f32* y){
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) *x += 5;
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT) *x -= 5;
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP) *y -= 5;
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN) *y += 5;

    return SUCCESS;
}

// Input Checking (Raw input, no specialty things like UGX_WPADMovement())
// Literally just a wrapper for WiiUse, lmao
int UGX_inputScan(){
    WPAD_ScanPads();
    UGX_buttonsHeld = WPAD_ButtonsHeld(0);
    UGX_buttonsDown = WPAD_ButtonsDown(0);
    return SUCCESS;
}

// Home button
int UGX_WPAD_home(){
    return (UGX_buttonsDown & WPAD_BUTTON_HOME) ? UGX_TRUE : UGX_FALSE;
}

// Right button
int UGX_WPAD_right(){
    return (UGX_buttonsDown & WPAD_BUTTON_RIGHT) ? UGX_TRUE : UGX_FALSE;
}

// Left button
int UGX_WPAD_left(){
    return (UGX_buttonsDown & WPAD_BUTTON_LEFT) ? UGX_TRUE : UGX_FALSE;
}

// Up button
int UGX_WPAD_up(){
    return (UGX_buttonsDown & WPAD_BUTTON_UP) ? UGX_TRUE : UGX_FALSE;
}

// Down button
int UGX_WPAD_down(){
    return (UGX_buttonsDown & WPAD_BUTTON_DOWN) ? UGX_TRUE : UGX_FALSE;
}

// B button
int UGX_WPAD_b(){
    return (UGX_buttonsDown & WPAD_BUTTON_B) ? UGX_TRUE : UGX_FALSE;
}

// A button
int UGX_WPAD_a(){
    return (UGX_buttonsDown & WPAD_BUTTON_A) ? UGX_TRUE : UGX_FALSE;
}

// Minus button
int UGX_WPAD_minus(){
    return (UGX_buttonsDown & WPAD_BUTTON_MINUS) ? UGX_TRUE : UGX_FALSE;
}

// Plus button
int UGX_WPAD_plus(){
    return (UGX_buttonsDown & WPAD_BUTTON_PLUS) ? UGX_TRUE : UGX_FALSE;
}

// One button
int UGX_WPAD_one(){
    return (UGX_buttonsDown & WPAD_BUTTON_1) ? UGX_TRUE : UGX_FALSE;
}

// Two button
int UGX_WPAD_two(){
    return (UGX_buttonsDown & WPAD_BUTTON_2) ? UGX_TRUE : UGX_FALSE;
}