#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"
#include "sprite.h"
#include "level.h"

class Enemy : public Sprite {
	public:
		// Construct & Destruct
		Enemy(int i, s16 x, s16 y, s32 vx, s32 vy);
		~Enemy();
		
		// Update functions
		void draw();
		void move();
		
		// Setting functions
		void setLevel(Level* level) { s_level = level; }
		
	private:
		// Absolute position
		s16 s_ax;
		s16 s_ay;
		
		// Movement
		s32 s_vx;
		s32 s_vy;
		
		Level* s_level; // Pointer to the current level
};

#endif // ENEMY_H