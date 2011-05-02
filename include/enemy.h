#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"
#include "sprite.h"
#include "level.h"

class Level;

class Enemy : public Sprite {
	public:
		Enemy(int i, s16 x, s16 y);
		~Enemy();
		void move();
		void draw();
		void setLevel(Level* level) { s_level = level; };
		s32 vx() const { return s_vx; };
	
	private:
		Level* s_level;
		s32 s_vx;
		s16 s_lx;
		s16 s_ly;
};

#endif // ENEMY_H