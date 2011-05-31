#include "levelEditor.h"

LevelEditor::LevelEditor(Level** levels, int bg) {
	s_bg = bg;
	
	s_levels = levels;
	s_level = menu();
	
	// Initialize the sub background
	s_bgSub = bgInitSub(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
	
	// Reset top background
	dmaCopy(0, bgGetGfxPtr(s_bg), titleScreenTilesLen);
	dmaCopy(0, BG_PALETTE, titleScreenPalLen);
	dmaCopy(0, bgGetMapPtr(s_bg), titleScreenMapLen);
	
	// Set up top background
	dmaCopy(tilesTiles, bgGetGfxPtr(s_bg), tilesTilesLen);
	dmaCopy(tilesPal, BG_PALETTE, tilesPalLen);
	
	unsigned short s;
	for(s = 0 ; s < TILES_NB ; s++) {
		tilesMap[s] = s;
	}
	
	dmaCopyHalfWords(3, tilesMap, bgGetMapPtr(s_bg), tilesMapLen);
	
	// Reset level scrolling
	s_level->resetScrolling();
	
	// Init currentLevel
	s_level->initializeBg(s_bgSub, false);
	
	// Init cursor
	s_cursor = new Cursor;
	
	update();
}

LevelEditor::~LevelEditor() {
}

Level* LevelEditor::menu() {
	dmaCopy(titleScreenTiles, bgGetGfxPtr(s_bg), titleScreenTilesLen);
	dmaCopy(titleScreenPal, BG_PALETTE, titleScreenPalLen);
	dmaCopy(titleScreenMap, bgGetMapPtr(s_bg), titleScreenMapLen);
	
	Level* level;
	
	int curPos = 0;
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		
		printf("\x1b[1;6HChoose a level to edit");
		
		for(i = 0 ; i < Level::nbLevels ; i++) {
			printf("\x1b[%i;%iHLevel %i", 3 + 2 * i, 4, i);
		}
		
		printf("\x1b[%i;2H>", 3 + 2 * curPos);
		
		if(keysDown() & KEY_DOWN) {
			curPos++;
		}
		else if(keysDown() & KEY_UP) {
			curPos--;
		}
		
		if(curPos < 0) {
			curPos = Level::nbLevels - 1;
		}
		else if(curPos > Level::nbLevels - 1){
			curPos = 0;
		}
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
	}
	
	consoleClear();
	
	// Enable sprite system
	oamEnable(&oamMain);
	
	// Set the selectionned level as current level
	level = s_levels[curPos];
	
	return level;
}

void LevelEditor::draw() {
	s_cursor->draw();
}

void LevelEditor::commands() {
	if(keysDown() & KEY_START) {
		//pause(); // Display pause menu
	}
	
	s_cursor->move();
}

void LevelEditor::update() {
	while(1) {
		// Wait for VBlank interrupt
		swiWaitForVBlank();
		
		// Scan keys state
		scanKeys();
		
		// Commands ( keys interaction )
		commands();
		
		// Draw editor
		draw();
		
		// Manage bg scrolling
		bgSetScroll(s_bgSub, s_level->scrollX() % 8, s_level->scrollY() % 8);
		
		bgUpdate(); // Update the background
		oamUpdate(&oamMain); // Update the sprite system
	}
}
