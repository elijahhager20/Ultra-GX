// For macros and variables that need to be public throughout all function scopes
#pragma once

#define DEFAULT_FIFO_SIZE (256 * 1024)

// Return codes, also a boolean wrapper for 0 and 1
#define SUCCESS 0
#define GENERAL_FAILURE -1
#define UGX_FALSE 0
#define UGX_TRUE 1

// Colors, obviously
const u8 UGX_RED_U8[3] = {255, 0 , 0};
const u8 UGX_GREEN_U8[3] = {0, 255 , 0};
const u8 UGX_BLUE_U8[3] = {0, 0 , 255};
const u8 UGX_WHITE_U8[3] = {255, 255, 255};
const u8 UGX_BLACK_U8[3] = {0, 0, 0};
const f32 UGX_RED_F32[3] = {1.0f, 0.0f, 0.0f};
const f32 UGX_GREEN_F32[3] = {0.0f, 1.0f, 0.0f};
const f32 UGX_BLUE_F32[3] = {0.0f, 0.0f, 1.0f};
const f32 UGX_WHITE_F32[3] = {1.0f, 1.0f, 1.0f};
const f32 UGX_BLACK_F32[3] = {0.0f, 0.0f, 0.0f};

typedef union{
    struct{
        union{
            struct{
                u8 r;
                u8 g;
                u8 b;
            };
            u8 rgb[3];
        };
        u8 alpha;
    };
    u8 rgba[4];
} UGX_colorU8;

// Video variables
GXRModeObj* rmode;
void* xfb;
void* gp_fifo;
Mtx44 ortho;

// Wrappers for WiiUse, assigned in UGX_inputScan(), see in UGX.h
u32 UGX_buttonsHeld;
u32 UGX_buttonsDown;