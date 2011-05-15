#include "levelsData.h"

Map map0 = {0, 0, 0, 96, 24, ARGB16(1,0,31,31)};
Map map1 = {1, 0, 0, 32, 72, ARGB16(1,0,31,31)};

void fill_map(Map* map) {
	// Map
	map->map = (u16*)malloc(map->length * map->height * sizeof(u16));
	
	char filename[25];
	sprintf(filename, "efs:/level%i/map", map->id);
	
	FILE* file = fopen(filename, "r");
	int currentChar = 0;
	int n = 0;
	char c[2];
	
	if(file != NULL) {
		
		printf("\x1b[11;11HLoading...");
		
		while(currentChar != EOF) {
			currentChar = fgetc(file);
			sprintf(c, "%c", currentChar);
			if((currentChar != ',') && (currentChar != '\n')) {
				map->map[n] = (u16)atoi(c);
				n++;
			}
		}
		
		fclose(file);
	} else {
		printf("Error while reading map %i\nFilename: %s", map->id, filename);
		while(1) { swiWaitForVBlank(); }
	}
	
	// Enemies
	map->enemies = (Enemy**)malloc(119 * sizeof(Enemy*));
	
	sprintf(filename, "efs:/level%i/enemies", map->id);
	
	file = fopen(filename, "r");
	char line[MAX_SIZE] = "";
	int id = 1;
	s16 x,y = 0;
	s32 vx,vy = 1;
	
	n = 0;
	
	if(file != NULL) {
		
		printf("\x1b[11;11HLoading...");
		
		while(fgets(line, MAX_SIZE, file) != NULL) {
			sscanf(line, "%i;%hi;%hi;%i;%i", &id, &x, &y, &vx, &vy);
			map->enemies[n] = new Enemy(id, x, y, vx, vy);
			n++;
		}
		
		fclose(file);
	} else {
		printf("Error while reading enemies set %i\nFilename: %s", map->id, filename);
		while(1) { swiWaitForVBlank(); }
	}
}
