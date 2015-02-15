/*
 * =====================================================================================
 *
 *       Filename:  Enemy.cpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:19:08
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include "Enemy.hpp"
#include "Game.hpp"

int Enemy::nbEnemies = 0;

Enemy::Enemy(int id, s16 x, s16 y, s32 vx, s32 vy) : Sprite(id, SpriteSize_8x8, SpriteColorFormat_Bmp, x, y) {
	/*u16 enemyData[8 * 8] = {
		0,1,0,1,1,0,1,0,
		1,1,1,1,1,1,1,1,
		0,1,0,1,1,0,1,0,
		1,1,1,1,1,1,1,1,
		1,1,1,0,0,1,1,1,
		0,1,0,1,1,0,1,0,
		1,1,1,1,1,1,1,1,
		0,1,0,1,1,0,1,0
	};
	
	u16 u;
	for(u = 0 ; u < 8 * 8 ; u++) {
		if(enemyData[u]) {
			m_gfx[u] = ARGB16(1,31,0,0);
		}
	}*/
	
	dmaFillHalfWords(ARGB16(1, 31, 0, 0), m_gfx, 8 * 8 * 2);
	
	m_ix = x;
	m_iy = y;
	
	m_ivx = vx;
	m_ivy = vy;
	
	m_ax = x;
	m_ay = y;
	
	m_vx = vx;
	m_vy = vy;
	
	nbEnemies++;
}

void Enemy::draw() {
	if(m_x + 7 > 0 && m_x < 256 && m_y + 7 > 0 && m_y < 192) {
		Sprite::draw();
	} else {
		oamClearSprite(&oamMain, m_id);
	}
}

void Enemy::move() {
	// Horizontal movement
	if(Game::currentLevel->isKubeAt(m_x + m_vx, m_y)
	|| Game::currentLevel->isKubeAt(m_x + 7 + m_vx, m_y)
	|| Game::currentLevel->isKubeAt(m_x + m_vx, m_y + 7)
	|| Game::currentLevel->isKubeAt(m_x + 7 + m_vx, m_y + 7)) {
		m_vx = -m_vx;
	}
	
	m_ax += m_vx;
	m_x = m_ax - Game::currentLevel->scrollX();
	
	// Vertical movement
	if(Game::currentLevel->isKubeAt(m_x, m_y + m_vy)
	|| Game::currentLevel->isKubeAt(m_x, m_y + 7 + m_vy)
	|| Game::currentLevel->isKubeAt(m_x + 7, m_y + m_vy)
	|| Game::currentLevel->isKubeAt(m_x + 7, m_y + 7 + m_vy)) {
		m_vy = -m_vy;
	}
	
	m_ay += m_vy;
	m_y = m_ay - Game::currentLevel->scrollY();
}

void Enemy::reset() {
	m_x = m_ix;
	m_y = m_iy;
	
	m_ax = m_x;
	m_ay = m_y;
	
	m_vx = m_ivx;
	m_vy = m_ivy;
}

