#define MINIMAX_DEPTH_MAX   4

static void usage(const char *prog_name);
static int parse_cli_args(int argc, char *argv[]);
static bool handle_human_turn(void);

static char *background_image_path = "./assets/board.png";
static game_state_t game;

int main(int argc, char *argv[]) {
    int ret = EXIT_FAILURE;
    if (argc == 2)
        background_image_path = argv[1];

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dooz AI!");
    SetTargetFPS(TARGET_FPS);
    if (!gfx_load_background_texture(background_image_path)) {
        usage(argv[0]);
        goto ret_close_window;
    }

    memset(&game, 0, sizeof(game));
    game_init(&game);

    while (!WindowShouldClose()) {
        if (handle_human_turn())
            game.turn = AI;

        BeginDrawing();
        {
            gfx_board_draw(game.board, (game.can_remove) ? PURPLE : ((game.turn == HUMAN) ? BLUE : RED));
            if (game.selected_block != -1) {
                DrawCircleV(Vector2Add(BLOCKS_POS[game.selected_block], BLOCK_HALF),
                            PIECE_SELECTION_RADIUS,
                            (Color){0xff, 0xff, 0xff, 0x80});
            }
        }
        EndDrawing();

        if (game.turn == AI) {
            game = ai_best_next_state(game, MINIMAX_DEPTH_MAX);
            game.turn = HUMAN;
        }
    }

    ret = EXIT_SUCCESS;
    gfx_unload_background_texture();
ret_close_window:
    CloseWindow();
ret:
    return ret;
}

static bool handle_human_turn(void) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && game.turn == HUMAN) {
        block_index_t block_index = board_find_block_by_pos(game.board, GetMousePosition());
        if (block_index == -1)
            return false;

        if (game.can_remove) {
            if (game.board[block_index] != AI)
                return false;
            game_remove_do(&game, block_index);
            game.can_remove = false;
            return true;
        }

        if (game.board[block_index] == HUMAN) {
            game.selected_block = (game.selected_block == -1) ? block_index : -1;
            return false;
        } else if (game.board[block_index] == EMPTY) {
            move_t move = { 0 };

            if (game.selected_block != -1) {
                move.type = FLY;
                move.from = game.selected_block;
                move.to = block_index;
                game.selected_block = -1;
            } else {
                move.type = PLACE;
                move.to = block_index;
            }

            bool caused_mill = game_move_do(&game, move);
            if (caused_mill) {
                game.can_remove = true;
                return false;
            }

            return true;
        }
    }
    return false;
}

static void usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s [background image path]\n", prog_name);
}
