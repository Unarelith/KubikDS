#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "sprite.h"
#include "level.h"

#define GRAVITY 1
#define VY_MAX 8

class Player : public Sprite {
    public:
		// Construct & Destruct
        Player(s16 x, s16 y);
        ~Player();
		
		// Update functions
		void move();
		
		// Setting functions
		void setLevel(Level* level) { s_level = level; }
		
    private:
		// Movement
		s32 s_vx;
		s32 s_vy;
		bool s_jumping;
		
		int s_lifes; // Player's lifes
		bool s_hit; // Is player is or not
		
		Level* s_level; // Pointer to current level
};

#endif // PLAYER_H