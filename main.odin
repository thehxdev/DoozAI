package main

import "core:sync"
import "core:thread"
import "core:strings"
import "core:math"
import "core:fmt"
import "vendor:raylib"
import "gfx"

_ :: strings
_ :: math
_ :: fmt
_ :: raylib
_ :: gfx

BOARD_IMAGE_DATA :: #load("./game-asset/board.png")

game: Game
human_played: sync.Cond
font: raylib.Font

main :: proc() {
	using raylib

	board_texture: Texture
	should_draw_debug_info: bool = false

	SetTraceLogLevel(.WARNING)
	SetConfigFlags(ConfigFlags{.MSAA_4X_HINT})
	SetTargetFPS(TARGET_FPS)

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
	defer CloseWindow()

	font = GetFontDefault()
	game.blocks = BOARD_BLOCKS

	{
		board_image := LoadImageFromMemory(".png", raw_data(BOARD_IMAGE_DATA), cast(i32) len(BOARD_IMAGE_DATA))
		board_texture = LoadTextureFromImage(board_image)
		UnloadImage(board_image)
	}
	defer UnloadTexture(board_texture)

	ai_thread := thread.create_and_start(ai_routine)
	defer {
		thread.terminate(ai_thread, 0)
		thread.destroy(ai_thread)
	}

	for !WindowShouldClose() {
		// User input (human player turn)
		if IsMouseButtonPressed(.LEFT) && game.turn == .HUMAN {
			block, found := board_get_block(game.blocks[:], GetMousePosition())
			if found {
				if game.selected_block != nil {
					// Selection state
					if game.selected_block.pos != block.pos && !block_is_active(block) {
						block.color = HUMAN_COLOR
						block.player = game.turn
						game.turn = .AI
						block_toggle(game.selected_block)
						sync.signal(&human_played)
					}
					game.selected_block = nil
				} else {
					if block.player == .HUMAN && block_is_active(block) {
						// Go to selection state
						game.selected_block = block
					} else {
						if !block_is_active(block) {
							block.color = HUMAN_COLOR
							block.player = game.turn
							game.turn = .AI
							sync.signal(&human_played)
						}
					}
				}
			}
		}

		if IsKeyPressed(.D) {
			should_draw_debug_info = !should_draw_debug_info
		}

		// Draw
		BeginDrawing()
		{
			tint := gfx.texture_rgb_tint_color() when WITH_RGB_BOARD else DARKGRAY
			DrawTexture(board_texture, 0, 0, tint)

			for b in game.blocks {
				DrawCircleV(block_center_pos(b.pos), CIRCLE_RADIUS, b.color)
			}

			if game.selected_block != nil {
				sb := game.selected_block
				gfx.draw_circle_lineV(block_center_pos(sb.pos), SELECTION_INITIAL_RADIUS, SELECTION_THICKNESS, .OUTLINED, SELECTION_COLOR)
			}

			when ODIN_DEBUG {
				if should_draw_debug_info {
					draw_debug_info()
				}
			}

			DrawFPS(5, 5)
		}
		EndDrawing()
	}
}

// AI logic
ai_routine :: proc() {
	@(static) c := 0
	mu: sync.Mutex

	for {
		sync.cond_wait(&human_played, &mu)

		for game.blocks[c].color.a != 0 { c = (c + 1) %% 24 }

		game.blocks[c].color = AI_COLOR
		game.blocks[c].player = game.turn
		game.turn = .HUMAN
	}
}

draw_debug_info :: proc() {
	using raylib

	when WITH_GRIDLINE {
		GRIDLINE_THICKNESS :: 2
		GRIDLINE_COLOR     :: raylib.DARKBLUE
		for i: i32 = 1; i < BOARD_BLOCK_COUNT; i += 1 {
			start_pos := Vector2{ f32(i * BOARD_BLOCK_WIDTH), 0 }
			end_pos := Vector2{ f32(i * BOARD_BLOCK_WIDTH), WINDOW_HEIGHT}
			DrawLineEx(start_pos, end_pos, GRIDLINE_THICKNESS, GRIDLINE_COLOR)
			start_pos.x, start_pos.y = start_pos.y, start_pos.x
			end_pos.x, end_pos.y = end_pos.y, end_pos.x
			DrawLineEx(start_pos, end_pos, GRIDLINE_THICKNESS, GRIDLINE_COLOR)
		}
	}

	{
		FONT_SIZE :: 30
		sb: strings.Builder
		strings.builder_init(&sb, 0, 32, context.temp_allocator)
		defer strings.builder_destroy(&sb)

		mouse_pos := GetMousePosition()
		fmt.sbprintf(&sb, "x = %d | y = %d", i32(mouse_pos.x), i32(mouse_pos.y))
		text_cstring := cstring(raw_data(sb.buf))

		text_size := MeasureTextEx(font, text_cstring, FONT_SIZE, 0)
		DrawText(text_cstring, (WINDOW_WIDTH/2) - (i32(text_size.x)/2), 20, FONT_SIZE, RED)
	}

	free_all(context.temp_allocator)
}
