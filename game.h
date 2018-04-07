

typedef enum {
	STATE_START,
	STATE_RUN,
	GAMEOVER,
	STATE_TITLE,
	STATE_DISPLAYSCREEN
} GBAState;

extern GBAState state;

void startGame();
void run();
void reset();
void displaytitle();
void displayScreen();
void moveEnemies();


