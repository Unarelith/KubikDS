/*---------------------------------------------------------------------------------

	ShootDS
	- Quent42340

---------------------------------------------------------------------------------*/
#include "main.h"
#include "sprite.h"
#include "player.h"

int main(void) {
    PrintConsole bottomScreen;
	
    Player* player = new Player;
    
    srand(time(NULL));
    
    // Setup the video modes.
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_0_2D);
    
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
    vramSetBankC(VRAM_C_SUB_BG);
	
    // initialize the oam
    oamInit(&oamMain, SpriteMapping_1D_128, false);
	
    // initialize the console
    consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
    
	while(1) {
        scanKeys();
        
        player->move();
        player->draw();
		
		swiWaitForVBlank();
		
		oamUpdate(&oamMain);
	}
}
