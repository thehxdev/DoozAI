package main

import "vendor:raylib"

PlayerTurn :: enum { HUMAN, AI }

Game :: struct {
	blocks: [24]Block,
	selected_block: ^Block,
	turn: PlayerTurn,
	score: [PlayerTurn]u8,
}

Block :: struct {
	pos: raylib.Vector2,
	color: raylib.Color,
	player: PlayerTurn,
}

board_draw_manually :: proc() {
	using raylib

	ClearBackground(BACKGROUND_COLOR)

	for line in BOARD_LINES {
		DrawLineEx(line.start_pos, line.end_pos, BOARD_LINES_THICKNESS, BOARD_COLOR)
	}

	// mask the lines in the center of board
	DrawRectangleRec(BOARD_SQUARES[2], BACKGROUND_COLOR)

	for rect in BOARD_SQUARES {
		DrawRectangleLinesEx(rect, BOARD_LINES_THICKNESS, BOARD_COLOR)
	}
}

snap_pos_to_grid :: #force_inline proc(pos: raylib.Vector2) -> raylib.Vector2 {
	v := raylib.Vector2{
		f32(i32(pos.x) % BOARD_BLOCK_WIDTH),
		f32(i32(pos.y) % BOARD_BLOCK_WIDTH),
	}
	return pos - v
}

board_get_block :: proc(blocks: []Block, pos: raylib.Vector2) -> (^Block, bool) {
	normalized_pos := snap_pos_to_grid(pos)
	for i in 0..<len(blocks) {
		b := &blocks[i]
		if b.pos == normalized_pos {
			return b, true
		}
	}
	return nil, false
}

block_toggle :: #force_inline proc(block: ^Block) {
	MAX_ALPHA :: 0xFF
	block.color.a ~= MAX_ALPHA
	// block.color.a = MAX_ALPHA if block.color.a == 0 else 0
}

block_is_active :: #force_inline proc(block: ^Block) -> bool {
	return block.color.a != 0
}

block_center_pos :: #force_inline proc(pos: raylib.Vector2) -> raylib.Vector2 {
	V :: raylib.Vector2{ BOARD_BLOCK_WIDTH/2, BOARD_BLOCK_WIDTH/2 }
	return pos + V
}

BOARD_SQUARES :: [3]raylib.Rectangle{
	{
		x = BOARD_BLOCK_WIDTH/2,
		y = BOARD_BLOCK_WIDTH/2,
		width = BOARD_BLOCK_WIDTH * 6,
		height = BOARD_BLOCK_WIDTH * 6,
	},
	{
		x = BOARD_BLOCK_WIDTH + (BOARD_BLOCK_WIDTH/2),
		y = BOARD_BLOCK_WIDTH + (BOARD_BLOCK_WIDTH/2),
		width = BOARD_BLOCK_WIDTH * 4,
		height = BOARD_BLOCK_WIDTH * 4,
	},
	{
		x = (BOARD_BLOCK_WIDTH*2) + (BOARD_BLOCK_WIDTH/2),
		y = (BOARD_BLOCK_WIDTH*2) + (BOARD_BLOCK_WIDTH/2),
		width = BOARD_BLOCK_WIDTH * 2,
		height = BOARD_BLOCK_WIDTH * 2,
	},
}

BOARD_LINES :: [4]struct { start_pos: raylib.Vector2, end_pos: raylib.Vector2 }{
	{
		start_pos = {
			BOARD_SQUARES[0].x + BOARD_LINES_THICKNESS,
			BOARD_SQUARES[0].y + BOARD_LINES_THICKNESS,
		},
		end_pos = {
			BOARD_SQUARES[0].x + BOARD_SQUARES[0].width - BOARD_LINES_THICKNESS,
			BOARD_SQUARES[0].y + BOARD_SQUARES[0].height - BOARD_LINES_THICKNESS,
		}
	},
	{
		start_pos = {
			BOARD_SQUARES[0].x + BOARD_SQUARES[0].width/2,
			BOARD_SQUARES[0].y,
		},
		end_pos = {
			BOARD_SQUARES[0].x + BOARD_SQUARES[0].width/2,
			BOARD_SQUARES[0].y + BOARD_SQUARES[0].height,
		}
	},
	{
		start_pos = {
			BOARD_SQUARES[0].x,
			BOARD_SQUARES[0].y + BOARD_SQUARES[0].height/2,
		},
		end_pos = {
			BOARD_SQUARES[0].x + BOARD_SQUARES[0].width,
			BOARD_SQUARES[0].y + BOARD_SQUARES[0].height/2,
		}
	},
	{
		start_pos = {
			BOARD_SQUARES[0].x + BOARD_LINES_THICKNESS,
			BOARD_SQUARES[0].y + BOARD_SQUARES[0].height - BOARD_LINES_THICKNESS,
		},
		end_pos = {
			BOARD_SQUARES[0].x + BOARD_SQUARES[0].width - BOARD_LINES_THICKNESS,
			BOARD_SQUARES[0].y + BOARD_LINES_THICKNESS,
		}
	},
}

// NOTE: The order of blocks here is circular starting from outer square and top-left block
// and in clockwise order.
// Then the middle and inner square.
BOARD_BLOCKS :: [24]Block{
	// Outer square
	{
		pos = {
			0,
			0,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
			0,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-1),
			0,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-1),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-1),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-1),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-1),
		},
	},
	{
		pos = {
			0,
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-1),
		},
	},
	{
		pos = {
			0,
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
		},
	},


	// Middle square
	{
		pos = {
			BOARD_BLOCK_WIDTH,
			BOARD_BLOCK_WIDTH,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
			BOARD_BLOCK_WIDTH,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-2),
			BOARD_BLOCK_WIDTH,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-2),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-2),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-2),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-2),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH,
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-2),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH,
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
		},
	},

	// Inner  square
	{
		pos = {
			BOARD_BLOCK_WIDTH*2,
			BOARD_BLOCK_WIDTH*2,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
			BOARD_BLOCK_WIDTH*2,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-3),
			BOARD_BLOCK_WIDTH*2,
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-3),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-3),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-3),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-3),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*2,
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT-3),
		},
	},
	{
		pos = {
			BOARD_BLOCK_WIDTH*2,
			BOARD_BLOCK_WIDTH*(BOARD_BLOCK_COUNT/2),
		},
	},
}
