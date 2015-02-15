/*
 * =====================================================================================
 *
 *       Filename:  LevelsData.cpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:10:32
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include <cstdio>

#include "LevelsData.hpp"

void fill_map(Map &map) {
	map.map.resize(map.length * map.height);
	
	char filename[25];
	sprintf(filename, "efs:/level%i/map", map.id);
	
	FILE* file = fopen(filename, "r");
	int currentChar = 0;
	int n = 0;
	char c[2];
	
	if(file != nullptr) {
		printf("\x1b[11;11HLoading...");
		
		while(currentChar != EOF) {
			currentChar = fgetc(file);
			sprintf(c, "%c", currentChar);
			if((currentChar != ',') && (currentChar != '\n')) {
				map.map[n] = (u16)atoi(c);
				n++;
			}
		}
		
		fclose(file);
	} else {
		printf("Error while reading map %i\nFilename: %s", map.id, filename);
		
		while(1) { swiWaitForVBlank(); }
	}
	
	sprintf(filename, "efs:/level%i/enemies", map.id);
	
	file = fopen(filename, "r");
	char line[MAX_SIZE] = "";
	
	int id = 1;
	
	s16 x = 0;
	s16 y = 0;
	
	s16 vx = 1;
	s16 vy = 1;
	
	if(file != nullptr) {
		printf("\x1b[11;11HLoading...");
		
		while(fgets(line, MAX_SIZE, file) != nullptr) {
			sscanf(line, "%i;%hi;%hi;%hi;%hi", &id, &x, &y, &vx, &vy);
			map.enemies.emplace_back(id, x, y, vx, vy);
		}
		
		fclose(file);
	} else {
		printf("Error while reading enemies set %i\nFilename: %s", map.id, filename);
		
		while(1) { swiWaitForVBlank(); }
	}
	
	// Map params
	sprintf(filename, "efs:/level%i/params", map.id);
	
	file = fopen(filename, "r");
	
	n = 0;
	
	if(file != nullptr) {
		printf("\x1b[11;11HLoading...");
		
		while(fgets(line, MAX_SIZE, file) != nullptr) {
			sscanf(line, "%li;%li;%hi;%hi", &map.scrollX, &map.scrollY, &map.playerX, &map.playerY);
			n++;
		}
		
		fclose(file);
	} else {
		printf("Error while reading params %i\nFilename: %s", map.id, filename);
		
		while(1) { swiWaitForVBlank(); }
	}
}

