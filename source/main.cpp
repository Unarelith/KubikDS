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
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_0_2D);
    
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
    vramSetBankC(VRAM_C_SUB_BG);
	
    // initialize the console
    consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
    // initialize the oam
    oamInit(&oamMain, SpriteMapping_1D_128, false);
    
	// initialize the background
    u16* bgGfx = bgGetGfxPtr(bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0));
    
    Player* player = new Player;
	Level* level1 = new Level(32, map1_1, ARGB16(1,0,31,31), bgGfx);
	
	player->setLevel(level1, 176);
	
	while(1) {
		swiWaitForVBlank();
		
        scanKeys();
		
		level1->draw();
		
        player->move();
        player->draw();
		
		oamUpdate(&oamMain);
	}
}
