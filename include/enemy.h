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
		void stop() { s_stop = true; };
		void start() { s_stop = false; };
		s32 vx() const { return s_vx; };
		s32 vy() const { return s_vy; };
	
	private:
		Level* s_level;
		s32 s_vx;
		s32 s_vy;
		s16 s_lx;
		s16 s_ly;
		bool s_stop;
};

#endif // ENEMY_H