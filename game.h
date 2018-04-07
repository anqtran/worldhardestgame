

typedef enum {
	STATE_START,
	STATE_RUN,
	STATE_GAMEOVER,
	STATE_TITLE,
	STATE_DISPLAYSCREEN,
	STATE_WIN

} GBAState;

extern GBAState state;

void startGame();
void run();
void lastScreen();
void displaytitle();
void displayScreen();
void moveEnemies();
void setUp();
void winGame();