#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "sprite.h"
#include "level.h"

#define GRAVITY 1
#define VY_MAX 8

class Level;

class Player : public Sprite {
    public:
        Player();
        ~Player();
        u16 lifesRemaining() const { return s_lifes; };
        void move();
		void removeLife();
		void setLevel(Level* level);
		void stop() { s_stop = true; };
		void start() { s_stop = false; };
		s32 vx() const { return s_vx; };
		s32 vy() const { return s_vy; };
		Level* level() const { return s_level; };
    
    private:
        u16 s_lifes;
		Level* s_level;
		s32 s_vx;
		s32 s_vy;
		bool s_jumping;
		bool s_stop;
		bool s_hit;
};

#endif // PLAYER_H