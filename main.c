#include "myLib.h"
#include "game.h"


int main () {
	// set the REG_DISPCTL for use with mode 3 and the 2nd background
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	
	state = STATE_START;

	while(1) {
		waitForVblank();
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = STATE_START;
		}
		switch(state) {
			case STATE_START:
			startGame();
			break;
			
			case STATE_TITLE:
			displaytitle();
			break;

			case STATE_DISPLAYSCREEN:
			displayScreen();
			break;

			case STATE_RUN:
			run();
			break;
			
			case STATE_WIN:
			winGame();
			break;

			case STATE_GAMEOVER:
			lastScreen();
			break;

		}
	}
	return 0;
}
