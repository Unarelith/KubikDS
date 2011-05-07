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
		void update();
		
		// Get functions
		int lifesRemaining() const { return s_lifes; }
		
		// Setting functions
		void setLevel(Level* level) { s_level = level; }
		void set_vy(s32 vy) { s_vy = vy; }
		void setJumpState(bool jumping) { s_jumping = jumping; }
		void setHitState(bool hit) { s_hit = hit; }
		
		// Other functions
		void removeLife();
		
		// Static variables
		static bool a;
		static int frame;
		
    private:
		// Movement
		s32 s_vx;
		s32 s_vy;
		bool s_jumping;
		
		int s_lifes; // Player's lifes
		bool s_hit; // Is player hit or not
		
		Level* s_level; // Pointer to current level
};

#endif // PLAYER_H