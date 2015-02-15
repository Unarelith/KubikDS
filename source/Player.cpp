/*
 * =====================================================================================
 *
 *       Filename:  Player.cpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:34:28
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include "Game.hpp"
#include "Player.hpp"

bool Player::a = false;
int Player::frame = 0;

u16 u;
/*u16 playerData[8 * 8] = {
	0,0,1,1,1,1,0,0,
	0,1,1,1,1,1,1,0,
	1,1,0,1,1,0,1,1,
	1,1,1,1,1,1,1,1,
	1,1,0,1,1,0,1,1,
	1,1,1,0,0,1,1,1,
	0,1,1,1,1,1,1,0,
	0,0,1,1,1,1,0,0
};*/

Player::Player(s16 x, s16 y) : Sprite(0, SpriteSize_8x8, SpriteColorFormat_Bmp, x, y) {
	/*for(u = 0 ; u < 8 * 8 ; u++) {
		if(playerData[u]) {
			m_gfx[u] = ARGB16(1,31,31,31);
		}
	}*/
	
    dmaFillHalfWords(ARGB16(1, 31, 31, 31), m_gfx, 8 * 8 * 2);
}

void Player::move() {
	// Horizontal movement
	if(keysHeld() & KEY_LEFT) {
		m_vx = -1;
	}
	else if(keysHeld() & KEY_RIGHT) {
		m_vx = 1;
	}
    
	if(Game::currentLevel->isKubeAt(m_x + m_vx, m_y)
	|| Game::currentLevel->isKubeAt(m_x + 7 + m_vx, m_y)
	|| Game::currentLevel->isKubeAt(m_x + m_vx, m_y + 7)
	|| Game::currentLevel->isKubeAt(m_x + 7 + m_vx, m_y + 7)
	|| (m_x + m_vx < 0) || (m_x + m_vx > Game::currentLevel->length() * 8)) {
		m_vx = 0;
	}
	
	m_x += m_vx;
	
	m_vx = 0;
	
	// Jumping
	if((keysDown() & KEY_B) && (m_vy == 0) && (!m_jumping)) {
		m_vy = -9;
		m_jumping = true;
	}
	
	m_vy += GRAVITY;
	
	if(m_vy > VY_MAX) {
		m_vy = VY_MAX;
	}
	
	if((Game::currentLevel->isKubeAt(m_x, m_y + 7 + m_vy)) || (Game::currentLevel->isKubeAt(m_x + 7, m_y + 7 + m_vy))) {
		m_vy = 0;
		m_jumping = false;
	}
	
	m_y += m_vy;
	
	// Level scrolling
	
	/* TODO:
	 * 
	 * It may be optimized: The scrolling could be started before 124 and stopped after 124
	 * 
	 */
	
	if(m_x > 124 && Game::currentLevel->scrollX() < Game::currentLevel->length() * 8 - 256) {
		m_x = 124;
		Game::currentLevel->scroll(1, 0);
	}
	
	if(m_x < 124 && Game::currentLevel->scrollX() > 0) {
		m_x = 124;
		Game::currentLevel->scroll(-1, 0);
	}
	
	if(!m_jumping) {
		if(m_y > 120 && Game::currentLevel->scrollY() < Game::currentLevel->height() * 8 - 192) {
			m_y -= m_vy;
			Game::currentLevel->scroll(0, m_vy);
		}
		
		if(m_y < 72 && Game::currentLevel->scrollY() > 0) {
			m_y++;
			Game::currentLevel->scroll(0, -1);
		}
	}
}

void Player::update() {
	// If player is hit
	if(m_hit) {
		if(!a) {
			frame = Game::frame;
			a = true;
		}
		
		u16 randomColor = ARGB16(1, rand() % 32, rand() % 32, rand() % 32);
		dmaFillHalfWords(randomColor, m_gfx, 8 * 8 * 2);
				
		if(Game::frame > frame + 120) {
			
			a = false;
			
			dmaFillHalfWords(ARGB16(1, 31, 31, 31), m_gfx, 8 * 8 * 2);
			
			m_hit = false;
		}
	}
	
	// Test game over
	if(m_lifes == 0) {
		m_lifes = 3;
		a = false;
		
		dmaFillHalfWords(ARGB16(1, 31, 31, 31), m_gfx, 8 * 8 * 2);
		
		m_hit = false;
		Game::gameOver();
	}
	
	// Test finish
	if(Game::currentLevel->isFinishAt(m_x + 4, m_y + 4)) {
		a = false;
		
		dmaFillHalfWords(ARGB16(1, 31, 31, 31), m_gfx, 8 * 8 * 2);
		
		m_hit = false;
		
		Game::finish();
	}
}

void Player::removeLife() {
	if(!m_hit) {
		m_lifes--;
		m_hit = true;
	}
}
