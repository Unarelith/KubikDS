#ifndef LEVEL_H
#define LEVEL_H

#include "main.h"
#include "levelsData.h"

#include "tiles.h"

class Level {
	public:
		// Construct & Destruct
		Level(int id, Map* map, int bg);
		Level(int id, Map* map, int bg, s16 scrollX, s16 scrollY);
		~Level();
		
		// Update functions
		void scroll(s32 x, s32 y);
		
		// Get functions
		int id() const { return s_id; }
		u16 length() const { return s_map->length; }
		s32 scrollX() const { return s_scrollX; }
		s32 scrollY() const { return s_scrollY; }
		
		// Other functions
		bool isKubeAt(s16 x, s16 y);
		bool isFinishAt(s16 x, s16 y);
		
	private:
		// Functions
		void initializeBg(); // Needed by both constructors
		
		// Variables
		int s_id; // Id of the level
		
		Map* s_map; // Pointer to the map data
		
		int s_bg; // BG id returned by bgInit()
		
		// Scrolling position
		s16 s_scrollX;
		s16 s_scrollY;
};

#endif // LEVEL_H