#include "cursor.h"

u16 cursorMap[ 8 * 8 * 2 ] = {
	1,1,1,0,0,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,0,0,1,1,1
};

Cursor::Cursor() : Sprite(127, SpriteSize_8x8, SpriteColorFormat_Bmp, 0, 0) {
	u16 u;
	for(u = 0 ; u < 8 * 8 * 2 ; u++) {
		if(cursorMap[u]) {
			s_gfx[u] = ARGB16(1,31,31,31);
		}
	}
}

Cursor::~Cursor() {
}

void Cursor::move() {
	if((keysDown() & KEY_UP) && (s_y > 0)) {
		s_y -= 8;
	}
	if((keysDown() & KEY_DOWN) && (s_y < 191 - 8)) {
		s_y += 8;
	}
	if((keysDown() & KEY_LEFT) && (s_x > 0)) {
		s_x -= 8;
	}
	if((keysDown() & KEY_RIGHT) && (s_x < 255 - 8)) {
		s_x += 8;
	}
}
