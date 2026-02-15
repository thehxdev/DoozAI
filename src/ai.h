#ifndef _DOOZ_AI_H_
#define _DOOZ_AI_H_

#include <stdbool.h>
#include "game.h"

int ai_evaluate(game_state_t game);

int ai_minimax(game_state_t game, int depth, int alpha, int beta, bool maximizing);

game_state_t ai_best_next_state(game_state_t game, int depth);

#endif // !_DOOZ_AI_H_
