/*
 * =====================================================================================
 *
 *       Filename:  Game.cpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 15:55:48
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include "efs_lib.h"

#include "Game.hpp"

#include "finished.h"
#include "gameOver.h"
#include "titleScreen.h"

int Game::frame = 0;

bool Game::isGameOver = false;
bool Game::paused = false;
bool Game::isFinished = false;

Level* Game::currentLevel = nullptr;

touchPosition Game::touch;

Game::Game() {
	defaultExceptionHandler();
	
	srand(time(NULL));
	
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);
	
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG);
	
	PrintConsole bottomScreen;
	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 16, 0, false, true);
	
	printf("EFS loading...");
	
	if(!EFS_Init(EFS_AND_FAT | EFS_DEFAULT_DEVICE, nullptr)) {
		printf("FATAL ERROR: Bad filesystem init");
		while(1) { swiWaitForVBlank(); }
	}
	
	printf("EFS loaded!");
	
	oamInit(&oamMain, SpriteMapping_Bmp_1D_256, false);
	
	m_bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
	
	initLevels();
	
	titleScreen();
}

void Game::initLevels() {
	m_levels.emplace_back(0, Map{0, 96, 24, ARGB16(1, 0, 31, 31), 0, 0, 0, 0});
	m_levels.emplace_back(1, Map{1, 32, 72, ARGB16(1, 0, 31, 31), 0, 0, 0, 0});
}

void Game::init(s16 px, s16 py) {
	m_player.setPosition(px, py);
	
	currentLevel->resetScrolling();
	
	currentLevel->initializeBg(m_bg);
	
	m_enemies.swap(currentLevel->map().enemies);
	
	for(auto &it : m_enemies) {
		it.reset();
	}
	
	update();
}

void Game::titleScreen() {
	dmaCopy(titleScreenTiles, bgGetGfxPtr(m_bg), titleScreenTilesLen);
	dmaCopy(titleScreenPal, BG_PALETTE, titleScreenPalLen);
	dmaCopy(titleScreenMap, bgGetMapPtr(m_bg), titleScreenMapLen);
	
	while(1) {
		scanKeys();
		
		if(keysDown() & KEY_START) break;
		
		consoleClear();
		
		printf("\x1b[1;12HKubikDS");
		printf("\x1b[11;10HPress start");
		
		bgUpdate();
		
		swiWaitForVBlank();
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
			printf("\x1b[8;12HPlay");
			//printf("\x1b[10;12HLevel Editor");
			printf("\x1b[11;12HCredits");
		
		printf("\x1b[%i;10H>", 5 + curPos*3);
		
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
		levelsMenu();
	}
	/*else if(curPos == 2) {
		oamClearSprite(&oamMain, s_player->i());
		for(i = 0 ; i < Enemy::nbEnemies ; i++) {
			oamClearSprite(&oamMain, s_enemies[i]->i());
		}
		LevelEditor* levelEditor = new LevelEditor(s_levels, s_bg);
		delete levelEditor;
		titleScreen();
	}*/
	else if(curPos == 2) {
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
		scanKeys();
		
		consoleClear();
		
		printf("\x1b[1;8HChoose your level");
		
		for(int i = 0 ; i < Level::nbLevels ; i++) {
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
		
		swiWaitForVBlank();
	}
	
	consoleClear();
	
	oamEnable(&oamMain);
	
	currentLevel = &m_levels[curPos];
	
	init(currentLevel->map().playerX, currentLevel->map().playerY);
}

void Game::testCollisionsPE() {
	for(auto &it : m_enemies) {
		if(m_player.x() > it.x() - 7 && m_player.x() < it.x() + 7
		&& m_player.y() < it.y() + 7 && m_player.y() > it.y() - 7) {
			m_player.removeLife();
		}
	}
}

void Game::displayHUD() {
	consoleClear();
	
	printf("\x1b[1;1HLifes: %i", m_player.lifes());
	//printf("\x1b[3;1HPosition: %i;%i / %i:%i", (s_player->x() + currentLevel->scrollX())/8, (s_player->y() + currentLevel->scrollY())/8, currentLevel->length(), currentLevel->height());
	// TODO: A mini map to show position
	printf("\x1b[3;1HLevel: %i", currentLevel->id());
}

void Game::pause() {
	bool paused = true;
	
	int curPos = 1;
	
	while(paused) {
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
		
		swiWaitForVBlank();
	}
	
	if(curPos == 1) {
		paused = false;
	}
	else if(curPos == 2) {
		Player::a = false;
		
		dmaFillHalfWords(ARGB16(1, 31, 31, 31), m_player.gfx(), 8 * 8 * 2);
		m_player.setHitState(false);
		
		paused = false;
		
		currentLevel = NULL;
		
		oamDisable(&oamMain);
		
		titleScreen();
	}
}

void Game::update() {
	while(1) {
		frame++;
		
		swiWaitForVBlank();
		
		scanKeys();
		
		testCollisionsPE();
		
		displayHUD();
		
		if(keysDown() & KEY_START) {
			pause();
		}
		
		m_player.move();
		m_player.update();
		m_player.draw();
		
		for(auto &it : m_enemies) {
			it.move();
			it.draw();
		}
		
		bgSetScroll(m_bg, currentLevel->scrollX() % 8, currentLevel->scrollY() % 8);
		
		bgUpdate();
		oamUpdate(&oamMain);
		
		if((isGameOver) || (isFinished)) {
			break;
		}
	}
	
	currentLevel = nullptr;
	
	oamDisable(&oamMain);
	
	if(isGameOver) {
		gameOverScreen();
	}
	else if(isFinished) {
		finishedScreen();
	}
}

void Game::gameOverScreen() {
	bgSetScroll(m_bg, 0, 0);
	
	dmaCopy(gameOverTiles, bgGetGfxPtr(m_bg), gameOverTilesLen);
	dmaCopy(gameOverPal, BG_PALETTE, gameOverPalLen);
	dmaCopy(gameOverMap, bgGetMapPtr(m_bg), gameOverMapLen);
	
	int curPos = 1;
	
	while(1) {
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
		
		swiWaitForVBlank();
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
	bgSetScroll(m_bg, 0, 0);
	
	dmaCopy(finishedTiles, bgGetGfxPtr(m_bg), finishedTilesLen);
	dmaCopy(finishedPal, BG_PALETTE, finishedPalLen);
	dmaCopy(finishedMap, bgGetMapPtr(m_bg), finishedMapLen);
	
	int curPos = 1;
	
	while(1) {
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
		
		swiWaitForVBlank();
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

