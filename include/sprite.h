#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"

class Sprite {
    public:
        Sprite(int i, SpriteSize size, SpriteColorFormat format, s16 x, s16 y, u8 speed);
		~Sprite();
        u16* gfx() const { return s_gfx; };
        s16 x() const { return s_x; };
        s16 y() const { return s_y; };
        void draw();
    
    protected:
        u16* s_gfx;
        SpriteSize s_size;
        SpriteColorFormat s_format;
        s16 s_x;
        s16 s_y;
        u8 s_speed;
        int s_i;
};

#endif // SPRITE_H
