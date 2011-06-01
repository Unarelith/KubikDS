#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "main.h"
#include "level.h"
#include "cursor.h"

#define tilesMapLen 8

#include "titleScreen.h"

class LevelEditor {
	public:
		// Construct & Destruct
		LevelEditor(Level** levels, int bg);
		~LevelEditor();
		
		// Update functions
		void draw();
		void update();
		
		// Interface management
		Level* menu();
		void pause();
		void commands();
	
	private:
		Level* s_level;
		Level** s_levels;
		int s_bg;
		int s_bgSub;
		Cursor* s_cursor;
};

#endif // LEVELEDITOR_H
