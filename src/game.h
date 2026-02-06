#ifndef _DOOZ_GAME_GAME_H_
#define _DOOZ_GAME_GAME_H_

typedef enum {
    NO_MOVE,
    PLACE,
    FLY,
} move_type_t;

typedef struct {
    move_type_t type;
    int from, to;
} move_t;

typedef enum {
    EMPTY,
    HUMAN,
    AI,

    __P_COUNT,
} player_t;

typedef int block_index_t;
typedef player_t board_t[BOARD_BLOCKS_COUNT];

typedef struct {
    board_t board;

    // Selected block index
    block_index_t selected_block;

    int player_pieces_count[__P_COUNT];
    player_t turn;
    bool can_remove;
} game_state_t;

#endif // !_DOOZ_GAME_GAME_H_
