#include "myLib.h"
volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;

// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void waitForVblank() {
	while(*SCANLINECOUNTER > 160)
		;
	while(*SCANLINECOUNTER<160)
		;
}





 /*drawImage3
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3(int r, int c, int width, int height, const u16* image) {
	for (int x = 0; x < height; x++) {
		DMA[3].cnt = 0; // clear old flags
		DMA[3].src = &image[OFFSET(x, 0, width)];
		DMA[3].dst = &videoBuffer[OFFSET(r + x, c, 240)];
		DMA[3].cnt = (width) | DMA_ON;
	}
}

void drawFullscreenImage3(const u16* image) {
	drawImage3(0, 0, 240, 160, image);
}

void drawHollowRect3(int row, int col, int height, int width, unsigned short color) {
		volatile unsigned short lineColor = color;
	 	DMA[3].cnt = 0; // clear old flags
		DMA[3].src = &lineColor;
		DMA[3].dst = &videoBuffer[OFFSET(row, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;

		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + height - 1, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;

		for(int r = row; r < height + row; r++) {
			setPixel(r, col, color);
			setPixel(r, col + width, color);
		}

}

// faster DMA loop (per row) version
void drawRect3(int row, int col, int height, int width, unsigned short color) {
		volatile unsigned short lineColor = color;
	for(int r=0; r < height; r++) {
		DMA[3].cnt = 0; // clear old flags
		DMA[3].src = &lineColor;
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawHorizontalLine(int row, int col, int width, unsigned short color) {
		volatile unsigned short lineColor = color;
		DMA[3].cnt = 0; // clear old flags
		DMA[3].src = &lineColor;
		DMA[3].dst = &videoBuffer[OFFSET(row, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
}
void drawVerticalLine(int row, int col, int height, unsigned short color) {
	for (int i = 0; i < height; ++i)
	{
		setPixel(row + i, col, color);
	}
}

void drawEnemy(ENEMY* enemy, unsigned short Ecolor) {
	volatile unsigned short color = Ecolor;
	u16 size = enemy -> size;
	drawRect3(enemy -> row - size/2, enemy -> col - size/2, size, size, color);
}

void drawPlayer(player* player) {
	volatile unsigned short border = BLACK;
	volatile unsigned short inside = GREEN;
	u16 size = player -> size;
	drawRect3(player -> row - size/2, player -> col - size/2, size, size, inside);
	drawHollowRect3(player -> row - size/2, player -> col - size/2, size, size, border);
}

