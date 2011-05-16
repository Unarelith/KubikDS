#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "player.h"
#include "enemy.h"
#include "level.h"

#include "titleScreen.h"
#include "gameOver.h"
#include "finished.h"

class Game {
	public:
		// Construct & Destruct
		Game();
		~Game();
		
		// Init functions
		void init(s16 x, s16 y);
		void initLevels();
		
		// Update functions
		void update();
		
		// Get functions
		int bg() const { return s_bg; }
		
		// Game interface functions
		void titleScreen(); // Draw title screen
		void drawCredits(); // Draw credits
		void levelsMenu(); // Display levels menu
		void displayHUD(); // Display HUD ( Head Up Display )
		void pause(); // Display pause menu and stop player
		void gameOverScreen(); // Draw game over screen
		void finishedScreen(); // Draw finished screen
		
		// Other functions
		void testCollisionsPE(); // Test collisions between player and enemies
		
		// Static functions
		static void gameOver(); // When the game is finished
		static void finish(); // When the player win
		
		// Static variables
		static int frame; // Current frame
		static bool isGameOver; // Is the game over
		static bool paused; // Is the game paused
		static bool isFinished; // Is the game finished
		static Level* currentLevel; // Pointer to the current level
	
	private:
		int s_bg; // BG id returned by bgInit()
		Player* s_player; // Pointer to player
		Enemy** s_enemies; // Enemies table
		Level** s_levels; // Levels table
};

#endif // GAME_H