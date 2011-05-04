#include "level.h"

Level::Level(Map* map, int bg, int id) {
	s_id = id;
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
	for(j = 0 ; j < 32 ; j++) {
		for(i = 0 ; i < 32 ; i++) {
			mapPtr[i + j * 32] = s_map->map[i + j * s_length];
			mapPtr[i + 32 + j * 32 + 32 * 32] = s_map->map[(i + 32) + j * s_length];
		}
	}
	
}

Level::~Level() {
}

void Level::scroll(s16 x, s16 y) {
	s_x += x;
	s_y += y;
	
	u16* mapPtr = bgGetMapPtr(s_bg);
	
	/* TODO: 
		<ludo6431> bon ensuite, tu peux optimiser un peu tout ça, au lieu de tout recopier à chaque déplacement, tu peux scroller un peu tant que tu n'arrives pas à la fin des 512 pixels
		<ludo6431> et à ce moment là tu recharges la suite de ta map et tu reviens au début
	*/
	int i, j;
	for(j = 0 ; j < 32 ; j++) {
		for(i = 0 ; i < 32 ; i++) {
			mapPtr[i + j * 32] = s_map->map[(i + s_x / 8) + (j + s_y / 8) * s_length];
			mapPtr[i + j * 32 + 32 * 32] = s_map->map[(i + 32 + s_x / 8) + (j + s_y / 8) * s_length];
		}
	}
	
	bgSetScroll(s_bg, s_x % 8, s_y % 8);
}


void Level::testCollisionsPE() {
	int i;
	for(i = 0 ; i < s_nbEnemies ; i++) {
		s16 px = s_player->x();
		s16 py = s_player->y();
		s16 ex = s_enemies[i]->x();
		s16 ey = s_enemies[i]->y();
		
		s_player->start();
		s_enemies[i]->start();
		
		if((px + 7 >= ex) && (px <= ex + 7) && (py <= ey + 7) && (py + 7 >= ey)) {
			s_player->removeLife();
		}
	}
}

bool Level::isKubeAt(s16 x, s16 y) {
	if(s_map->map[(x + s_x) / 8 + (y + s_y) / 8 * s_length] == 1) {
		return true;
	}
	
	return false;
}
