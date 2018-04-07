#include "game.h"
#include "myLib.h"
#include "img/startScreen.h"
#include "img/win.h"
#include "img/doraemon.h"
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
void movePlayer();
int checkValidMove(int rd, int cd, unsigned short color);
int checkColision();
void winGame();
void lastScreen();
GBAState state;
ENEMY enemies[NENEMIES];
ENEMY oldenemies[NENEMIES];
player P;
u16 row = 30;
u16 col = 20;
unsigned int deathCount;
void startGame() {
	sprintf(str, "Press Enter to start");
	drawFullscreenImage3(startScreen);
	drawString(80, 60, str, GREEN);
	state = STATE_TITLE;
	deathCount = 0;
	
} 

void displaytitle() {
	bool start_down = false;
	if(KEY_DOWN_NOW(BUTTON_START)) {
		if(!start_down) {
			start_down = true;
			state = STATE_DISPLAYSCREEN;
		}
	} else {
		if(start_down)
			start_down = false;
	}

}

void displayScreen() {
	drawRect3(0, 0, 160, 240, GRAY);

	// int width = 240 -2*c;
	int height = 160 - 2* row;

	

	drawHollowRect3(row + 89, col + 30 , 11, 20, BLACK);
	drawHollowRect3(row, col + 150, 11, 20, BLACK);	

	drawHollowRect3(row + 10, col + 40 , 80, 120, BLACK);



	drawHollowRect3(row, col, height, 30, BLACK);
	drawRect3(row ,col + 170, height, 30, CYAN);
	drawHollowRect3(row , col + 170, height, 30, BLACK);

	drawHorizontalLine(row + 89, col + 41, 9, GRAY);
	drawVerticalLine(row + 90,  col + 30, 9, GRAY);
	drawHorizontalLine(row + 10, col + 41 + 110, 9, GRAY); 
	drawVerticalLine(row + 1, col + 140 + 30, 9, GRAY);
	sprintf(str, "Death Count: %-1d", deathCount);
	drawString(0, 0, str, GREEN);


	setUp();
	state = STATE_RUN;	
}
void run() {

	moveEnemies();
	movePlayer();
}



void setUp() {
	u16 size = 18;

	unsigned int enemiestartRow = row + 20;

	P.row = row + 30;
	P.col = col + 15;
	P.size = 3;
	drawPlayer(&P,BLACK,GREEN);
	for (int i = 0; i < NENEMIES; i++) {
		enemies[i].row = enemiestartRow + i*20;
		enemies[i].col = 240/2;
		enemies[i].size = size;
		if (i % 2 == 0)
		{
			enemies[i].cd = (i - 3);
		} else {
			enemies[i].cd = -(i - 4);
		}
		oldenemies[i] = enemies[i];
	}
}
void moveEnemies() {
	for (int i = 0; i < NENEMIES; i++) {
		ENEMY *enemy = &enemies[i];
		enemies[i].col = enemy->col + enemy->cd;
		if(enemy->col < col + 41 + enemy->size /2) {
			enemy->col = col + 42 + enemy->size /2;
			enemy->cd = -enemy->cd;
		}
		if(enemy->col > col + 160 -1 - enemy->size/2) {
			enemy->col =  col + 158 - enemy->size/2;
			enemy->cd = -enemy->cd;
		}
	}	

	waitForVblank();
	for (int i = 0; i < NENEMIES; i++) {
		drawEnemy(&oldenemies[i], GRAY);
	}
	for (int i = 0; i < NENEMIES; i++) {
		drawEnemy(&enemies[i], RED);
		oldenemies[i] = enemies[i];
	}
}
void movePlayer() {
	int rd = 0;
	int cd = 0;
	int dist = 1;
	player oldP = P;
	if (KEY_DOWN_NOW(BUTTON_DOWN)) {
		rd = dist;
	} else if (KEY_DOWN_NOW(BUTTON_LEFT)) {
		cd = -dist;
	} else if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
		cd = dist;
	} else if (KEY_DOWN_NOW(BUTTON_UP)) {
		rd = -dist;
	}

	if(checkValidMove(rd,cd,BLACK) == 1) {
		if(checkColision() == 1) {
			deathCount++;
			state = STATE_DISPLAYSCREEN;
		}
		P.row = P.row + rd;
		P.col = P.col + cd;
		drawPlayer(&oldP, GRAY, GRAY);
		}
		drawPlayer(&P, BLACK,GREEN);
	if(P.col > col + 170) {
		state = STATE_WIN;
	}
}
int checkValidMove(int rd, int cd, unsigned short color) {
	if(cd  > 0 && videoBuffer[OFFSET(P.row, P.col + cd + P.size/2 + 1, 240)] == color) {
		return 0;
	}
	else if(rd > 0 && videoBuffer[OFFSET(P.row + rd + P.size/2, P.col, 240)] == color) {
		return 0;
	}	
	
	else if(cd < 0 && videoBuffer[OFFSET(P.row, P.col + cd - P.size/2, 240)] == color) {
		return 0;
	}
	else if(rd < 0 && videoBuffer[OFFSET(P.row + rd - P.size/2, P.col, 240)] == color) {
		return 0;
	}
	return 1;
}
int checkColision() {
	int pRow = P.row;
	int pCol = P.col;
	if(videoBuffer[OFFSET(pRow, pCol + P.size/2 + 1, 240)] == RED ||
	   videoBuffer[OFFSET(pRow, pCol - P.size/2 - 1, 240)] == RED ||
	   videoBuffer[OFFSET(pRow + P.size/2 + 1, pCol, 240)] == RED ||
	   videoBuffer[OFFSET(pRow - P.size/2 - 1, pCol, 240)] == RED)
		return 1;

	return 0;
}

void winGame() {
	drawFullscreenImage3(win);
	sprintf(str, "CONGRATULATIONS");
	drawString(80, 80 , str, YELLOW);
	drawImage3(0, 20, DORAEMON_WIDTH, DORAEMON_HEIGHT, doraemon);
	state = STATE_GAMEOVER;

}


void lastScreen() { 
	if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		state = STATE_DISPLAYSCREEN;
	}

}
