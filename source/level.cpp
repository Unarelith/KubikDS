#include "level.h"
#include "game.h"

int Level::nbLevels = 0;

Level::Level(int id, Map* map) {
	s_id = id;
	s_map = map;
	
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

void Level::initializeBg(int bg, bool main) {
	s_bg = bg;
	
	u16* mapPtr = bgGetMapPtr(bg);
	
	dmaCopy(tilesTiles, bgGetGfxPtr(bg), tilesTilesLen);
	
	if(main) {
		dmaCopy(tilesPal, BG_PALETTE, tilesPalLen);
	} else {
		dmaCopy(tilesPal, BG_PALETTE_SUB, tilesPalLen);
		
		// Took from console.c to resolve some palette issues
		BG_PALETTE_SUB[1 * 16 - 1] = RGB15(0,0,0); //30 normal black
		BG_PALETTE_SUB[2 * 16 - 1] = RGB15(15,0,0); //31 normal red
		BG_PALETTE_SUB[3 * 16 - 1] = RGB15(0,15,0); //32 normal green
		BG_PALETTE_SUB[4 * 16 - 1] = RGB15(15,15,0); //33 normal yellow

		BG_PALETTE_SUB[5 * 16 - 1] = RGB15(0,0,15); //34 normal blue
		BG_PALETTE_SUB[6 * 16 - 1] = RGB15(15,0,15); //35 normal magenta
		BG_PALETTE_SUB[7 * 16 - 1] = RGB15(0,15,15); //36 normal cyan
		BG_PALETTE_SUB[8 * 16 - 1] = RGB15(24,24,24); //37 normal white

		BG_PALETTE_SUB[9  * 16 - 1] = RGB15(15,15,15); //40 bright black
		BG_PALETTE_SUB[10 * 16 - 1] = RGB15(31,0,0); //41 bright red
		BG_PALETTE_SUB[11 * 16 - 1] = RGB15(0,31,0); //42 bright green
		BG_PALETTE_SUB[12 * 16 - 1] = RGB15(31,31,0);	//43 bright yellow
		
		BG_PALETTE_SUB[13 * 16 - 1] = RGB15(0,0,31); //44 bright blue
		BG_PALETTE_SUB[14 * 16 - 1] = RGB15(31,0,31);	//45 bright magenta
		BG_PALETTE_SUB[15 * 16 - 1] = RGB15(0,31,31);	//46 bright cyan
		BG_PALETTE_SUB[16 * 16 - 1] = RGB15(31,31,31); //47 & 39 bright white
		
		BG_PALETTE_SUB[0] = RGB15(0,0,0);
	}
	
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

void Level::setKube(s16 x, s16 y, u16 tile) {
	bgGetMapPtr(s_bg)[x + y * 32] = tile;
	//s_map->map[(x + s_scrollX / 8) + (y + s_scrollY / 8) * s_map->length] = tile;
}
