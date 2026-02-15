#ifndef _DOOZ_CORE_H_
#define _DOOZ_CORE_H_

#if defined(_M_AMD64) || defined(_M_X64) || defined(__x86_64) \
        || defined(__x86_64__) || defined(__amd64) || defined(__amd64__)
    #define DOOZ_ARCH_AMD64 1
    #define DOOZ_ARCH_EXT_SSE4_1 1
#endif

#include <raylib.h>

#define ADD_REMOVED_PIECE_BACK 0

#define ARRAY_LEN(a)    (sizeof(a) / sizeof(a[0]))

#define PLAYER_PIECES_MAX 12

#define BOARD_BLOCK_WIDTH       140
#define BOARD_ROW_BLOCK_COUNT   7
#define BOARD_BLOCKS_COUNT      24

#define PIECE_RADIUS                (BOARD_BLOCK_WIDTH>>2) /* Divide by 4 */
#define PIECE_SELECTION_RADIUS      PIECE_RADIUS + 15
#define PIECE_SELECTION_THICKNESS   10

#define PIECE_COLOR_HUMAN   BLUE
#define PIECE_COLOR_AI      RED

#define WINDOW_WIDTH    BOARD_ROW_BLOCK_COUNT * BOARD_BLOCK_WIDTH
#define WINDOW_HEIGHT   WINDOW_WIDTH

#define TARGET_FPS  30

#define MAX(a, b)   (((a) < (b)) ? (b) : (a))
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))

extern const Vector2 BLOCK_HALF;
extern const Vector2 BLOCKS_POS[BOARD_BLOCKS_COUNT];

#endif // !_DOOZ_CORE_H_
