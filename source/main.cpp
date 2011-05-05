/*---------------------------------------------------------------------------------

	ShootDS
	- Quent42340

---------------------------------------------------------------------------------*/
#include "main.h"
#include "sprite.h"
#include "player.h"
#include "level.h"
#include "levelsData.h"
#include "enemy.h"
#include "game.h"

#include "titleScreen.h"

int main(void) {
    PrintConsole bottomScreen;
    
    srand(time(NULL));
    
    // Setup the video modes.
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
    vramSetBankC(VRAM_C_SUB_BG);
	
    // initialize the console
    consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
    // initialize the oam
    oamInit(&oamMain, SpriteMapping_Bmp_1D_256, false);
    
	// initialize the background
	int bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
	
    Player* player = new Player;
	Enemy* enemy1 = new Enemy(1, 216, 176, 1, 0);
	Enemy* enemy2 = new Enemy(2, 128, 64, 0, 1);
	Level* level1 = new Level(&map1, bg, 1);
	
	player->setLevel(level1);
	enemy1->setLevel(level1);
	enemy2->setLevel(level1);
	
	Enemy* enemies[] = {
		enemy1,
		enemy2
	};
	
	level1->setEnemies(enemies, 2);
	
	Game* game = new Game(bg, player, level1);
	
	level1->setGame(game);
	
	while(1) {
		swiWaitForVBlank();
		
		scanKeys();
		
		game->update();
		
		bgUpdate();
		oamUpdate(&oamMain);
	}
}