#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "level.h"

class Player;
class Enemy;
class Level;

class Game {
	public:
		Game(int bg, Player* player, Enemy* enemy1, Level* level1);
		~Game();
		
		void displayHUD();
		void update();
		
		int frame() const { return s_frame; };
		Player* player() const { return s_player; };
		Enemy* enemy1() const { return s_enemy1; };
		Level* level1() const { return s_level1; };
	
	private:
		int s_frame;
		int s_bg;
		Player* s_player;
		Enemy* s_enemy1;
		Level* s_level1;
};

#endif // GAME_H