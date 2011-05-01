#include "player.h"

Player::Player() : Sprite(0, SpriteSize_8x8, SpriteColorFormat_Bmp, 5, 176, 1) {
    dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*16*16);
	s_jumpstate = "ground";
	s_jumpspeed = 10;
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
    
    if((keysHeld() & KEY_A) && (s_jumpstate == "ground"))
		s_jumpstate = "jumping";
	
	if(s_jumpstate == "jumping") {
		s_jumpspeed -= 1;
		s_gravity -= s_jumpspeed;
	}
	
	if(s_gravity < 0)
		s_jumpstate = "falling";
	
	if((s_gravity < 176) && (s_jumpstate == "falling"))
		s_gravity += s_jumpspeed + 5;
	
	if(s_gravity == 176) {
		s_jumpspeed = 10;
		s_jumpstate = "ground";
	}
	
	s_y = s_gravity;
	
	iprintf("%i, %i, %i, %s\n", s_jumpspeed, s_gravity, s_y, s_jumpstate.c_str());
}
