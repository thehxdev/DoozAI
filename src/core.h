#ifndef _DOOZ_CORE_H_
#define _DOOZ_CORE_H_

#define ADD_REMOVED_PIECE_BACK 0

#define ARRAY_LEN(a)    (sizeof(a) / sizeof(a[0]))

#define PLAYER_PIECES_MAX 12

#define BOARD_BLOCK_WIDTH       140
#define BOARD_ROW_BLOCK_COUNT   7
#define BOARD_BLOCKS_COUNT      24

#define PIECE_RADIUS                BOARD_BLOCK_WIDTH/4
#define PIECE_SELECTION_RADIUS      PIECE_RADIUS + 15
#define PIECE_SELECTION_THICKNESS   10

#define PIECE_COLOR_HUMAN   BLUE
#define PIECE_COLOR_AI      RED

#define WINDOW_WIDTH    BOARD_ROW_BLOCK_COUNT * BOARD_BLOCK_WIDTH
#define WINDOW_HEIGHT   WINDOW_WIDTH

#define TARGET_FPS  30

#define MAX(a, b)   (((a) < (b)) ? (b) : (a))
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))

#endif // !_DOOZ_CORE_H_
