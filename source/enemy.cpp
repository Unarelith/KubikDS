#include "enemy.h"

Enemy::Enemy(int i, s16 x, s16 y) : Sprite(i, SpriteSize_8x8, SpriteColorFormat_Bmp, x, y) {
    dmaFillHalfWords(ARGB16(1,31,5,5), s_gfx, 8*8*2);
	s_vx = 1;
	s_lx = x;
	s_ly = y;
}

Enemy::~Enemy() {
}

void Enemy::move() {
	if((s_level->isKubeAt(s_x + s_vx, s_y)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y)) || (s_level->isKubeAt(s_x + s_vx, s_y + 7)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y + 7)))
		s_vx = -s_vx;
	
	s_lx += s_vx;
	s_x = s_lx - s_level->x();
}

void Enemy::draw() {
<<<<<<< HEAD
	if((s_x >= 0) && (s_x <= 256 + s_level->x() + 7) && (s_y >= 0) && (s_y <= 192 + s_level->y() + 7)) {
=======
	if((s_x >= 0) && (s_x <= 256 + s_level->x() + 8) && (s_y >= 0) && (s_y <= 192 + s_level->y() + 8)) {
>>>>>>> 5e6f53e7fa1f555dcf36e58d4dcbadbfdab08068
		oamSet(&oamMain, s_i, s_x, s_y, 0,
			   15, s_size, s_format, s_gfx, -1,
			   false, false, false, false, false);
	} else {
		oamClearSprite(&oamMain, s_i);
	}
}
