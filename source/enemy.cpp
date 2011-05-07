#include "enemy.h"

int Enemy::nbEnemies = 0;

Enemy::Enemy(int i, s16 x, s16 y, s32 vx, s32 vy) : Sprite(i, SpriteSize_8x8, SpriteColorFormat_Bmp, x, y) {
    dmaFillHalfWords(ARGB16(1,31,0,0), s_gfx, 8*8*2); // Copy player data into sprite gfx
	
	// Initialize other members
	s_ax = x;
	s_ay = y;
	s_vx = vx;
	s_vy = vy;
	
	nbEnemies++;
}

Enemy::~Enemy() {
	oamFreeGfx(&oamMain, s_gfx);
}

void Enemy::draw() {
	if((s_x + 7 > 0) && (s_x < 256 + s_level->scrollX()) && (s_y + 7 >= 0) && (s_y <= 192 + s_level->scrollY())) { // If the sprite is on the screen
		// Draw the sprite on the screen
		oamSet(&oamMain, s_i, s_x, s_y, 0,
			   15, s_size, s_format, s_gfx, -1,
			   false, false, false, false, false);
	} else {
		oamClearSprite(&oamMain, s_i); // Clear the sprite
	}
}

void Enemy::move() {
	// Horizontal movement
	if((s_level->isKubeAt(s_x + s_vx, s_y)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y)) || (s_level->isKubeAt(s_x + s_vx, s_y + 7)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y + 7))) {
		s_vx = -s_vx;
	}
	
	s_ax += s_vx;
	s_x = s_ax - s_level->scrollX();
	
	// Vertical movement
	if((s_level->isKubeAt(s_x, s_y + s_vy)) || (s_level->isKubeAt(s_x, s_y + 7 + s_vy)) || (s_level->isKubeAt(s_x + 7, s_y + s_vy)) || (s_level->isKubeAt(s_x + 7, s_y + 7 + s_vy))) {
		s_vy = -s_vy;
	}
	
	s_ay += s_vy;
	s_y = s_ay - s_level->scrollY();
}
