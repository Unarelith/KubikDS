#include "game.h"
#include "levelEditor.h"

int Game::frame = 0;
bool Game::isGameOver = false;
bool Game::paused = false;
bool Game::isFinished = false;
Level* Game::currentLevel = 0;

touchPosition Game::touch;

Game::Game() {
	
	// Initialize the background
	s_bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
	
	// Initialize the player
	s_player = new Player(0, 0);
	
	// Initialize all levels
	initLevels();
	
	// Display titleScreen
	titleScreen();
}

Game::~Game() {
}

void Game::initLevels() {
	Level* level0 = new Level(0, &map0);
	Level* level1 = new Level(1, &map1);
	
	s_levels[0] = level0;
	s_levels[1] = level1;
}

void Game::init(s16 px, s16 py) {
	// Set player position
	s_player->setPosition(px, py);
	
	// Reset level scrolling
	currentLevel->resetScrolling();
	
	// Init currentLevel
	currentLevel->initializeBg(s_bg);
	
	// Init enemies table
	s_enemies = currentLevel->map()->enemies;
	
	// Reset enemies
	for(i = 0 ; i < Enemy::nbEnemies ; i++) {
		s_enemies[i]->reset();
	}
	
	update();
}

void Game::titleScreen() {
	dmaCopy(titleScreenTiles, bgGetGfxPtr(s_bg), titleScreenTilesLen);
	dmaCopy(titleScreenPal, BG_PALETTE, titleScreenPalLen);
	dmaCopy(titleScreenMap, bgGetMapPtr(s_bg), titleScreenMapLen);
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		printf("\x1b[1;12HKubikDS");
		iprintf("\x1b[11;10HPress start");
		
		if(keysDown() & KEY_START) {
			break;
		}
		
		bgUpdate();
	}
	
	titleScreen2();
}

void Game::titleScreen2() {
	int curPos = 1;
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		printf("\x1b[1;12HKubikDS");
			printf("\x1b[7;12HPlay");
			printf("\x1b[10;12HLevel Editor");
			printf("\x1b[13;12HCredits");
		
		printf("\x1b[%i;10H>", 4 + curPos*3);
		
		if(keysDown() & KEY_DOWN) {
			curPos++;
		}
		else if(keysDown() & KEY_UP) {
			curPos--;
		}
		
		if(curPos < 1) {
			curPos = 3;
		}
		else if(curPos > 3){
			curPos = 1;
		}
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
	}
	
	consoleClear();
	
	if(curPos == 1) {
		levelsMenu();
	}
	else if(curPos == 2) {
		oamClearSprite(&oamMain, s_player->i());
		for(i = 0 ; i < Enemy::nbEnemies ; i++) {
			oamClearSprite(&oamMain, s_enemies[i]->i());
		}
		LevelEditor* levelEditor = new LevelEditor(s_levels, s_bg);
		delete levelEditor;
		titleScreen();
	}
	else if(curPos == 3) {
		drawCredits();
	}
}

void Game::drawCredits() {
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		printf("\x1b[1;1HDeveloper, designer, etc:");
			printf("\x1b[3;2H- Quent42340");
		
		printf("\x1b[5;1HSpecial thanks:");
			printf("\x1b[7;2H- From dev-fr:");
				printf("\x1b[8;4H* ludo6431");
				printf("\x1b[9;4H* smealum");
				printf("\x1b[10;4H* EvilTroopa");
			
			printf("\x1b[12;2H- And those I've forgotten.");
		
		printf("\x1b[18;10H> Main menu");
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
	}
	
	titleScreen();
}

void Game::levelsMenu() {
	int curPos = 0;
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		
		printf("\x1b[1;8HChoose your level");
		
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
	currentLevel = s_levels[curPos];
	
	// Initialize game system
	init(currentLevel->map()->playerX, currentLevel->map()->playerY);
}

void Game::testCollisionsPE() {
	for(i = 0 ; i < Enemy::nbEnemies ; i++) {
		if((s_player->x() + 7 > s_enemies[i]->x()) && (s_player->x() < s_enemies[i]->x() + 7) && (s_player->y() < s_enemies[i]->y() + 7) && (s_player->y() + 7 > s_enemies[i]->y())) {
			s_player->removeLife();
		}
	}
}

void Game::displayHUD() {
	consoleClear();
	printf("\x1b[1;1HLifes: %i", s_player->lifesRemaining());
	//printf("\x1b[3;1HPosition: %i;%i / %i:%i", (s_player->x() + currentLevel->scrollX())/8, (s_player->y() + currentLevel->scrollY())/8, currentLevel->length(), currentLevel->height());
	// TODO: A mini map to show position
	printf("\x1b[3;1HLevel: %i", currentLevel->id());
}

void Game::pause() {
	bool paused = true;
	
	int curPos = 1;
	
	while(paused) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		
		printf("\x1b[1;14HPause");
			printf("\x1b[8;12HContinue");
			printf("\x1b[11;12HMain menu");
		
		printf("\x1b[23;7HPress A to continue");
		
		if(curPos == 1) {
			printf("\x1b[8;10H>");
		}
		else if (curPos == 2) {
			printf("\x1b[11;10H>");
		}
		
		if(keysDown() & KEY_DOWN) {
			curPos++;
		}
		else if(keysDown() & KEY_UP) {
			curPos--;
		}
		
		if(curPos < 1) {
			curPos = 2;
		}
		else if(curPos > 2){
			curPos = 1;
		}
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
		oamUpdate(&oamMain);
	}
	
	if(curPos == 1) {
		paused = false;
	}
	else if(curPos == 2) {
		Player::a = false;
		dmaFillHalfWords(ARGB16(1,31,31,31), s_player->gfx(), 8*8*2);
		s_player->setHitState(false);
		
		paused = false;
		
		currentLevel = NULL;
		
		oamDisable(&oamMain);
		
		titleScreen();
	}
}

void Game::update() {
	while(1) {
		// Increment frame variable
		frame++;
		
		// Wait for VBlank interrupt
		swiWaitForVBlank();
		
		// Scan keys state
		scanKeys();
		
		// Test collisions between Player and Enemies
		testCollisionsPE();
		
		// Display HUD ( Head Up Display )
		displayHUD();
		
		if(keysDown() & KEY_START) {
			pause(); // Display pause menu
		}
		
		// Player update functions
		s_player->move(); // Allow to move the player
		s_player->update(); // Update the player
		s_player->draw(); // Draw the player
		
		// Enemy update functions
		for(i = 0 ; i < Enemy::nbEnemies ; i++) {
			s_enemies[i]->move(); // Allow to move an enemy
			s_enemies[i]->draw(); // Draw an enemy
		}
		
		// Manage bg scrolling
		bgSetScroll(s_bg, currentLevel->scrollX() % 8, currentLevel->scrollY() % 8);
		
		bgUpdate(); // Update the background
		oamUpdate(&oamMain); // Update the sprite system
		
		if((isGameOver) || (isFinished)) {
			break;
		}
	}
	
	currentLevel = NULL;
	
	oamDisable(&oamMain);
	
	if(isGameOver) {
		gameOverScreen();
	}
	else if(isFinished) {
		finishedScreen();
	}
}

void Game::gameOverScreen() {
	bgSetScroll(s_bg, 0, 0);
	
	dmaCopy(gameOverTiles, bgGetGfxPtr(s_bg), gameOverTilesLen);
	dmaCopy(gameOverPal, BG_PALETTE, gameOverPalLen);
	dmaCopy(gameOverMap, bgGetMapPtr(s_bg), gameOverMapLen);
	
	int curPos = 1;
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		printf("\x1b[1;11HGame Over");
			//printf("\x1b[8;12HTry again");
			printf("\x1b[11;12HMain menu");
		
		if(curPos == 1) {
			printf("\x1b[11;10H>");
		}
		/*else if (curPos == 2) {
			printf("\x1b[11;10H>");
		}
		
		if(keysDown() & KEY_DOWN) {
			curPos++;
		}
		else if(keysDown() & KEY_UP) {
			curPos--;
		}
		
		if(curPos < 1) {
			curPos = 2;
		}
		else if(curPos > 2){
			curPos = 1;
		}*/
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
	}
	
	consoleClear();
	isGameOver = false;
	
	if(curPos == 1) {
	/*	// Enable sprite system
		oamEnable(&oamMain);
		
		// Initialize game system
		init(10, 176);
	}
	else if(curPos == 2) {
		// Return to title screen*/
		titleScreen();
	}
}

void Game::finishedScreen() {
	bgSetScroll(s_bg, 0, 0);
	
	dmaCopy(finishedTiles, bgGetGfxPtr(s_bg), finishedTilesLen);
	dmaCopy(finishedPal, BG_PALETTE, finishedPalLen);
	dmaCopy(finishedMap, bgGetMapPtr(s_bg), finishedMapLen);
	
	int curPos = 1;
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		printf("\x1b[1;12HYou win!");
			//printf("\x1b[8;12HPlay again");
			printf("\x1b[11;12HMain menu");
		
		if(curPos == 1) {
			printf("\x1b[11;10H>");
		}
		/*else if (curPos == 2) {
			printf("\x1b[11;10H>");
		}
		
		if(keysDown() & KEY_DOWN) {
			curPos++;
		}
		else if(keysDown() & KEY_UP) {
			curPos--;
		}
		
		if(curPos < 1) {
			curPos = 2;
		}
		else if(curPos > 2){
			curPos = 1;
		}*/
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
	}
	
	consoleClear();
	isFinished = false;
	
	if(curPos == 1) {
	/*	// Enable sprite system
		oamEnable(&oamMain);
		
		// Initialize game system
		init(10, 176);
	}
	else if(curPos == 2) {
		// Return to title screen*/
		titleScreen();
	}
}

void Game::gameOver() {
	isGameOver = true;
}

void Game::finish() {
	isFinished = true;
}
