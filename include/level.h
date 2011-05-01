#ifndef LEVEL_H
#define LEVEL_H

#include "main.h"
#include "levelsData.h"

#include "tiles.h"

class Level {
	public:
		Level(Map* map, int bg);
		~Level();
        Map* map() const { return s_map; };
		bool isKubeAt(s16 x, s16 y);
		s16 x() const { return s_x; };
		s16 y() const { return s_y; };
		void scroll(s16 x, s16 y);
		u16 length() const { return s_length; };
		u16 height() const { return s_height; };
	
	private:
		u16 s_length;
		u16 s_height;
		s16 s_x;
		s16 s_y;
		Map* s_map;
		u16 s_color;
		int s_bg;
};

#endif // LEVEL_H