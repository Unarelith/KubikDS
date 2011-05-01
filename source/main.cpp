/*---------------------------------------------------------------------------------

	ShootDS
	- Quent42340

---------------------------------------------------------------------------------*/
#include "main.h"
#include "sprite.h"
#include "player.h"
#include "level.h"
#include "levelsData.h"

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
    oamInit(&oamMain, SpriteMapping_1D_128, false);
    
	// initialize the background
	int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);
    
    Player* player = new Player;
	Level* level1 = new Level(&map1, bg);
	
	player->setLevel(level1);
	
	while(1) {
		swiWaitForVBlank();
		
        scanKeys();
		
        player->move();
        player->draw();
		
		bgUpdate();
		oamUpdate(&oamMain);
	}
}
