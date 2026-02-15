#ifndef _DOOZ_GFX_H_
#define _DOOZ_GFX_H_

#include <stdbool.h>
#include "game.h"

bool gfx_load_background_texture(const char *image_path);

void gfx_unload_background_texture(void);

void gfx_board_draw(board_t board, Color texture_tint);

#endif // !_DOOZ_GFX_H_
