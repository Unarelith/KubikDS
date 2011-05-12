#include "levelsData.h"

u16 map[96 * 24];

Map map1 = {0, map, 96, 24, ARGB16(1,0,31,31)};

using namespace std;

void fill_map(Map* map) {
	char filename[25];
	sprintf(filename, "efs:/maps/map%i.map", map->id);
	
	FILE* file = fopen(filename, "r");
	int currentChar = 0;
	int n = 0;
	char c[1];
	
	if(file != NULL) {
		
		printf("\x1b[11;11HLoading...");
		
		do {
			currentChar = fgetc(file);
			sprintf(c, "%c", currentChar);
			if((c != ",") && (n < 2304)) {
				map->map[n] = (u16)atoi(c);
				n++;
			}
		} while(currentChar != EOF);
		
		fclose(file);
	} else {
		printf("Error while reading map %i\nFilename: %s", map->id, filename);
		while(1) { swiWaitForVBlank(); }
	}
}
