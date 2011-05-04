#ifndef LEVEL_H
#define LEVEL_H

#include "main.h"
#include "levelsData.h"
#include "enemy.h"
#include "player.h"
#include "game.h"

#include "tiles.h"

class Enemy;
class Player;
class Game;

class Level {
	public:
		Level(Map* map, int bg, int id);
		~Level();
        Map* map() const { return s_map; };
		bool isKubeAt(s16 x, s16 y);
		s16 x() const { return s_x; };
		s16 y() const { return s_y; };
		void scroll(s16 x, s16 y);
		u16 length() const { return s_length; };
		u16 height() const { return s_height; };
		void setEnemies(Enemy** enemies, int nbEnemies) { s_enemies = enemies; s_nbEnemies = nbEnemies; };
		void setPlayer(Player* player) { s_player = player; };
		Enemy** enemies() const { return s_enemies; };
		Player* player() const { return s_player; };
		void testCollisionsPE();
		void setGame(Game* game) { s_game = game; };
		Game* game() const { return s_game; };
		int id() const { return s_id; };
		int nbEnemies() const { return s_nbEnemies; };
	
	private:
		u16 s_length;
		u16 s_height;
		s16 s_x;
		s16 s_y;
		Map* s_map;
		u16 s_color;
		int s_bg;
		Enemy** s_enemies;
		int s_nbEnemies;
		Player* s_player;
		Game* s_game;
		int s_id;
};

#endif // LEVEL_H