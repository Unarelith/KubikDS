/*
 * =====================================================================================
 *
 *       Filename:  Game.hpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 15:51:58
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef GAME_HPP_
#define GAME_HPP_

#include "Enemy.hpp"
#include "Level.hpp"
#include "Player.hpp"

class Game {
	public:
		Game();
		
		void init(s16 px, s16 py);
		void initLevels();
		
		void update();
		
		void titleScreen();
		void titleScreen2();
		void drawCredits();
		void levelsMenu();
		void displayHUD();
		void pause();
		void gameOverScreen();
		void finishedScreen();
		
		void testCollisionsPE();
		
		int bg() const { return m_bg; }
		
		static void gameOver();
		static void finish();
		
		static int frame;
		
		static bool isGameOver;
		static bool paused;
		static bool isFinished;
		
		static Level* currentLevel;
		
		static touchPosition touch;
		
	private:
		int m_bg;
		
		Player m_player{0, 0};
		
		std::vector<Enemy> m_enemies;
		std::vector<Level> m_levels;
};

#endif // GAME_HPP_
