#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "player.h"
#include "enemy.h"
#include "level.h"

class Game {
	public:
		// Construct & Destruct
		Game();
		~Game();
		
		// Update functions
		void update();
		
		// Other functions
		void testCollisionsPE(); // Test collisions between player and enemies
		void displayHUD(); // Display HUD ( Head Up Display )
		
		// Get functions
		int bg() const { return s_bg; }
		
		static int frame; // Current frame
	
	private:
		int s_bg; // BG id returned by bgInit()
		Player* s_player; // Pointer to player
		Enemy** s_enemies; // Enemies table
		Level* s_level; // Pointer to current level
};

#endif // GAME_H