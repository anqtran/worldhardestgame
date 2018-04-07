#include "game.h"
#include "myLib.h"
#include "img/startScreen.h"
#include "text.h"
#include "stdio.h"
#include <stdbool.h>

#define NENEMIES 4
void drawFullscreenImage3(const u16* image);
void waitForVblank();
void drawString(int row, int col, char str[], unsigned short color);
void drawRect3(int row, int col, int height, int width, unsigned short color);
char str[60];
void setPixel(int row, int col, unsigned short color);
void drawHorizontalLine(int row, int col, int width, unsigned short color);
void drawVerticalLine(int row, int col, int height, unsigned short color);
void drawEnemy(ENEMY* enemy, unsigned short color);
GBAState state;


void startGame() {
	// sprintf(str, "Press Enter to start");
	// drawFullscreenImage3(startScreen);
	// drawString(80, 60, str, GREEN);
	// state = STATE_TITLE;
	state = STATE_DISPLAYSCREEN;
} 

void displaytitle() {
	bool start_down = false;
	if(KEY_DOWN_NOW(BUTTON_START)) {
		if(!start_down) {
			start_down = true;
			state = STATE_RUN;
	}
	} else {
		if(start_down)
		start_down = false;
	}

}

void displayScreen() {
	drawRect3(0, 0, 160, 240, GRAY);
	int r = 30;
	int c = 20;
	// int width = 240 -2*c;
	int height = 160 - 2*r;

	

	drawHollowRect3(r + 89, c + 30 , 11, 20, BLACK);
	drawHollowRect3(r, c + 150, 11, 20, BLACK);	

	drawHollowRect3(r + 10, c + 40 , 80, 120, BLACK);


	drawRect3(r,c, height, 30, CYAN);
	drawHollowRect3(r,c, height, 30, BLACK);
	drawRect3(r,c + 170, height, 30, CYAN);
	drawHollowRect3(r, c + 170, height, 30, BLACK);

	drawHorizontalLine(r + 89, c + 41, 9, GRAY);
	drawVerticalLine(r + 90,  c + 30, 9, GRAY);
	drawHorizontalLine(r + 10, c + 41 + 110, 9, GRAY); 
	drawVerticalLine(r + 1, c + 140 + 30, 9, GRAY);
	// drawHollowRect3(r,c + width - 30, height, 30, BLACK);
	// drawHollowRect3(r + 90, c + 150 , 10, 20, BLUE);

	state = STATE_RUN;	
}
void run() {
	moveEnemies();
}

void reset() {

}

void moveEnemies() {
	int size = 16;
	int row = 30;
	int col = 20;
	unsigned int enemiestartRow = row + 20;
	ENEMY enemies[NENEMIES];
	ENEMY oldenemies[NENEMIES];
	player P;
	P.row = row + 100/2;
	P.col = col + 15;
	P.size = 5;
	drawPlayer(&P);
	for (int i = 0; i < NENEMIES; i++) {
		enemies[i].row = enemiestartRow + i*20;
		enemies[i].col = 240/2;
		enemies[i].rd = 0;
		enemies[i].size = size;
		if (i % 2 == 0)
		{
			enemies[i].cd = 2;
		} else {
			enemies[i].cd = -2;
		}
		oldenemies[i] = enemies[i];
	}
	while(1) {
		for (int i = 0; i < NENEMIES; i++) {
			ENEMY *enemy = &enemies[i];
			enemies[i].col = enemy->col + enemy->cd;
			if(enemy->col < col + 41 + size /2) {
				enemy->col = col + 42 + size /2;
				enemy->cd = -enemy->cd;
			}
			if(enemy->col > col + 160 -1 - size/2) {
				enemy->col =  col + 158 - size/2;
				enemy->cd = -enemy->cd;
			}
		}
		waitForVblank();
		for (int i = 0; i < NENEMIES; i++) {
			drawEnemy(&oldenemies[i], GRAY);
		}
		for (int i = 0; i < NENEMIES; i++) {
			drawEnemy(&enemies[i], RED);
			enemies[i].size = size;
			oldenemies[i] = enemies[i];
		}
	}
}

