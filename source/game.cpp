#include "game.h"

int Game::frame = 0;

int i;
Game::Game() {
	// Initialize the background
	s_bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
	
	// Initialize the player
	s_player = new Player(10, 176);
	
	// Initialize the enemies
	Enemy* enemy1 = new Enemy(1, 216, 176, 1, 0);
	Enemy* enemy2 = new Enemy(2, 128, 64, 0, 1);
	
	Enemy* enemies[60] = {
		enemy1,
		enemy2
	};
	
	s_enemies = enemies;
	
	// Initialize the level
	s_level = new Level(0, &map1, s_bg);
	
	// Give current level data to player
	s_player->setLevel(s_level);
	
	// Give current level data to enemies
	for(i = 0 ; i < Enemy::nbEnemies ; i++) {
		s_enemies[i]->setLevel(s_level);
	}
}

Game::~Game() {
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
		
		// Player update functions
		s_player->move(); // Allow to move the player
		s_player->draw(); // Draw the player
		
		// Enemy update functions
		for(i = 0 ; i < Enemy::nbEnemies ; i++) {
			s_enemies[i]->move(); // Allow to move an enemy
			s_enemies[i]->draw(); // Draw an enemy
		}
		
		bgUpdate(); // Update the background
		oamUpdate(&oamMain); // Update the sprite system
	}
}
