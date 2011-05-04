#include "game.h"

Game::Game(int bg, Player* player, Enemy* enemy1, Level* level1) {
	s_bg = bg;
	
    /*s_player = new Player;
	s_enemy1 = new Enemy(1, 224, 176);
	s_level1 = new Level(&map1, s_bg);
	
	s_player->setLevel(s_level1);
	s_enemy1->setLevel(s_level1);
	
	int nbEnemies = 1;
	Enemy* enemies[] = {
		s_enemy1
	};
	
	s_level1->setEnemies(enemies, nbEnemies);
	s_level1->setGame(this);*/
	
	s_player = player;
	s_enemy1 = enemy1;
	s_level1 = level1;
}

Game::~Game() {
}

void Game::displayHUD() {
	iprintf("\x1b[1;1HLifes: %i", s_player->lifesRemaining());
	iprintf("\x1b[3;1HPosition: %i/%i", (s_player->x() + s_level1->x())/8, s_level1->length()); // TODO: Maybe a minimap will coming here..
	iprintf("\x1b[5;1HLevel: %i", s_level1->id());
}

void Game::update() {
	s_frame++;
	
	displayHUD();
	
    s_player->move();
    s_player->draw();
	
	s_enemy1->move();
	s_enemy1->draw();
}
