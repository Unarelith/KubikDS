#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"
#include "sprite.h"

class Enemy : public Sprite {
	public:
		// Construct & Destruct
		Enemy(int i, s16 x, s16 y, s32 vx, s32 vy);
		~Enemy();
		
		// Update functions
		void draw();
		void move();
		
		// Get functions
		s32 vx() { return s_vx; }
		s32 vy() { return s_vy; }
		
		// Other functions
		void reset();
		
		static int nbEnemies;
	
	private:
		// Initial position
		s16 s_ix;
		s16 s_iy;
		s32 s_ivx;
		s32 s_ivy;
		
		// Absolute position
		s16 s_ax;
		s16 s_ay;
		
		// Movement
		s32 s_vx;
		s32 s_vy;
};

#endif // ENEMY_H