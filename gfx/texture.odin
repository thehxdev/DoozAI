package gfx

import "core:math"
import "vendor:raylib"

texture_rgb_tint_color :: proc() -> raylib.Color {
	using raylib

	now := f32(GetTime())
	c := Vector4{
		.5 + math.sin(now) / 2,
		.5 + math.sin(now + math.PI * 2 / 3) / 2,
		.5 + math.sin(now + math.PI * 4 / 3) / 2,
		1,
	}
	return ColorFromNormalized(c)
}

