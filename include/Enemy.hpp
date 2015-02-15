/*
 * =====================================================================================
 *
 *       Filename:  Enemy.hpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:19:14
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include "Sprite.hpp"

class Enemy : public Sprite {
	public:
		Enemy(int id, s16 x, s16 y, s32 vx, s32 vy);
		
		void draw();
		void move();
		
		void reset();
		
		s32 vx() { return m_vx; }
		s32 vy() { return m_vy; }
		
		static int nbEnemies;
	
	private:
		s16 m_ix = 0;
		s16 m_iy = 0;
		
		s32 m_ivx = 0;
		s32 m_ivy = 0;
		
		s16 m_ax = 0;
		s16 m_ay = 0;
		
		s32 m_vx = 0;
		s32 m_vy = 0;
};

#endif // ENEMY_HPP_
