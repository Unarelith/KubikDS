#ifndef LEVELSDATA_H
#define LEVELSDATA_H

#include "main.h"

typedef struct {
	u8* map;
	u16 length;
	u16 height;
	u16 color;
} Map;

extern Map map1;

#endif // LEVELSDATA_H