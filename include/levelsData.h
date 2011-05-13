#ifndef LEVELSDATA_H
#define LEVELSDATA_H

#include "main.h"

#include "malloc.h"

typedef struct {
	int id;
	u16* map;
	u16 length;
	u16 height;
	u16 color;
} Map;

extern Map map1;

void fill_map(Map* map);

#endif // LEVELSDATA_H