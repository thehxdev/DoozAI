#ifndef _DOOZ_GAME_H_
#define _DOOZ_GAME_H_

#include "core.h"

typedef enum {
    NO_MOVE,
    PLACE,
    FLY,
} move_type_t;

typedef struct {
    move_type_t type;
    int from, to;
} move_t;

enum {
    EMPTY,
    HUMAN,
    AI,

    __P_COUNT,
};

typedef unsigned char player_t;
typedef char block_index_t;
typedef player_t board_t[BOARD_BLOCKS_COUNT];

typedef struct {
    board_t board;

    // Selected block index
    block_index_t selected_block;

    int player_pieces_count[__P_COUNT];
    player_t turn;
    bool can_remove;
} game_state_t;

bool board_move_caused_mill(const board_t board, player_t player, block_index_t idx);

int board_count_mills(const board_t board, player_t player);

bool game_move_do(game_state_t *game, move_t move);

bool game_remove_do(game_state_t *game, block_index_t idx);

game_state_t *game_move_gen_next_states(game_state_t game, move_t move, player_t player);

bool game_is_over(board_t board);

move_t *game_gen_valid_moves(board_t board, player_t player, int player_pieces_count);

Vector2 board_pos_snap_to_grid(Vector2 pos);

block_index_t board_find_block_by_pos(board_t board, Vector2 pos);

#endif // !_DOOZ_GAME_H_
