/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 15:51:14
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include <nds.h>

#include "Game.hpp"

int main(int, char *[]) {
	Game game;
	
	while(1) swiWaitForVBlank();
	
	return 0;
}

