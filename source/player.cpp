#include "player.h"
#include "game.h"

bool Player::a = false;
int Player::frame = 0;

u16 u;
u16 playerData[8 * 8] = {
	0,0,1,1,1,1,0,0,
	0,1,1,1,1,1,1,0,
	1,1,0,1,1,0,1,1,
	1,1,1,1,1,1,1,1,
	1,1,0,1,1,0,1,1,
	1,1,1,0,0,1,1,1,
	0,1,1,1,1,1,1,0,
	0,0,1,1,1,1,0,0
};

Player::Player(s16 x, s16 y) : Sprite(0, SpriteSize_8x8, SpriteColorFormat_Bmp, x, y) {
	for(u = 0 ; u < 8 * 8 ; u++) {
		if(playerData[u]) {
			s_gfx[u] = ARGB16(1,31,31,31);
		}
	}
    //dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*8*2); // Copy player data into sprite gfx
	
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
    
	if((Game::currentLevel->isKubeAt(s_x + s_vx, s_y)) || (Game::currentLevel->isKubeAt(s_x + 7 + s_vx, s_y)) || (Game::currentLevel->isKubeAt(s_x + s_vx, s_y + 7)) || (Game::currentLevel->isKubeAt(s_x + 7 + s_vx, s_y + 7)) || (s_x + s_vx < 0) || (s_x + s_vx > Game::currentLevel->length() * 8)) {
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
	
	if((Game::currentLevel->isKubeAt(s_x, s_y + 7 + s_vy)) || (Game::currentLevel->isKubeAt(s_x + 7, s_y + 7 + s_vy))) {
		s_vy = 0;
		s_jumping = false;
	}
	
	s_y += s_vy;
	
	// Level scrolling
	
	/* TODO:
	 * 
	 * It may be optimized: The scrolling could be started before 124 and stopped after 124
	 * 
	 */
	
	if((s_x > 124) && (Game::currentLevel->scrollX() < Game::currentLevel->length() * 8 - 256)) {
		s_x = 124;
		Game::currentLevel->scroll(1, 0);
	}
	
	if((s_x < 124) && (Game::currentLevel->scrollX() > 0)) {
		s_x = 124;
		Game::currentLevel->scroll(-1, 0);
	}
	
	if(!s_jumping) {
		if((s_y > 120) && (Game::currentLevel->scrollY() < Game::currentLevel->height() * 8 - 192)) {
			s_y -= s_vy;
			Game::currentLevel->scroll(0, s_vy);
		}
		
		if((s_y < 72) && (Game::currentLevel->scrollY() > 0)) {
			s_y++;
			Game::currentLevel->scroll(0, -1);
		}
	}
}

void Player::update() {
	// If player is hit
	if(s_hit) {
		if(!a) {
			frame = Game::frame;
			a = true;
		}
		
		for(u = 0 ; u < 8 * 8 ; u++) {
			if(playerData[u]) {
				s_gfx[u] = ARGB16(1,rand()%32,rand()%32,rand()%32);
			}
		}
		
		if(Game::frame > frame + 120) {
			
			a = false;
			
			for(u = 0 ; u < 8 * 8 ; u++) {
				if(playerData[u]) {
					s_gfx[u] = ARGB16(1,31,31,31);
				}
			}
			
			s_hit = false;
		}
	}
	
	// Test game over
	if(s_lifes == 0) {
		s_lifes = 3;
		a = false;
		
		for(u = 0 ; u < 8 * 8 ; u++) {
			if(playerData[u]) {
				s_gfx[u] = ARGB16(1,31,31,31);
			}
		}
		
		s_hit = false;
		Game::gameOver();
	}
	
	// Test finish
	if(Game::currentLevel->isFinishAt(s_x + 4, s_y + 4)) {
		a = false;
		
		for(u = 0 ; u < 8 * 8 ; u++) {
			if(playerData[u]) {
				s_gfx[u] = ARGB16(1,31,31,31);
			}
		}
		
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
