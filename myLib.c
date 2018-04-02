#include "myLib.h"
#define OFFSET(r, c, rowlen) ((c) + (rowlen)*(r))
volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;

// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}




// faster DMA loop (per row) version
void drawRect(int row, int col, int height, int width, unsigned short color) {

	for(int r=0; r<height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
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
void drawImage3(int r, int c, int width, int height, const u16*
image) {
	for (int x = 0; x < height; x++) {
		DMA[3].src = &image[OFFSET(x, 0, width)];
		DMA[3].dst = &videoBuffer[OFFSET(r + x, c, 240)];
		DMA[3].cnt = (width) | DMA_ON;
	}
}

void drawFullscreenImage3(const u16* image) {
		for (int x = 0; x < 160; x++) {
		DMA[3].src = &image[OFFSET(x, 0, 240)];
		DMA[3].dst = &videoBuffer[OFFSET(x, 0, 240)];
		DMA[3].cnt = (240) | DMA_ON;
	}
}
