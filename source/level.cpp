#include "level.h"

Level::Level(Map* map, int bg) {
	s_length = map->length;
	s_height = map->height;
	s_map = map;
	s_color = map->color;
	s_bg = bg;
	s_x = 0;
	s_y = 0;
	
	u16* mapPtr = bgGetMapPtr(s_bg);
	
	dmaCopy(tilesTiles, bgGetGfxPtr(s_bg), tilesTilesLen);
	dmaCopy(tilesPal, BG_PALETTE, tilesPalLen);
	
	int i, j;
	for(j = 0 ; j < 24 ; j++)
		for(i = 0 ; i < 32 ; i++)
			mapPtr[i + j * 32] = s_map->map[i + j * s_length];
	
}

Level::~Level() {
}

void Level::scroll(s16 x, s16 y) {
	s_x += x;
	s_y += y;
	
	u16* mapPtr = bgGetMapPtr(s_bg);
	
	int i, j;
	for(j = 0 ; j < 24 ; j++)
		for(i = 0 ; i < 32 ; i++)
			mapPtr[i + j * 32] = s_map->map[(i + s_x / 8) + (j + s_y / 8) * s_length];
	
	bgSetScroll(s_bg, s_x % 8, s_y % 8);
}

bool Level::isKubeAt(s16 x, s16 y) {
	if(s_map->map[(x + s_x) / 8 + (y + s_y) / 8 * s_length] == 1) {
		return true;
	}
	
	return false;
}
