#include "level.h"

Level::Level(u16 length, u8 map[], u16 color, u16* bgGfx) {
	s_length = length;
	s_height = 24;
	s_map = map;
	s_color = color;
	s_bgGfx = bgGfx;
	s_x = 0;
	s_y = 0;
}

Level::~Level() {
}

void Level::drawKube(s16 x, s16 y, u16 color) {
	int xx, yy;
	for(yy = 0 ; yy < 8 ; yy++) {
		for(xx = 0 ; xx < 8 ; xx++) {
			s_bgGfx[(x + xx) + (y + yy) * 256] = color;
		}
	}
}

void Level::draw() {
	u16 x, y;
	for(y = 0 ; y < s_height ; y++) {
		for(x = 0 ; x < s_length ; x++) {
			if(s_map[x + y * 32] == 1) {
				drawKube(x*8, y*8, s_color);
			}
		}
	}
}

bool Level::isKubeAt(s16 x, s16 y) {
	if(s_map[x / 8 + y / 8 * 32] == 1) {
		return true;
	}
	
	return false;
}
