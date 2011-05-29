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
		
		// Update functions
		void move();
};

#endif // CURSOR_H