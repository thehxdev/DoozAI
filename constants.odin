package main

import "vendor:raylib"

// Game board is a 7x7 grid each 140 pixel in size
BOARD_BLOCK_COUNT :: 7
BOARD_BLOCK_WIDTH :: 140
BOARD_LINES_THICKNESS :: 3
SQUARE_BLOCK_COUNT :: 8

WINDOW_TITLE  :: "Dooz AI"
WINDOW_WIDTH  :: BOARD_BLOCK_COUNT * BOARD_BLOCK_WIDTH
WINDOW_HEIGHT :: WINDOW_WIDTH
TARGET_FPS    :: 60

CIRCLE_RADIUS :: BOARD_BLOCK_WIDTH/4

SELECTION_THICKNESS :: 10
SELECTION_INITIAL_RADIUS :: CIRCLE_RADIUS + 15

// https://colorhunt.co/palette/f5f2f2feb05d5a7acd2b2a2a
BACKGROUND_COLOR :: raylib.Color{ 0x18, 0x18, 0x18, 0xFF }
// AI_COLOR         :: raylib.Color{ 0xFE, 0xB0, 0x5D, 0xFF }
// HUMAN_COLOR      :: raylib.Color{ 0x5A, 0x7A, 0xCD, 0xFF }
// SELECTION_COLOR  :: raylib.Color{ 0xF9, 0x6E, 0x5B, 0xFF }
BOARD_COLOR      :: raylib.Color{ 0xF5, 0xF2, 0xF2, 0xFF }

AI_COLOR         :: raylib.RED
HUMAN_COLOR      :: raylib.BLUE
SELECTION_COLOR  :: raylib.PURPLE
