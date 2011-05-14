#include "game.h"

int Game::frame = 0;
bool Game::isGameOver = false;
bool Game::paused = false;
bool Game::isFinished = false;
Level* Game::currentLevel = 0;

int i;
Game::Game() {
	// Initialize the background
	s_bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
	
	// Initialize the player
	s_player = new Player(0, 0);
	
	// Display titleScreen
	titleScreen();
}

Game::~Game() {
}

void Game::init(s16 px, s16 py) {
	// Set player position
	s_player->setPosition(px, py);
	
	// Initialize the level
	s_level = new Level(0, &map1, s_bg, 0, 0);
	
	s_enemies = s_level->map()->enemies;
	
	// Reset enemies
	for(i = 0 ; i < Enemy::nbEnemies ; i++) {
		s_enemies[i]->reset();
	}
	
	// Give current level data to the static variable
	Game::currentLevel = s_level;
	
	update();
}

void Game::titleScreen() {
	dmaCopy(titleScreenTiles, bgGetGfxPtr(s_bg), titleScreenTilesLen);
	dmaCopy(titleScreenPal, BG_PALETTE, titleScreenPalLen);
	dmaCopy(titleScreenMap, bgGetMapPtr(s_bg), titleScreenMapLen);
	
	int curPos = 1;
	
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
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		consoleClear();
		printf("\x1b[1;12HKubikDS");
			printf("\x1b[8;12HPlay");
			printf("\x1b[11;12HCredits");
		
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
	}
	
	consoleClear();
	
	if(curPos == 1) {
		// Enable sprite system
		oamEnable(&oamMain);
		
		// Initialize game system
		init(10, 176);
	}
	else if(curPos == 2) {
		consoleClear();
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
			printf("\x1b[7;2H- ludo6431,smealum,EvilTroopa");
			printf("\x1b[8;4Hfrom dev-fr");
			printf("\x1b[9;2H- Thomas Gosselin, a friend");
			printf("\x1b[10;2H- And those I've forgotten.");
		
		printf("\x1b[18;10H> Main menu");
		
		if(keysDown() & KEY_A) {
			break;
		}
		
		bgUpdate();
	}
	
	titleScreen();
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
	printf("\x1b[3;1HPosition: %i/%i", (s_player->x() + s_level->scrollX())/8, s_level->length());
	printf("\x1b[5;1HLevel: %i", s_level->id());
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
		
		for(i = 0 ; i < Enemy::nbEnemies ; i++) {
			delete s_enemies[i];
		}
		
		s_level = NULL;
		
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
		bgSetScroll(s_bg, s_level->scrollX() % 8, s_level->scrollY() % 8);
		
		bgUpdate(); // Update the background
		oamUpdate(&oamMain); // Update the sprite system
		
		if((isGameOver) || (isFinished)) {
			break;
		}
	}
	
	for(i = 0 ; i < Enemy::nbEnemies ; i++) {
		delete s_enemies[i];
	}
	
	s_level = NULL;
	
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
			printf("\x1b[8;12HTry again");
			printf("\x1b[11;12HMain menu");
		
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
	}
	
	consoleClear();
	isGameOver = false;
	
	if(curPos == 1) {
		// Enable sprite system
		oamEnable(&oamMain);
		
		// Initialize game system
		init(10, 176);
	}
	else if(curPos == 2) {
		// Return to title screen
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
			printf("\x1b[8;12HPlay again");
			printf("\x1b[11;12HMain menu");
		
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
	}
	
	consoleClear();
	isFinished = false;
	
	if(curPos == 1) {
		// Enable sprite system
		oamEnable(&oamMain);
		
		// Initialize game system
		init(10, 176);
	}
	else if(curPos == 2) {
		// Return to title screen
		titleScreen();
	}
}

void Game::gameOver() {
	isGameOver = true;
}

void Game::finish() {
	isFinished = true;
}
