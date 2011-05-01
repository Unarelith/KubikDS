#ifndef LEVEL_H
#define LEVEL_H

#include "main.h"
#include "levelsData.h"

class Level {
	public:
		Level(Map* map, u16* bgGfx);
		~Level();
		void draw();
		void drawKube(s16 x, s16 y, u16 color);
        Map* map() const { return s_map; };
		bool isKubeAt(s16 x, s16 y);
		s16 x() const { return s_x; };
		s16 y() const { return s_y; };
		void scroll(s16 x, s16 y) { s_x += x; s_y += y; };
		u16 length() const { return s_length; };
		u16 height() const { return s_height; };
	
	private:
		u16 s_length;
		u16 s_height;
		s16 s_x;
		s16 s_y;
		Map* s_map;
		u16 s_color;
		u16* s_bgGfx;
};

#endif // LEVEL_H