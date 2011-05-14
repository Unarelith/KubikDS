#ifndef LEVELSDATA_H
#define LEVELSDATA_H

#include "main.h"
#include "enemy.h"

#include "malloc.h"

#define MAX_SIZE 100

typedef struct {
	int id;
	u16* map;
	Enemy** enemies;
	u16 length;
	u16 height;
	u16 color;
} Map;

extern Map map1;

void fill_map(Map* map);

#endif // LEVELSDATA_H