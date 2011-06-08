#include "levelEditor.h"
#include "game.h"

unsigned short tilesMap[TILES_NB];

LevelEditor::LevelEditor(Level** levels, int bg) {
	s_bg = bg;
	
	s_levels = levels;
	s_level = menu();
	
	// Initialize the sub background
	s_bgSub = bgInitSub(0, BgType_Text8bpp, BgSize_T_512x512, 19, 3);
	
	// Reset top background
	dmaCopy(0, bgGetGfxPtr(s_bg), titleScreenTilesLen);
	dmaCopy(0, BG_PALETTE, titleScreenPalLen);
	dmaCopy(0, bgGetMapPtr(s_bg), titleScreenMapLen);
	
	// Set up top background
	dmaCopy(tilesTiles, bgGetGfxPtr(s_bg), tilesTilesLen);
	dmaCopy(tilesPal, BG_PALETTE, tilesPalLen);
	
	// Fill tiles map
	unsigned short s;
	for(s = 0 ; s < TILES_NB ; s++) {
		tilesMap[s] = s;
	}
	
	// Copy tiles map into bg map
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
	printf("\x1b[0;0HTile: %i", s_cursor->tile());
	printf("\x1b[1;0H%i %i %i", s_level->scrollX(), s_level->scrollY(), s_level->map()->height);
}

void LevelEditor::commands() {
	if(keysDown() & KEY_START) {
		pause(); // Display pause menu
	}
	
	s_cursor->move();
	
	if((keysDown() & KEY_X) && (s_level->scrollY() > 0)) { // Up
		s_level->scroll(0,-8);
	}
	if((keysDown() & KEY_B) && ((s_level->scrollY() / 8 + 24) < s_level->map()->height)) { // Down
		s_level->scroll(0,8);
	}
	if((keysDown() & KEY_Y) && (s_level->scrollX() > 0)) { // Left
		s_level->scroll(-8,0);
	}
	if((keysDown() & KEY_A) && ((s_level->scrollX() / 8 + 32) < s_level->map()->length)) { // Right
		s_level->scroll(8,0);
	}
}


void LevelEditor::pause() {
	bool paused = true;
	
	bgHide(s_bgSub);
	
	int curPos = 1;
	
	while(paused) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		
		printf("\x1b[1;14HPause");
			printf("\x1b[8;12HContinue");
			printf("\x1b[11;12HHelp");
			printf("\x1b[14;12HMain menu");
		
		printf("\x1b[23;7HPress A to continue");
		
		if(curPos == 1) {
			printf("\x1b[8;10H>");
		}
		else if(curPos == 2) {
			printf("\x1b[11;10H>");
		}
		else if(curPos == 3) {
			printf("\x1b[14;10H>");
		}
		
		if(keysDown() & KEY_DOWN) {
			curPos++;
		}
		else if(keysDown() & KEY_UP) {
			curPos--;
		}
		
		if(curPos < 1) {
			curPos = 3;
		}
		else if(curPos > 2){
			curPos = 3;
		}
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
		oamUpdate(&oamMain);
	}
	
	consoleClear();
	
	if(curPos == 1) {
		scanKeys();
		paused = false;
	}
	else if(curPos == 2) {
		scanKeys();
		paused = false;
		help();
	}
	else if(curPos == 3) {
		paused = false;
		s_quit = true;
	}
	
	bgShow(s_bgSub);
}

void LevelEditor::help() {
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		printf("\x1b[1;14HHelp");
			printf("\x1b[3;0H- Change current tile with the");
			printf("\x1b[4;2Hcursor on top screen.");
			printf("\x1b[6;0H- Scroll the map with ABXY keys.");
			printf("\x1b[9;0H- To change a tile, just tap on");
			printf("\x1b[10;2Hit.");
		
		printf("\x1b[20;6H> Back to pause menu");
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
	}
	
	pause();
}

void LevelEditor::update() {
	while(1) {
		// Wait for VBlank interrupt
		swiWaitForVBlank();
		
		// Scan keys state
		scanKeys();
		
		if(keysHeld() & KEY_TOUCH) {
			touchRead(&Game::touch);
		}
		
		// Commands ( keys interaction )
		commands();
		
		// Draw editor
		draw();
		
		// Manage bg scrolling
		bgSetScroll(s_bgSub, s_level->scrollX() % 8, s_level->scrollY() % 8);
		
		bgUpdate(); // Update the background
		oamUpdate(&oamMain); // Update the sprite system
		
		if(s_quit) {
			break;
		}
	}
	
	s_quit = false;
	
	bgHide(s_bgSub);
	
	delete s_cursor;
}
