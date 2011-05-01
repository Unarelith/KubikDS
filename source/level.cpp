#include "level.h"

Level::Level(Map* map, u16* bgGfx) {
	s_length = map->length;
	s_height = map->height;
	s_map = map;
	s_color = map->color;
	s_bgGfx = bgGfx;
	s_x = 8;
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
	for(y = s_y / 8 ; y < s_y / 8 + 24 ; y++) {
		for(x = s_x / 8 ; x < s_x / 8 + 32 ; x++) {
			if(s_map->map[x + y * s_length] == 1) {
				drawKube(x*8, y*8, s_color);
			}
		}
	}
	
	bgSetScroll(2, s_x, s_y);
}

bool Level::isKubeAt(s16 x, s16 y) {
	if(s_map->map[x / 8 + y / 8 * s_length] == 1) {
		return true;
	}
	
	return false;
}
