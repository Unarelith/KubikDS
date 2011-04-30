#ifndef LEVEL_H
#define LEVEL_H

#include "main.h"

class Level {
	public:
		Level(u16 length, u8 map[], u16 color, u16* bgGfx);
		~Level();
		void draw();
		void drawKube(s16 x, s16 y, u16 color);
        u8* map() const { return s_map; };
		bool isKubeAt(u8 pos, s16 x, s16 y); // pos = 0:Left, 1:Right, 2:Up, 3:Down
	
	private:
		u16 s_length;
		u16 s_height;
		s16 s_x;
		s16 s_y;
		u8* s_map;
		u16 s_color;
		u16* s_bgGfx;
};

#endif // LEVEL_H