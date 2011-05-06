#include "game.h"

int i;

Game::Game() {
	// Initialize the background
	s_bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
	
	// Initialize the player
	s_player = new Player(10, 176);
	
	// Initialize the enemies
	Enemy* enemy = new Enemy(1, 216, 176, 1, 0);
	s_nbEnemies = 1;
	
	Enemy* enemies[] = {
		enemy
	};
	
	s_enemies = enemies;
	
	// Initialize the level
	s_level = new Level(0, &map1, s_bg);
	
	// Give current level data to player
	s_player->setLevel(s_level);
	
	// Give current level data to enemies
	for(i = 0 ; i < s_nbEnemies ; i++) {
		s_enemies[i]->setLevel(s_level);
	}
}

Game::~Game() {
}

void Game::update() {
	while(1) {
		// Wait for VBlank interrupt
		swiWaitForVBlank();
		
		// Scan keys state
		scanKeys();
		
		// Player update functions
		s_player->move(); // Allow to move the player
		s_player->draw(); // Draw the player
		
		// Enemy update functions
		for(i = 0 ; i < s_nbEnemies ; i++) {
			s_enemies[i]->move(); // Allow to move an enemy
			s_enemies[i]->draw(); // Draw an enemy
		}
		
		bgUpdate(); // Update the background
		oamUpdate(&oamMain); // Update the sprite system
	}
}
