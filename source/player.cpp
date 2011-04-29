#include "player.h"

Player::Player() : Sprite(0, SpriteSize_8x8, SpriteColorFormat_Bmp, 0, 0, 2) {
    dmaFillHalfWords(ARGB16(1,31,31,31), s_gfx, 8*8*2);
}

Player::~Player() {
}

void Player::move() {
    if(keysHeld() & KEY_LEFT) {
        s_x -= s_speed;
    }
    else if(keysHeld() & KEY_RIGHT) {
        s_x += s_speed;
    }
}
