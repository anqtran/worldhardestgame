#include "myLib.h"
#include "img/welcomeScreen.h"
#include "text.h"
#include "stdio.h"

void drawFullscreenImage3(const u16* image);
void waitForVblank();
void drawString(int row, int col, char str[], unsigned short color);
char str[60];

enum GBAState {
	STATE_START,
	STATE_LEVEL1,
	STATE_LEVEL2,
	STATE_LEVEL3,
	STATE_LEVEL4,
	STATE_LEVEL5,
};


int main () {
	// set the REG_DISPCTL for use with mode 3 and the 2nd background
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	
	enum GBAState state = STATE_START;

	bool start_down = false;

	while(1) {
		waitForVblank();

		switch(state) {
			case STATE_START:
			sprintf(str, "Welcome to the World Hardest Game!!! \n Press Start to Start!");
			drawFullscreenImage3(welcomeScreen);
			drawString(120, 80, str, GREEN);
			if(KEY_DOWN_NOW(BUTTON_START)) {
				if(!start_down) {
					start_down = true;
					state = STATE_LEVEL1;
				}
			} else {
				if(start_down)
					start_down = false;
			}
			break;

			case STATE_LEVEL1:

			break;

			case STATE_LEVEL2:

			break;

			case STATE_LEVEL3:

			break;

			case STATE_LEVEL4:

			break;

			case STATE_LEVEL5:

			break;

		}
	}
	return 0;
}
