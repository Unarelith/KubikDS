#include "level.h"
#include "game.h"

int Level::nbLevels = 0;

Level::Level(int id, Map* map, int bg) {
	s_id = id;
	s_map = map;
	s_bg = bg;
	
	if(s_map->map == 0) {
		// Reset nbEnemies count
		Enemy::nbEnemies = 0;
		
		// Fill level's map and enemies
		fill_map(s_map);
	}
	
	s_scrollX = s_map->scrollX;
	s_scrollY = s_map->scrollY;
	
	nbLevels++;
}

Level::~Level() {
	free(s_map->map);
}

void Level::initializeBg() {
	u16* mapPtr = bgGetMapPtr(s_bg);
	
	dmaCopy(tilesTiles, bgGetGfxPtr(s_bg), tilesTilesLen);
	dmaCopy(tilesPal, BG_PALETTE, tilesPalLen);
	
	int i, j;
	for(j = 0 ; j < 32 ; j++) {
		for(i = 0 ; i < 32 ; i++) {
			mapPtr[i + j * 32] = s_map->map[(i + s_scrollX / 8) + (j + s_scrollY / 8) * s_map->length];
			mapPtr[i + j * 32 + 32 * 32] = s_map->map[(i + 32 + s_scrollX / 8) + (j + s_scrollY / 8) * s_map->length];
		}
	}
}

bool Level::isKubeAt(s16 x, s16 y) {
	if(s_map->map[(x + s_scrollX) / 8 + (y + s_scrollY) / 8 * s_map->length] == 1) {
		return true;
	}
	return false;
}

bool Level::isFinishAt(s16 x, s16 y) {
	if(s_map->map[(x + s_scrollX) / 8 + (y + s_scrollY) / 8 * s_map->length] == 2) {
		return true;
	}
	return false;
}

void Level::scroll(s32 x, s32 y, bool absolute) {
	if(absolute) {
		s_scrollX = x;
		s_scrollY = y;
	} else {
		s_scrollX += x;
		s_scrollY += y;
	}
	
	u16* mapPtr = bgGetMapPtr(s_bg);
	
	/* TODO: 
		<ludo6431> bon ensuite, tu peux optimiser un peu tout ça, au lieu de tout recopier à chaque déplacement, tu peux scroller un peu tant que tu n'arrives pas à la fin des 512 pixels
		<ludo6431> et à ce moment là tu recharges la suite de ta map et tu reviens au début
	*/
	int i, j;
	for(j = 0 ; j < 32 ; j++) {
		for(i = 0 ; i < 32 ; i++) {
			mapPtr[i + j * 32] = s_map->map[(i + s_scrollX / 8) + (j + s_scrollY / 8) * s_map->length];
			mapPtr[i + j * 32 + 32 * 32] = s_map->map[(i + 32 + s_scrollX / 8) + (j + s_scrollY / 8) * s_map->length];
		}
	}
}

void Level::resetScrolling() {
	s_scrollX = s_map->scrollX;
	s_scrollY = s_map->scrollY;
}
