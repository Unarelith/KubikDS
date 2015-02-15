/*
 * =====================================================================================
 *
 *       Filename:  Player.hpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:34:19
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Sprite.hpp"

#define GRAVITY 1
#define VY_MAX 8

class Player : public Sprite {
    public:
        Player(s16 x, s16 y);
		
		void move();
		void update();
		
		void removeLife();
		
		int lifes() const { return m_lifes; }
		
		void setVY(s32 vy) { m_vy = vy; }
		
		void setJumpState(bool jumping) { m_jumping = jumping; }
		
		void setHitState(bool hit) { m_hit = hit; }
		
		static bool a;
		static int frame;
		
    private:
		s32 m_vx = 0;
		s32 m_vy = 0;
		
		bool m_jumping = false;
		bool m_hit = false;
		
		int m_lifes = 3;
};

#endif // PLAYER_HPP_
