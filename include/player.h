#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "sprite.h"

class Player : public Sprite {
    public:
        Player();
        ~Player();
        u16 lifesRemaining() const { return s_lifes; };
        void move();
    
    private:
        u16 s_lifes;
        u16 s_player_data[360];
};

#endif // PLAYER_H