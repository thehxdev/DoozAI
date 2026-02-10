#ifdef _MSC_VER
    #pragma comment(lib, "winmm.lib")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include <raylib.h>
#include <raymath.h>

#include "core.h"
#include "game.h"

#include "core.c"
#include "game.c"
#include "ai.c"
#include "gfx.c"
#include "main.c"
