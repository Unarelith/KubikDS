#ifndef LEVEL_H
#define LEVEL_H

#include "main.h"
#include "levelsData.h"

#include "tiles.h"

class Level {
	public:
		// Construct & Destruct
		Level(int id, Map* map);
		~Level();
		
		// Init functions
		void initializeBg(int bg, bool main = true);
		
		// Update functions
		void scroll(s32 x, s32 y, bool absolute = false);
		
		// Reset functions
		void resetScrolling();
		
		// Get functions
		int id() const { return s_id; }
		u16 length() const { return s_map->length; }
		u16 height() const { return s_map->height; }
		s32 scrollX() const { return s_scrollX; }
		s32 scrollY() const { return s_scrollY; }
		Map* map() const { return s_map; }
		
		// Setting functions
		void setKube(s16 x, s16 y, u16 tile);
		
		// Other functions
		bool isKubeAt(s16 x, s16 y);
		bool isFinishAt(s16 x, s16 y);
		
		static int nbLevels;
		
	private:
		int s_id; // Id of the level
		
		Map* s_map; // Pointer to the map data
		
		int s_bg; // BG id returned by bgInit()
		
		// Scrolling position
		s16 s_scrollX;
		s16 s_scrollY;
};

#endif // LEVEL_H