// For macros and variables that need to be public throughout all function scopes
#pragma once

#define DEFAULT_FIFO_SIZE (256 * 1024)

// Return codes, also a boolean wrapper for 0 and 1
#define SUCCESS 0
#define GENERAL_FAILURE -1
#define UGX_FALSE 0
#define UGX_TRUE 1

// Colors, declared as u8s for GXColor compatibility
static const u8 RED[3] = {255, 0 ,0};
static const u8 GREEN[3] = {0, 255 ,0};
static const u8 BLUE[3] = {0, 0 ,255};
static const u8 WHITE[3] = {255, 255, 255};
static const u8 BLACK[3] = {0, 0, 0};

// Video variables
GXRModeObj* rmode;
void* xfb;
void* gp_fifo;
Mtx44 ortho;

// Wrappers for WiiUse, assigned in UGX_inputScan(), see in UGX.h
u32 UGX_buttonsHeld;
u32 UGX_buttonsDown;