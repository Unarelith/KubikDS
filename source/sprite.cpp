#include "sprite.h" 

Sprite::Sprite(int i, SpriteSize size, SpriteColorFormat format, s16 x, s16 y) {
    s_size = size;
    s_format = format;
    s_x = x;
    s_y = y;
    s_gfx = oamAllocateGfx(&oamMain, s_size, s_format);
    s_i = i;
}

Sprite::~Sprite() {
}

void Sprite::draw() {
    oamSet(&oamMain, s_i, s_x, s_y, 0,
           15, s_size, s_format, s_gfx, -1,
           false, false, false, false, false);
}
