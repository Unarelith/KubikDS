/*---------------------------------------------------------------------------------

	ShootDS
	- Quent42340

---------------------------------------------------------------------------------*/
#include "main.h"
#include "game.h"

int main(void) {
	// Initialize random seed
	srand(time(NULL));
	
	// Setup the video modes
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);
	
	// Setup VRAM banks
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG);
	
	// Initialize the console
	PrintConsole bottomScreen;
	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	// Initialize the oam ( Sprite system )
	oamInit(&oamMain, SpriteMapping_Bmp_1D_256, false);
	
	// Initialize game
	Game* game = new Game;
	
	while(1);
}
