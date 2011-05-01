#include "player.h"

Player::Player() : Sprite(0, SpriteSize_8x8, SpriteColorFormat_Bmp, 10, 176) {
    dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*16*16);
	s_vx = 0;
	s_vy = 0;
	s_jumping = false;
}

Player::~Player() {
}

void Player::move() {
	// Move
    if(keysHeld() & KEY_LEFT) {
		s_vx = -1;
    }
    else if(keysHeld() & KEY_RIGHT) {
		s_vx = 1;
    }
    
	if((s_level->isKubeAt(s_x + s_vx, s_y)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y)) || (s_level->isKubeAt(s_x + s_vx, s_y + 7)) || (s_level->isKubeAt(s_x + 7 + s_vx, s_y + 7)))
		s_vx = 0;
    
    s_x += s_vx;
    
	s_vx = 0;
    
	// Jump
    if((keysDown() & KEY_A) && (s_vy == 0) && (!s_jumping)) {
		s_vy = -9;
		s_jumping = true;
	}
	
	s_vy += GRAVITY;
	
	if(s_vy > VY_MAX)
		s_vy = VY_MAX;
	
	/*bool coll = false;
	int i;
	for(i = s_y ; i < s_y + 7 + s_vy ; i += 4) {
		if((s_level->isKubeAt(s_x, i)) || (s_level->isKubeAt(s_x + 7, i))) {
			coll = true;
		}
	}*/  // EvilTroopa: Without VY_MAX
	
	if((s_level->isKubeAt(s_x, s_y + 7 + s_vy)) || (s_level->isKubeAt(s_x + 7, s_y + 7 + s_vy))) { // || (coll)) {
		s_vy = 0;
		s_jumping = false;
	}
	
	s_y += s_vy;
}
