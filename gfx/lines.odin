package gfx

import "vendor:raylib"

_ :: raylib

LINE_THICKNESS_STEP :: 0.5

LineDrawingStrategy :: enum {
	CENTERED,
	OUTLINED,
}

draw_circle_lineV :: proc(pos: raylib.Vector2, radius, thickness: f32, kind: LineDrawingStrategy, color: raylib.Color) {
	draw_circle_line(i32(pos.x), i32(pos.y), radius, thickness, kind, color)
}

draw_circle_line :: proc(centerX, centerY: i32, radius, thickness: f32, kind: LineDrawingStrategy, color: raylib.Color) {
	using raylib

	switch kind {
	case .OUTLINED:
		for i: f32 = 0; i < thickness; i += LINE_THICKNESS_STEP {
			DrawCircleLines(centerX, centerY, radius + i, color)
		}

	case .CENTERED:
		t := thickness/2
		for i: f32 = 0; i < t; i += LINE_THICKNESS_STEP {
			DrawCircleLines(centerX, centerY, radius + i, color)
			DrawCircleLines(centerX, centerY, radius - i, color)
		}
	}
}
