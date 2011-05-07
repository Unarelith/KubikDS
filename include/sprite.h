#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"

class Sprite {
    public:
		// Construct & Destruct
        Sprite(int i, SpriteSize size, SpriteColorFormat format, s16 x, s16 y);
		~Sprite();
		
		// Update functions
        void draw();
		
		// Get functions
        u16* gfx() const { return s_gfx; } // Get sprite gfx pointer
        s16 x() const { return s_x; } // Get sprite x coordinate
        s16 y() const { return s_y; } // Get sprite y coordinate
        
        // Setting functions
        void setPosition(s16 x, s16 y) { s_x = x; s_y = y; }
    
    protected:
        int s_i; // Sprite index
        u16* s_gfx; // Pointer to sprite gfx
		
        SpriteSize s_size; // Sprite size
        SpriteColorFormat s_format; // Sprite format
		
		// Sprite coordinates
        s16 s_x;
        s16 s_y;
};

#endif // SPRITE_H
