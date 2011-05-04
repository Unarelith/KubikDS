#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "level.h"

class Player;
class Enemy;
class Level;

class Game {
	public:
		Game(int bg, Player* player, Level* level1);
		~Game();
		
		void displayHUD();
		void update();
		
		int frame() const { return s_frame; };
		Player* player() const { return s_player; };
		Enemy** enemies() const { return s_enemies; };
		Level* level1() const { return s_level1; };
	
	private:
		int s_frame;
		int s_bg;
		Player* s_player;
		Enemy** s_enemies;
		Level* s_level1;
};

#endif // GAME_H