/**
 * FIXME: Currently `place/fly and then remove` kind of moves are atomic and cannot
 * be processsed seperately. This is because for each type of move (place or fly),
 * the game generates a new state and handles removal in the generation phase.
 * So removing is not a seperate type of move. This causes an issue that we can't
 * use `ai_best_move` to encode all types of moves.
 * So currently we use `ai_best_next_state` to return an entire new state after
 * AI has completed it's turn.
 */

 #include <limits.h>
 #include "ai.h"
 #include "stb_ds.h"

int ai_evaluate(game_state_t game) {
    int human_mills = board_count_mills(game.board, HUMAN);
    int ai_mills = board_count_mills(game.board, AI);

    int human_pieces = PLAYER_PIECES_MAX - game.player_pieces_count[HUMAN];
    int ai_pieces = PLAYER_PIECES_MAX - game.player_pieces_count[AI];
    return ((ai_mills - human_mills) * 16) + ((ai_pieces - human_pieces) * 4);
}

int ai_minimax(game_state_t game, int depth, int alpha, int beta, bool maximizing) {
    if (game_is_over(game.board))
        return 0;
    if (depth == 0)
        return ai_evaluate(game);

    if (maximizing) {
        // AI
        int max_eval = INT_MIN;
        move_t *moves = game_gen_valid_moves(game.board, AI, game.player_pieces_count[AI]);
        if (!moves)
            return INT_MIN;

        int moves_len = arrlen(moves);
        for (int i = 0; i < moves_len; i++) {
            game_state_t *next_states = game_move_gen_next_states(game, moves[i], AI);
            if (!next_states)
                continue;

            int current_best_move = INT_MIN;
            int next_states_len = arrlen(next_states);
            for (int j = 0; j < next_states_len; j++) {
                int eval_score = ai_minimax(next_states[j], depth-1, alpha, beta, false);
                current_best_move = MAX(current_best_move, eval_score);
            }
            arrfree(next_states);

            max_eval = MAX(max_eval, current_best_move);
            alpha = MAX(alpha, current_best_move);
            if (beta <= alpha)
                break;
        }

        arrfree(moves);
        return max_eval;
    } else {
        // Human
        int min_eval = INT_MAX;
        move_t *moves = game_gen_valid_moves(game.board, HUMAN, game.player_pieces_count[HUMAN]);
        if (!moves)
            return INT_MAX;

        int moves_len = arrlen(moves);
        for (int i = 0; i < moves_len; i++) {
            game_state_t *next_states = game_move_gen_next_states(game, moves[i], HUMAN);
            if (!next_states)
                continue;

            int current_best_move = INT_MAX;
            int next_states_len = arrlen(next_states);
            for (int j = 0; j < next_states_len; j++) {
                int eval_score = ai_minimax(next_states[j], depth-1, alpha, beta, true);
                current_best_move = MIN(current_best_move, eval_score);
            }
            arrfree(next_states);

            min_eval = MIN(min_eval, current_best_move);
            beta = MIN(beta, current_best_move);
            if (beta <= alpha)
                break;
        }

        arrfree(moves);
        return min_eval;
    }
}

game_state_t ai_best_next_state(game_state_t game, int depth) {
    game_state_t best_state = game;
    move_t *moves = game_gen_valid_moves(game.board, AI, game.player_pieces_count[AI]);
    if (moves == NULL)
        goto ret;

    int best_score = INT_MIN;
    int moves_len = arrlen(moves);

    for (int i = 0; i < moves_len; i++) {
        int branch_best_score = INT_MIN;
        game_state_t *next_states = game_move_gen_next_states(game, moves[i], AI);
        if (next_states == NULL)
            continue;

        int next_states_len = arrlen(next_states);
        for (int j = 0; j < next_states_len; j++) {
            int branch_score = ai_minimax(next_states[j], depth-1, INT_MIN, INT_MAX, false);
            branch_best_score = MAX(branch_best_score, branch_score);
            if (branch_best_score > best_score) {
                best_score = branch_best_score;
                best_state = next_states[j];
            }
        }

        arrfree(next_states);
    }

    arrfree(moves);
ret:
    return best_state;
}

#if 0
move_t ai_best_move(game_state_t game, int depth) {
    move_t *moves = game_gen_valid_moves(game.board, AI, game.player_pieces_count[AI]);
    if (moves == NULL)
        return ((move_t){ 0 });

    move_t best_move = { 0 };
    int best_score = INT_MIN;
    int moves_len = arrlen(moves);

    for (int i = 0; i < moves_len; i++) {
        int branch_best_score = INT_MIN;
        game_state_t *next_states = game_move_gen_next_states(game, moves[i], AI);
        if (next_states == NULL)
            continue;

        int next_states_len = arrlen(next_states);
        for (int j = 0; j < next_states_len; j++) {
            int branch_score = ai_minimax(next_states[j], depth-1, INT_MIN, INT_MAX, false);
            branch_best_score = MAX(branch_best_score, branch_score);
        }
        arrfree(next_states);

        if (branch_best_score > best_score) {
            best_score = branch_best_score;
            best_move = moves[i];
        }
    }

    arrfree(moves);
    return best_move;
}
#endif // ai_best_move
