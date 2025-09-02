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
    
    // Vertex descriptions and attribute formats
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    // Vertex format 0 is for RGBA
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    // Vertex format 1 is for RGB
    GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);

    // TEV setup
    GX_SetNumChans(1);
	GX_SetNumTexGens(0);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);

    GX_SetCullMode(GX_CULL_NONE);

    return SUCCESS;
}

int UGX_drawInit(){
    guOrtho(ortho, rmode->xfbHeight, 0, 0, rmode->fbWidth, -1, 1);
    GX_LoadProjectionMtx(ortho, GX_ORTHOGRAPHIC);

    return SUCCESS;
}

int UGX_consoleInit(){
    CON_InitEx(rmode, 10, 10, rmode->fbWidth - 20, rmode->xfbHeight - 20);

    return SUCCESS;
}

int UGX_drawPointRGBA(f32 x, f32 y, const u8 color[4]){
    GX_Begin(GX_POINTS, GX_VTXFMT0, 1);
        GX_Position3f32(x, y, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);
    GX_End();

    return SUCCESS;
}

int UGX_drawPointRGB(f32 x, f32 y, const f32 color[3]){
    GX_Begin(GX_POINTS, GX_VTXFMT1, 1);
        GX_Position3f32(x, y, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);
    GX_End();

    return SUCCESS;
}

int UGX_drawLineRGBA(f32 x1, f32 y1, f32 x2, f32 y2, const u8 color[4]){
    GX_Begin(GX_LINES, GX_VTXFMT0, 2);
        GX_Position3f32(x1, y1, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);
        
        GX_Position3f32(x2, y2, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);
    GX_End();

    return SUCCESS;
}

int UGX_drawLineRGB(f32 x1, f32 y1, f32 x2, f32 y2, const f32 color[3]){
    GX_Begin(GX_LINES, GX_VTXFMT1, 2);
        GX_Position3f32(x1, y1, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);
        
        GX_Position3f32(x2, y2, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);
    GX_End();

    return SUCCESS;
}

int UGX_drawQuadRGBA(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4, const u8 color[4]){
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        // Top-left
        GX_Position3f32(x1, y1, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        // Top-right
        GX_Position3f32(x2, y2, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        // Bottom-right
        GX_Position3f32(x3, y3, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        // Bottom-left
        GX_Position3f32(x4, y4, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);
    GX_End();

    return SUCCESS;
}

int UGX_drawQuadRGB(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4, const f32 color[3]){
    GX_Begin(GX_QUADS, GX_VTXFMT1, 4);
        // Top-left
        GX_Position3f32(x1, y1, 0.0f);
        GX_Color4u8(color[0], color[1], color[2]);

        // Top-right
        GX_Position3f32(x2, y2, 0.0f);
        GX_Color4u8(color[0], color[1], color[2]);

        // Bottom-right
        GX_Position3f32(x3, y3, 0.0f);
        GX_Color4u8(color[0], color[1], color[2]);

        // Bottom-left
        GX_Position3f32(x4, y4, 0.0f);
        GX_Color4u8(color[0], color[1], color[2]);
    GX_End();

    return SUCCESS;
}

int UGX_drawSquareRGBA(f32 x, f32 y, int width, const u8 color[4]){
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        // Top-left
        GX_Position3f32(x, y, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        // Top-right
        GX_Position3f32(x + width, y, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        // Bottom-right
        GX_Position3f32(x + width, y - width, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        // Bottom-left
        GX_Position3f32(x, y - width, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);
    GX_End();

    return SUCCESS;
}

int UGX_drawSquareRGB(f32 x, f32 y, int width, const f32 color[3]){
    GX_Begin(GX_QUADS, GX_VTXFMT1, 4);
        // Top-left
        GX_Position3f32(x, y, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);

        // Top-right
        GX_Position3f32(x + width, y, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);

        // Bottom-right
        GX_Position3f32(x + width, y - width, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);

        // Bottom-left
        GX_Position3f32(x, y - width, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);
    GX_End();

    return SUCCESS;
}

int UGX_drawTriangleRGBA(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, const u8 color[4]){
    GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
        GX_Position3f32(x0, y0, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        GX_Position3f32(x1, y1, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        GX_Position3f32(x2, y2, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);
    GX_End();

    return SUCCESS;
}

int UGX_drawTriangleRGB(f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, const f32 color[3]){
    GX_Begin(GX_TRIANGLES, GX_VTXFMT1, 3);
        GX_Position3f32(x0, y0, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);

        GX_Position3f32(x1, y1, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);

        GX_Position3f32(x2, y2, 0.0f);
        GX_Color3f32(color[0], color[1], color[2]);
    GX_End();

    return SUCCESS;
}

int UGX_drawEquilateralTriangleRGBA(f32 x, f32 y, width const u8 color[4]){
    GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
        GX_Position3f32(x, y, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        GX_Position3f32(x + width, y, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);

        GX_Position3f32((x + width / 2.0f), y + width, 0.0f);
        GX_Color4u8(color[0], color[1], color[2], color[3]);
    GX_End();

    return SUCCESS;
}

int UGX_drawEquilateralTriangleRGB(f32 x, f32 y, width const f32 color[3]){
    GX_Begin(GX_TRIANGLES, GX_VTXFMT1, 3);
        GX_Position3f32(x, y, 0.0f);
        GX_Color4u8(color[0], color[1], color[2]);

        GX_Position3f32(x + width, y, 0.0f);
        GX_Color4u8(color[0], color[1], color[2]);

        GX_Position3f32((x + width / 2.0f), y + width, 0.0f);
        GX_Color4u8(color[0], color[1], color[2]);
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
int UGX_setCopyClear(const u8 color[4]){
    GX_SetCopyClear((GXColor){color[0], color[1], color[2], color[3]}, 0x00FFFFFF);

    return SUCCESS;
}

// Basically WASD movement but for the DPAD
int UGX_WPADMovement(f32* x, f32* y){
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) *x += 5;
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT) *x -= 5;
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP) *y += 5;
    if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN) *y -= 5;

    return SUCCESS;
}

// Converts UGX_ColorF32 to a UGX_ColorU8
int UGX_convertColorF32ToColorU8(const UGX_colorF32* input, UGX_colorU8* output){
    output->r = (u8)(fminf(fmaxf(input->r, 0.0f), 1.0f) * 255.0f);
    output->g = (u8)(fminf(fmaxf(input->g, 0.0f), 1.0f) * 255.0f);
    output->b = (u8)(fminf(fmaxf(input->b, 0.0f), 1.0f) * 255.0f);
    output->alpha = (u8)(fminf(fmaxf(input->alpha, 0.0f), 1.0f) * 255.0f);

    return SUCCESS;
}

// Converts UGX_ColorU8 to a UGX_ColorF32
int UGX_convertColorU8ToF32(const UGX_colorU8* input, UGX_colorF32* output){
    output->r = input->r / 255.0f;
    output->g = input->g / 255.0f;
    output->b = input->b / 255.0f;
    output->alpha = input->alpha / 255.0f;

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
