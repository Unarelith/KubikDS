#include "cursor.h"

u16 cursorMap[ 8 * 8 * 2 ] = {
	1,1,1,2,2,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	2,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,2,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,2,2,1,1,1
};

Cursor::Cursor() : Sprite(127, SpriteSize_8x8, SpriteColorFormat_Bmp, 0, 0) {
	u16 u;
	for(u = 0 ; u < 8 * 8 * 2 ; u++) {
		switch(cursorMap[u]) {
			case 1:
				s_gfx[u] = ARGB16(1,31,31,31);
				break;
			case 2:
				s_gfx[u] = ARGB16(1,0,0,0);
				break;
			default:
				break;
		}
	}
}

Cursor::~Cursor() {
	oamClearSprite(s_oam, s_i);
	u16 u;
	for(u = 0 ; u < 8 * 8 ; u++) {
		s_gfx[u] = 0;
	}
	oamFreeGfx(s_oam, s_gfx);
	oamUpdate(s_oam);
}

void Cursor::move() {
	if((keysDown() & KEY_UP) && (s_y > 0)) {
		s_y -= 8;
	}
	if((keysDown() & KEY_DOWN) && (s_y < (TILES_NB - 1) * 8 / 32)) { //191 - 8)) {
		s_y += 8;
	}
	if((keysDown() & KEY_LEFT) && (s_x > 0)) {
		s_x -= 8;
	}
	if((keysDown() & KEY_RIGHT) && (s_x < (TILES_NB - 1) * 8)) { //255 - 8)) {
		s_x += 8;
	}
}
