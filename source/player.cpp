#include "player.h"

Player::Player() : Sprite(0, SpriteSize_8x8, SpriteColorFormat_Bmp, 220, 176, 1) {
    dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*16*16);
}

Player::~Player() {
}

void Player::move() {
    if(keysHeld() & KEY_B) {
		s_speed = 2;
	} else {
		s_speed = 1;
	}
	
    if(keysHeld() & KEY_LEFT) {
		if(!s_level->isKubeAt(0, s_x, s_y)) {
			s_x -= s_speed;
		}
    }
    else if(keysHeld() & KEY_RIGHT) {
		if(!s_level->isKubeAt(1, s_x, s_y)) {
			s_x += s_speed;
		}
    }
    else if(keysHeld() & KEY_UP) {
		if(!s_level->isKubeAt(2, s_x, s_y)) {
			s_y -= s_speed;
		}
    }
    else if(keysHeld() & KEY_DOWN) {
		if(!s_level->isKubeAt(3, s_x, s_y)) {
			s_y += s_speed;
		}
    }
}
