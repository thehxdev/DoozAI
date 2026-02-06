static Texture background_texture;

bool gfx_load_background_texture(const char *image_path) {
    bool ok;
    Image image = LoadImage(image_path);
    if (!(ok = IsImageValid(image)))
        return ok;
    background_texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return IsTextureValid(background_texture);
}

void gfx_unload_background_texture(void) {
    UnloadTexture(background_texture);
}

void gfx_board_draw(board_t board, Color texture_tint) {
    DrawTexture(background_texture, 0, 0, texture_tint);
    Vector2 pos;
    for (int i = 0; i < STH_BASE_ARRAY_LEN(BLOCKS_POS); i++) {
        if (board[i] == EMPTY)
            continue;
        pos = BLOCKS_POS[i];
        DrawCircleV(Vector2Add(pos, BLOCK_HALF), PIECE_RADIUS,
                    (board[i] == HUMAN) ? PIECE_COLOR_HUMAN : PIECE_COLOR_AI );
    }
}
