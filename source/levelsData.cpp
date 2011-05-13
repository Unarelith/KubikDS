#include "levelsData.h"

Map map1 = {0, 0, 96, 24, ARGB16(1,0,31,31)};

void fill_map(Map* map) {
	map->map = (u16*)malloc(map->length * map->height * sizeof(u16));
	
	char filename[25];
	sprintf(filename, "efs:/map%i/map", map->id);
	
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
}
