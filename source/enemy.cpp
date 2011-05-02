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
