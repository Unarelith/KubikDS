#ifndef CURSOR_H
#define CURSOR_H

#include "main.h"
#include "sprite.h"

#define TILES_NB 3

class Cursor : public Sprite {
	public:
		// Construct & Destruct
		Cursor();
		~Cursor();
		u16 tile() const { return (((s_x / 8) + 1) * ((s_y / 8) + 1)) - 1; }
		
		// Update functions
		void move();
};

#endif // CURSOR_H