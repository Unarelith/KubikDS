#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "sprite.h"
#include "level.h"

#define GRAVITY 1
#define VY_MAX 8

class Player : public Sprite {
    public:
        Player();
        ~Player();
        u16 lifesRemaining() const { return s_lifes; };
        void move();
		void setLevel(Level* level) { s_level = level; };
    
    private:
        u16 s_lifes;
		Level* s_level;
		s32 s_vx;
		s32 s_vy;
		bool s_jumping;
};

#endif // PLAYER_H