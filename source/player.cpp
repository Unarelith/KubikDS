#include "player.h"
#include "game.h"

bool Player::a = false;
int Player::frame = 0;

Player::Player(s16 x, s16 y) : Sprite(0, SpriteSize_8x8, SpriteColorFormat_Bmp, x, y) {
    dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*8*2); // Copy player data into sprite gfx
	
	// Initialize other members
	s_vx = 0;
	s_vy = 0;
	s_lifes = 3;
	s_jumping = false;
	s_hit = false;
}

Player::~Player() {
	oamFreeGfx(&oamMain, s_gfx);
}

void Player::move() {
	// Horizontal movement
	if(keysHeld() & KEY_LEFT) {
		s_vx = -1;
	}
	else if(keysHeld() & KEY_RIGHT) {
		s_vx = 1;
	}
    
	if((s_level->isKubeAt(s_x + s_vx, s_y)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y)) || (s_level->isKubeAt(s_x + s_vx, s_y + 7)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y + 7)) || (s_x + s_vx < 0) || (s_x + s_vx > s_level->length() * 8)) {
		s_vx = 0;
	}
	
	s_x += s_vx;
	
	s_vx = 0;
	
	// Jumping
	if((keysDown() & KEY_B) && (s_vy == 0) && (!s_jumping)) {
		s_vy = -9;
		s_jumping = true;
	}
	
	s_vy += GRAVITY;
	
	if(s_vy > VY_MAX) {
		s_vy = VY_MAX;
	}
	
	if((s_level->isKubeAt(s_x, s_y + 7 + s_vy)) || (s_level->isKubeAt(s_x + 7, s_y + 7 + s_vy))) {
		s_vy = 0;
		s_jumping = false;
	}
	
	s_y += s_vy;
	
	// Level scrolling
	
	/* TODO:
	 * 
	 * Here is only horizontal scrolling, and it may be optimized: The scrolling could be started before 124 and stopped after 124
	 * 
	 */
	
	if((s_x > 124) && (s_level->scrollX() < s_level->length() * 8 - 256)) {
		s_x = 124;
		s_level->scroll(1, 0);
	}
	
	if((s_x < 124) && (s_level->scrollX() > 0)) {
		s_x = 124;
		s_level->scroll(-1, 0);
	}
}

void Player::update() {
	// If player is hit
	if(s_hit) {
		if(!a) {
			frame = Game::frame;
			a = true;
		}
		
		dmaFillHalfWords(ARGB16(1,rand()%32,rand()%32,rand()%32), s_gfx, 8*8*2);
		
		if(Game::frame > frame + 120) {
			
			a = false;
			
			dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*8*2);
			
			s_hit = false;
		}
	}
	
	// Test game over
	if(s_lifes == 0) {
		s_lifes = 3;
		a = false;
		dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*8*2);
		s_hit = false;
		Game::gameOver();
	}
	
	// Test finish
	if(s_level->isFinishAt(s_x + 4, s_y + 4)) {
		a = false;
		dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*8*2);
		s_hit = false;
		Game::finish();
	}
}

void Player::removeLife() {
	if(!s_hit) {
		s_lifes--;
		s_hit = true;
	}
}
