const block_index_t MILLS[][3] = {
    // Outer Ring
    {0, 1, 2}, {2, 3, 4}, {4, 5, 6}, {6, 7, 0},
    // Middle Ring
    {8, 9, 10}, {10, 11, 12}, {12, 13, 14}, {14, 15, 8},
    // Inner Ring
    {16, 17, 18}, {18, 19, 20}, {20, 21, 22}, {22, 23, 16},
    // Cross-ring connections {Midpoints}
    {1, 9, 17}, {3, 11, 19}, {5, 13, 21}, {7, 15, 23},
    // Cross-ring connections {Diagonals - Corners}
    {0, 8, 16}, {2, 10, 18}, {4, 12, 20}, {6, 14, 22}
};

const block_index_t IDX_TO_MILL_MAP[24][3][2] = {
    [0]  = { {  1, 2 },  {  8, 16 }, {  7,  6 } },
    [1]  = { {  0, 2 },  {  9, 17 } },
    [2]  = { {  0, 1 },  {  3,  4 }, { 10, 18 } },
    [3]  = { {  2, 4 },  { 11, 19 } },
    [4]  = { {  3, 2 },  {  5,  6 }, { 12, 20 } },
    [5]  = { {  4, 6 },  { 13, 21 } },
    [6]  = { {  4, 5 },  {  0,  7 }, { 14, 22 } },
    [7]  = { {  0, 6 },  { 15, 23 } },
    [8]  = { {  0, 16 }, { 14, 15 }, {  9, 10 } },
    [9]  = { {  1, 14 }, {  8, 10 } },
    [10] = { {  2, 18 }, {  8,  9 }, { 11, 12 } },
    [11] = { {  3, 19 }, { 10, 12 } },
    [12] = { {  4, 20 }, { 13, 14 }, { 11, 10 }, },
    [13] = { {  5, 21 }, { 12, 14 } },
    [14] = { {  6, 22 }, { 15, 16 }, { 13, 12 } },
    [15] = { {  7, 23 }, {  8, 14 } },
    [16] = { {  0,  8 }, { 17, 18 }, { 22, 23 } },
    [17] = { {  9,  1 }, { 16, 18 } },
    [18] = { { 17, 16 }, { 19, 20 }, { 10,  2 } },
    [19] = { { 11,  3 }, { 18, 20 } },
    [20] = { { 19, 18 }, { 12,  4 }, { 21, 22 } },
    [21] = { { 20, 22 }, { 13,  5 } },
    [22] = { { 14,  6 }, { 21, 20 }, { 23, 16 } },
    [23] = { { 15,  7 }, { 16, 22 } },
};

void game_init(game_state_t *game) {
    game->player_pieces_count[HUMAN] = PLAYER_PIECES_MAX;
    game->player_pieces_count[AI] = PLAYER_PIECES_MAX;
    game->turn = HUMAN;
    game->selected_block = -1;
    game->can_remove = false;
}

bool board_move_caused_mill(const board_t board, player_t player, block_index_t idx) {
    block_index_t mills[3][2];
    memcpy(mills, IDX_TO_MILL_MAP[idx], sizeof(mills));

    const int mills_count = ((idx & 1) == 0) ? 3 : 2;
    for (int i = 0; i < mills_count; i++) {
        block_index_t mill[2];
        memcpy(mill, mills[i], sizeof(mill));
        if (board[mill[0]] == player && board[mill[1]] == player)
            return true;
    }

    return false;
}

int board_count_mills(const board_t board, player_t player) {
    int count = 0;
    block_index_t mill[3];
    for (int i = 0; i < ARRAY_LEN(MILLS); i++) {
        bool is_mill = true;
        memcpy(mill, MILLS[i], sizeof(mill));
        for (int j = 0; j < ARRAY_LEN(mill); j++) {
            if (board[mill[j]] != player)
                is_mill = false;
        }
        if (is_mill)
            count++;
    }
    return count;
}

bool game_move_do(game_state_t *game, move_t move) {
    switch (move.type) {
        case NO_MOVE:
            return false;

        case PLACE:
            game->board[move.to] = game->turn;
            game->player_pieces_count[game->turn]--;
            break;

        case FLY:
            game->board[move.from] = EMPTY;
            game->board[move.to] = game->turn;
            break;
    }
    return board_move_caused_mill(game->board, game->turn, move.to);
}

bool game_remove_do(game_state_t *game, block_index_t idx) {
    player_t opponent = (game->turn == HUMAN) ? AI : HUMAN;
    if (game->board[idx] == opponent) {
        game->board[idx] = EMPTY;
#if ADD_REMOVED_PIECE_BACK == 1
        game->player_pieces_count[opponent]++;
#endif
        return true;
    }
    return false;
}

game_state_t *game_move_gen_next_states(game_state_t game, move_t move, player_t player) {
    game_state_t *new_states = NULL;
    player_t opponent = (player == HUMAN) ? AI : HUMAN;

    bool caused_mill = game_move_do(&game, move);
    if (caused_mill) {
        for (int i = 0; i < BOARD_BLOCKS_COUNT; i++) {
            if (game.board[i] == opponent) {
                game_state_t state_after_remove = game;
                game_remove_do(&state_after_remove, i);
                arrpush(new_states, state_after_remove);
            }
        }
        if (new_states == NULL) {
            arrpush(new_states, game);
        }
    } else {
        arrpush(new_states, game);
    }

    return new_states;
}

bool game_is_over(board_t board) {
    for (int i = 0; i < BOARD_BLOCKS_COUNT; i++)
        if (board[i] == EMPTY)
            return false;
    return true;
}

move_t *game_gen_valid_moves(board_t board, player_t player, int player_pieces_count) {
    int i, j;
    move_t *moves = NULL;

    if (player_pieces_count > 0) {
        for (i = 0; i < BOARD_BLOCKS_COUNT; i++) {
            if (board[i] == EMPTY)
                arrpush(moves, ((move_t){ .type = PLACE, .to = i }));
        }
    }

    for (i = 0; i < BOARD_BLOCKS_COUNT; i++) {
        if (board[i] == player) {
            for (j = 0; j < BOARD_BLOCKS_COUNT; j++) {
                if (board[j] == EMPTY)
                    arrpush(moves, ((move_t){ .type = FLY, .from = i, .to = j }));
            }
        }
    }

    return moves;
}

Vector2 board_pos_snap_to_grid(Vector2 pos) {
	const Vector2 v = {
		(float)((int)pos.x % BOARD_BLOCK_WIDTH),
		(float)((int)pos.y % BOARD_BLOCK_WIDTH),
	};
	return Vector2Subtract(pos, v);
}

block_index_t board_find_block_by_pos(board_t board, Vector2 pos) {
    Vector2 snapped_pos = board_pos_snap_to_grid(pos);
    for (int i = 0; i < ARRAY_LEN(BLOCKS_POS); i++) {
        if (Vector2Equals(BLOCKS_POS[i], snapped_pos))
            return i;
    }
    return -1;
}
