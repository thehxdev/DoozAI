#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _MSC_VER
    #pragma comment(lib, "winmm.lib")
#endif

#include "core.c"
#include "game.c"
#include "ai.c"
#include "gfx.c"
#include "main.c"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
