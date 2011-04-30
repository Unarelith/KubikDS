#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "sprite.h"
#include "level.h"

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
};

#endif // PLAYER_H