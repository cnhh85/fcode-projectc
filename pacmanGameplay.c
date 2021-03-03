#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>

//AUDIO
void playTheme() {
	PlaySound(TEXT(".audio\\theme.wav"), NULL, SND_SYNC | SND_LOOP); //PACMAN THEME
}
void playEating() {
	PlaySound(TEXT(".audio\\eating.wav"), NULL, SND_ASYNC | SND_LOOP); //EATING FOOD
}
void playDead() {
	PlaySound(TEXT(".audio\\dead.wav"), NULL, SND_ASYNC | SND_LOOP); //PACMAN DIES
}
void playMenuScroll() {
	PlaySound(TEXT(".audio\\menuscroll.wav"), NULL, SND_ASYNC | SND_LOOP); //SCROLL MENU
}
void playMenuSellect() {
	PlaySound(TEXT(".audio\\menuselect.wav"), NULL, SND_ASYNC | SND_LOOP); //MENU SELECT
}
void playMenuTheme() {
	PlaySound(TEXT(".audio\\menutheme.wav"), NULL, SND_ASYNC | SND_LOOP); //MENU MUSIC
}


void gotoxy(int x, int y)
{
	static HANDLE h = NULL;  
	if(!h)
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };  
	SetConsoleCursorPosition(h,c);
}

void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void changeColor ( int code ) {
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , code );
}



// START  ------------------------------------------------------------------------

// SAVE POSITION OF OBJECT

enum state {STOP, UP, DOWN, RIGHT, LEFT};

struct point {
	int x, y;
};

// CHARACTER

struct character {
	char shape, competitorShape;
	enum state direct;
	struct point pos;
};

// BUILD QUEUE FOR FINDING PACMAN OF GHOST

struct point queue[1005], trace[105][105];;
int front = 0, back = 0;

void push(struct point value) {
	queue[back++] = value; 
}

void pop() {
	++front;
}

void resetQueue() {
	front = back = 0;
}


struct point move[5] = {
	{0,0} , // STOP
	{-1,0}, // UP
	{1,0} , // DOWN
	{0,1} , // RIGHT
	{0,-1}, // LEFT
};

char moveGhost[4] = {'a', 'w', 'd', 's'};

int consoleWidth;
int consoleHeight;
int nFood = 300;
int nGhost, choiceMode = 1;
bool supperPacman = false;
int speed, mapChoice;

int score = 0, preScore = 0, timeGame = 0;
bool loseGame = false, winGame = false, mark[105][105];
char buffer[105][105];

// MAP

char map[105][105];
struct point mapLen[2] = {
	{33, 19},
	{58, 23},
};


maps[5][105][105] = {
		{ //33x19
		{219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219}, //1
		{219, ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', 219}, //2
		{219, ' ', ' ', 178, ' ', 178, ' ', 178, 178, 178, 178, 178, 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, 178, 178, 178, 178, 178, ' ', 178, ' ', 178, ' ', ' ', 219}, //3
		{219, 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, 219}, //4
		{219, ' ', 178, 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, 178, ' ', 178, ' ', 178, 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, 178, ' ', 219}, //5
		{219, ' ', ' ', ' ', ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, 178, ' ', 178, ' ', 178, 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', ' ', ' ', ' ', 219}, //6
		{219, 177, ' ', 178, ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', 178, ' ', 177, 219}, //7
		{219, ' ', ' ', ' ', ' ', 178, 178, 178, ' ', 178, ' ', 178, 178, ' ', 178, 178, 178, 178, 178, ' ', 178, 178, ' ', 178, ' ', 178, 178, 178, ' ', ' ', ' ', ' ', 219}, //8
		{219, 177, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 177, 219}, //9
		{219, ' ', ' ', 177, ' ', 177, 177, 177, 177, 177, 177, ' ', 178, 178, ' ', 178, 178, 178, ' ', 178, 178, ' ', 177, 177, 177, 177, 177, 177, ' ', 177, ' ', ' ', 219}, //10
		{219, ' ', ' ', 177, ' ', 177, ' ', 177, ' ', 177, ' ', ' ', 178, 219, ' ', 178, 178, 178, ' ', 219, 178, ' ', ' ', 177, ' ', 177, ' ', 177, ' ', 177, ' ', ' ', 219}, //11
		{219, ' ', 177, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 177, ' ', 219}, //12
		{219, ' ', ' ', ' ', 177, ' ', 177, ' ', 177, 177, ' ', ' ', ' ', ' ', 173, ' ', 178, ' ', 173, ' ', ' ', ' ', ' ', 177, 177, ' ', 177, ' ', 177, ' ', ' ', ' ', 219}, //13
		{219, ' ', ' ', 177, ' ', ' ', ' ', ' ', 177, ' ', ' ', 178, 178, ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, 178, ' ', ' ', 177, ' ', ' ', ' ', ' ', 177, ' ', ' ', 219}, //14
		{219, ' ', 177, ' ', ' ', 177, 177, ' ', 177, ' ', 178, 178, ' ', ' ', 178, ' ', 178, ' ', 178, ' ', ' ', 178, 178, ' ', 177, ' ', 177, 177, ' ', ' ', 177, ' ', 219}, //15
		{219, ' ', 177, ' ', 177, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', 178, 178, ' ', 178, ' ', 178, 178, ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', 177, ' ', 177, ' ', 219}, //16
		{219, ' ', ' ', ' ', ' ', ' ', 177, ' ', 177, ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', 177, ' ', 177, ' ', ' ', ' ', ' ', ' ', 219}, //17
		{219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219}  //19
	},
	{ //58x23
		{219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219 ,219},
		{219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178, 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 219},
		{219,   ' ',   ' ', 219,   ' ',   ' ', 219, 219, 219, 219, 219, 219, 219, 219,   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ',   ' ', 219},
		{219,   ' ',   ' ', 219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178, 178,   ' ',   ' ', 178, 178, 178, 178, 178,   ' ',   ' ', 178, 178, 178,   ' ',   ' ', 178, 178, 178,   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ', 178, 219},
		{219, 219, 219, 219, 219, 219,   ' ', 219, 219, 219, 219, 219, 219, 219,   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 178, 178, 178,   ' ',   ' ', 178, 178, 178, 178, 178,   ' ',   ' ', 178, 178, 178,   ' ',   ' ', 178, 178, 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178, 178,   ' ', 178,   ' ', 178,   ' ',   ' ', 219},
		{219,  178,   178,   178,   178, 219,   ' ', 219,   178,   178,   178,   178,   178, 219,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 219},
		{219,  178,   178,   178,   178, 219,   ' ', 219,   178,   178,   178,   178,   178, 219,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ', 219},
		{219, 219, 219, 219, 219, 219,   ' ', 219, 219, 219, 219, 219, 219, 219,   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ', 219, 219, 219, 176,   ' ', 176, 219, 219, 219,   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178, 178,   ' ', 219},
		{219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178, 178,   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ', 219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 219,   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ', 178, 178,   ' ', 178, 178,   ' ', 219},
		{219,   ' ', 178, 178,   ' ', 178, 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 178, 178,   ' ', 178, 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 219,   ' ',  ' ',   219,   ' ',   219,   ' ',   ' ', 219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 219},
		{219,   ' ', 178, 178,   ' ', 178, 178, 178,   ' ', 178, 178,   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ', ' ',   ' ',   ' ',   219,   ' ',   219,   ' ',   ' ', ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 219},
		{219,   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178,   ' ', 178, 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ', 219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 219,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ', 178, 178, 178, 219},
		{219,   ' ', 178,   ' ', 178,   ' ', 178, 178,   ' ',   ' ', 178,   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ', 219, 219, 219, 219, ' ', 219, 219, 219, 219,   ' ',   ' ',   ' ', 178, 178, 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 219},
		{219,   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178,   ' ', 178,   ' ', 178,   ' ', 178, 178, 178, 219},
		{219,   ' ',   ' ', 178,   ' ', 178, 178,   ' ', 178, 178, 178,   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178, 178, 178,   ' ', 178, 178, 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 219},
		{219, 178,   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ', 178, 178,   ' ', 178,   ' ', 178, 178,   ' ',   ' ',   ' ', 178, 178,   ' ', 178,   ' ', 178,   ' ', 178, 178,   ' ', 219},
		{219,   ' ',   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 178, 178,   ' ', 178,   ' ', 178, 178, 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ', 219},
		{219,   ' ',   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178, 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ', 178, 178,   ' ', 178, 178,   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178, 219},
		{219, 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178, 178,   ' ', 178,   ' ', 178, 178, 178, 178, 178,   ' ',   ' ',   ' ', 178, 178, 219},
		{219,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178,   ' ', 178,   ' ',   ' ', 178,   ' ', 178,   ' ', 178,   ' ', 178, 178, 178,   ' ',   ' ',   ' ', 178,   ' ', 178, 178, 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 178,   ' ',   ' ',   ' ', 219},
		{219,   ' ', 178,  178,   ' ', 178, 178, 178, 178, 178,  ' ', 178,  ' ',  178, 178, 178, 178, 178,   ' ', 178,   ' ',   ' ', 178,   ' ',   ' ', 178,   ' ', 178, 178,   ' ',   ' ',   ' ', 178, 178,   ' ', 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178, 178, 178, 178,   ' ', 178,   ' ', 178, 178, 178, 178, 178,   ' ', 219},
		{219,   ' ',   ' ',   ' ',   ' ',   178,   ' ',   ' ',   ' ',   ' ',   ' ',' ',' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 178, 178, 178, 178,   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ', 219},				
		{219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219 ,219}
	},
};

void setting(int mapIndextmp, int speedTmp) {
	speed = speedTmp;
	mapChoice = mapIndextmp;
}

void initMap(int mapIndex) {
	consoleHeight = mapLen[mapIndex].y;
	consoleWidth = mapLen[mapIndex].x;
	for (int i = 0; i < consoleHeight; ++i)
		for (int j = 0; j < consoleWidth; ++j)
			map[i][j] = maps[mapIndex][i][j];
}

void initBuffer() {
	for (int i = 0; i < consoleHeight; ++i)
		for (int j = 0; j < consoleWidth; ++j)
			buffer[i][j] = map[i][j];
}

void initCharacter(struct character *pacman, struct character *ghost) {
	// INIT PACMAN--------------------------------------------------

	(*pacman).shape = -108;
	(*pacman).competitorShape = -114;
	(*pacman).pos.x = 1;
	(*pacman).pos.y = 1;
	(*pacman).direct = STOP;	
	
	// INIT GHOST----------------------------------------------------
	
	for (int i = 0; i < nGhost; ++i) {
		(*(ghost + i)).shape = -114;
		(*(ghost + i)).competitorShape = -108;
		(*(ghost + i)).pos.x = consoleHeight - 3;
		(*(ghost + i)).pos.y = consoleWidth - 3;
		(*(ghost + i)).direct = STOP;
	}
	
	// INIT FOOD----------------------------------------------------
	
	int x, y;
	for (int i = 1; i <= nFood; ++i) {
		do {
			x = rand() % consoleHeight;
			y = rand() % consoleWidth;
		}
		while ((x == 1 && y == 1) || (x == consoleHeight && y == consoleWidth) || map[x][y] != ' ');
		map[x][y] = '*';
	}

}

void changeDirect(struct character *chacr, char key) {
	if (key == 'W' || key == 'w' || key == 72) (*chacr).direct = UP;
	if (key == 'S' || key == 's' || key == 80) (*chacr).direct = DOWN;
	if (key == 'a' || key == 'A' || key == 75) (*chacr).direct = LEFT;
	if (key == 'd' || key == 'D' || key == 77) (*chacr).direct = RIGHT;
}

void showBuffer() {
	changeColor(2);
	int x = 30 , y = 0;
	for (int i = 0; i <= consoleHeight; ++i) {
		x = 30;
		for (int j = 0; j <= consoleWidth; ++j) {
			gotoxy(x, y);
			if (buffer[i][j] == -108) {
				// CHANGE COLOR PACMAN
				if (supperPacman) changeColor(5);
					else changeColor(7);
				putchar(buffer[i][j]);
				changeColor(2);
			}
			else
				if (buffer[i][j] == -114) {
					// CHANGE COLOR GHOST 
					changeColor(9 + rand() % 4);
					putchar(buffer[i][j]);
					changeColor(2);
				}
				else
					if (buffer[i][j] == '*') {
						// CHANGE COLOR FOOD ( WHITE)
						changeColor(15);
						putchar(buffer[i][j]);
						changeColor(2);
					}
					else 
						if (buffer[i][j] == 'F') {
							changeColor(9 + rand() % 4);
							putchar(buffer[i][j]);
							changeColor(2);
						}
						else 
							putchar(buffer[i][j]);
			++x;
		}
		++y;
		putchar('\n');
	}
}

void checkFood(struct character *chart, int *score) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == '*' && (*chart).direct == DOWN) {
		map[(*chart).pos.x + 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == '*' && (*chart).direct == UP) {
		map[(*chart).pos.x - 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == '*' && (*chart).direct == RIGHT) {
		map[(*chart).pos.x][(*chart).pos.y + 1] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == '*' && (*chart).direct == LEFT) {
		map[(*chart).pos.x][(*chart).pos.y - 1] = ' ';
		++ *score;
	}
}

void checkSupperfood(struct character *chart, int *score) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == 'F' && (*chart).direct == DOWN) {
		supperPacman = true;
		map[(*chart).pos.x + 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == 'F' && (*chart).direct == UP) {
		supperPacman = true;
		map[(*chart).pos.x - 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == 'F' && (*chart).direct == RIGHT) {
		supperPacman = true;
		map[(*chart).pos.x][(*chart).pos.y + 1] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == 'F' && (*chart).direct == LEFT) {
		supperPacman = true;
		map[(*chart).pos.x][(*chart).pos.y - 1] = ' ';
		++ *score;
	}
}

void checkTouchCompetitor(struct character *chart) {
	if (buffer[(*chart).pos.x][(*chart).pos.y] == (*chart).competitorShape) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 3;
				(*chart).pos.y = consoleWidth - 3;
			}
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == (*chart).competitorShape && (*chart).direct == DOWN) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 3;
				(*chart).pos.y = consoleWidth - 3;
			}
			return;
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == (*chart).competitorShape && (*chart).direct == UP) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 3;
				(*chart).pos.y = consoleWidth - 3;
			}
			return;
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == (*chart).competitorShape && (*chart).direct == RIGHT) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 3;
				(*chart).pos.y = consoleWidth - 3;
			}
			return;
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == (*chart).competitorShape && (*chart).direct == LEFT) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 3;
				(*chart).pos.y = consoleWidth - 3;
			}
			return;
		}
		loseGame = true;
	}
}

void checkSupperpacmantouchghost(struct character *pacman, struct character *ghosts) {
	if (buffer[(*pacman).pos.x][(*pacman).pos.y] == -114) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x && (*(ghosts + i)).pos.y == (*pacman).pos.y) {
				(*(ghosts + i)).pos.x = consoleHeight - 3;
				(*(ghosts + i)).pos.y = consoleWidth - 3;
				return;
			}
	}
	if (buffer[(*pacman).pos.x + 1][(*pacman).pos.y] == -114 && (*pacman).direct == DOWN) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x + 1 && (*(ghosts + i)).pos.y == (*pacman).pos.y) {
				(*(ghosts + i)).pos.x = consoleHeight - 3;
				(*(ghosts + i)).pos.y = consoleWidth - 3;
				return;
			}
	}
	if (buffer[(*pacman).pos.x - 1][(*pacman).pos.y] == -114 && (*pacman).direct == UP) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x - 1 && (*(ghosts + i)).pos.y == (*pacman).pos.y) {
				(*(ghosts + i)).pos.x = consoleHeight - 3;
				(*(ghosts + i)).pos.y = consoleWidth - 3;
				return;
			}
	}
	if (buffer[(*pacman).pos.x][(*pacman).pos.y + 1] == -114 && (*pacman).direct == RIGHT) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x && (*(ghosts + i)).pos.y == (*pacman).pos.y + 1) {
				(*(ghosts + i)).pos.x = consoleHeight - 3;
				(*(ghosts + i)).pos.y = consoleWidth - 3;
				return;
			}
	}
	if (buffer[(*pacman).pos.x][(*pacman).pos.y - 1] == -114 && (*pacman).direct == LEFT) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x && (*(ghosts + i)).pos.y == (*pacman).pos.y - 1) {
				(*(ghosts + i)).pos.x = consoleHeight - 3;
				(*(ghosts + i)).pos.y = consoleWidth - 3;
				return;
			}
	}
}

void checkTouchWalls(struct character *chart) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] != ' ' && buffer[(*chart).pos.x + 1][(*chart).pos.y] != '*' && buffer[(*chart).pos.x + 1][(*chart).pos.y] != 'F' && (*chart).direct == DOWN) (*chart).direct = STOP;
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] != ' ' && buffer[(*chart).pos.x - 1][(*chart).pos.y] != '*' && buffer[(*chart).pos.x - 1][(*chart).pos.y] != 'F' && (*chart).direct == UP) (*chart).direct = STOP;
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] != ' ' && buffer[(*chart).pos.x][(*chart).pos.y + 1] != '*' && buffer[(*chart).pos.x][(*chart).pos.y + 1] != 'F' &&(*chart).direct == RIGHT) (*chart).direct =  STOP;
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] != ' ' && buffer[(*chart).pos.x][(*chart).pos.y - 1] != '*' && buffer[(*chart).pos.x][(*chart).pos.y - 1] != 'F' && (*chart).direct == LEFT) (*chart).direct = STOP;
}

void solvePacman(struct character *pacman, struct character *ghosts, int *score) {
	
	// CHECK EATING FOOD--------------------------------------------------------------------------
	
	checkFood(pacman, score);
	checkSupperfood(pacman, score);
	if (*score == nFood) winGame = true;
	
	// CHECK TOUCHING GHOST-----------------------------------------------------------------------
	
	
	if (supperPacman == true) checkSupperpacmantouchghost(pacman, ghosts);
	else checkTouchCompetitor(pacman);
	
	// CHECK TOUCHING WALLS-----------------------------------------------------------------------

	checkTouchWalls(pacman);
	
}

void solveGhost(struct character *ghost) {
	
	// CHECK TOUCHING PACMAN-----------------------------------------------------------------------
	
	checkTouchCompetitor(ghost);
	
	// CHECK TOUCHING WALLS------------------------------------------------------------------------
	
	checkTouchWalls(ghost);
}

void moveCharacter(struct character *chart) {
	(*chart).pos.x += move[(int) (*chart).direct].x;
	(*chart).pos.y += move[(int) (*chart).direct].y;
}

void findPacman(struct character *ghost,struct character *pacman) {
	// INIT 
	
	for (int i = 0; i <= 99; ++i)
		for (int j = 0; j <= 99; ++j) {
			mark[i][j] = false;
			trace[i][j].x = 0;
			trace[i][j].y = 0;
		}
		
	resetQueue();
	struct point tmp, tmp1;
	bool ok = false;
	tmp.x = (*ghost).pos.x;
	tmp.y = (*ghost).pos.y;
	// BFS 
	push(tmp);
	while (front <= back) {
		tmp = queue[front];
		pop();
		if (buffer[tmp.x][tmp.y] == (*pacman).shape) {
			ok = true;
			break;
		}
		for (int i = 1; i <= 4; ++i) {
			tmp1.x = tmp.x + move[i].x;
			tmp1.y = tmp.y + move[i].y;
			if ((buffer[tmp1.x][tmp1.y] != 'F' && buffer[tmp1.x][tmp1.y] != ' ' && buffer[tmp1.x][tmp1.y] != '*' && buffer[tmp1.x][tmp1.y] != -114 && buffer[tmp1.x][tmp1.y] != (*pacman).shape) || mark[tmp1.x][tmp1.y]) continue;
			mark[tmp1.x][tmp1.y] = true;
			push(tmp1);
			trace[tmp1.x][tmp1.y].x = tmp.x;
			trace[tmp1.x][tmp1.y].y = tmp.y;
		}
	}
	if (ok == false) return;
	// TRACE
	tmp.x = (*pacman).pos.x;
	tmp.y = (*pacman).pos.y;
	while (true) {
		if (trace[tmp.x][tmp.y].x == (*ghost).pos.x && trace[tmp.x][tmp.y].y == (*ghost).pos.y) break;
		tmp = trace[tmp.x][tmp.y];
	}
	
	char key;
	if (tmp.x == (*ghost).pos.x - 1 && tmp.y == (*ghost).pos.y) key = 'w';
	if (tmp.x == (*ghost).pos.x + 1 && tmp.y == (*ghost).pos.y) key = 's';
	if (tmp.x == (*ghost).pos.x && tmp.y == (*ghost).pos.y - 1) key = 'a';
	if (tmp.x == (*ghost).pos.x && tmp.y == (*ghost).pos.y + 1) key = 'd';
	
	changeDirect(ghost, key);
	
}

void modeEasy(int mapIndexCus, int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(0, 50);
	if (custom) setting(mapIndexCus, speedCus);
	nGhost = 3;
	score = 0;
	preScore = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	int xFood = -1, yFood = -1, timeSupperpacman = 0;
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
	
	
		if (loseGame == true) {	
			system("cls");
			gotoxy(55,13);
			changeColor(4);
			printf("YOU LOSE !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;	
		}
		
		if (winGame == true) {
			system("cls");
			gotoxy(55,8);
			changeColor(4);
			printf("YOU WIN !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;
		}
				
		// CLEAR----------------------------------
		
		gotoxy(47,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		// CACULATE SCORE-------------------------
		
		if (score != preScore) {
			changeColor(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 100);
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
			char key = _getch();
			if (key == 32) {
				while (true) {
					if (kbhit()) {
						key = _getch();
						if (key == 32) break;
					}
				}
			}
				else 
					if (key == 27) {
						system("cls");
						break;
					}
						else changeDirect(&pacman, key);
		}
		
		if (timeGame % 3 == 0) {
			for (int i = 0; i < nGhost; ++i)
				changeDirect(&ghosts[i], moveGhost[rand() % 4]);
		}		
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman , &ghosts, &score);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		
		
		if (timeGame >= 20) {
			if (supperPacman == true) ++timeSupperpacman;
			if (timeSupperpacman == 70) {
				supperPacman = false; 
				timeSupperpacman = 0;
			}
			if (timeSupperpacman == false && timeGame % 50 == 0) {
				++nFood;
				if (xFood >= 0) map[xFood][yFood] = ' ';
				while (true) {
					xFood = rand() % consoleHeight;
					yFood = rand() % consoleWidth;
					if (buffer[xFood][yFood] == ' ') break;
				}
				map[xFood][yFood] = 'F';
			}
		}
		
		++timeGame;
		timeGame %= 100;
		Sleep(speed);
	}
}

void modeHard(int mapIndexCus, int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(1, 50);
	if (custom) setting(mapIndexCus, speedCus);
	nGhost = 1;
	score = 0;
	preScore = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	int xFood = -1, yFood, timeSupperpacman = 0;
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
		
		if (loseGame == true) {	
			system("cls");
			gotoxy(55,13);
			changeColor(4);
			printf("YOU LOSE !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;	
		}
		
		if (winGame == true) {
			system("cls");
			gotoxy(55,8);
			changeColor(4);
			printf("YOU WIN !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;
		}
		
		// CLEAR----------------------------------
		
		gotoxy(47,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		// CACULATE SCORE-------------------------
		
		if (score != preScore) {
			changeColor(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 100);
		
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
				char key = _getch();
				if (key == 32) {
					while (true) {
						if (kbhit()) {
							key = _getch();
							if (key == 32) break;
						}
					}
				}
					else 
						if (key == 27) {
							system("cls");
							break;
						}
							else changeDirect(&pacman, key);
		}
		
		for (int i = 0; i < nGhost; ++i)
				findPacman(&ghosts[i], &pacman);
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman, &ghosts, &score);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		if (timeGame >= 20) {
			if (supperPacman == true) ++timeSupperpacman;
			if (timeSupperpacman == 70) {
				supperPacman = false; 
				timeSupperpacman = 0;
			}
			if (timeSupperpacman == false && timeGame % 50 == 0) {
				++nFood;
				if (xFood >= 0) map[xFood][yFood] = ' ';
				while (true) {
					xFood = rand() % consoleHeight;
					yFood = rand() % consoleWidth;
					if (buffer[xFood][yFood] == ' ') break;
				}
				map[xFood][yFood] = 'F';
			}
		}
		
		++timeGame;
		timeGame %= 100;
		Sleep(speed);
	}
}

void modeFaker(int mapIndexCus, int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(1, 0);
	if (custom) setting(mapIndexCus, speedCus);
	nGhost = 2;
	score = 0;
	preScore = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	int xFood = -1, yFood, timeSupperpacman;
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
	
		if (loseGame == true) {	
			system("cls");
			gotoxy(55,13);
			changeColor(4);
			printf("YOU LOSE !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;	
		}
		
		if (winGame == true) {
			system("cls");
			gotoxy(55,8);
			changeColor(4);
			printf("YOU WIN !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;
		}
		
		// CLEAR----------------------------------
		
		gotoxy(47,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		if (score != preScore) {
			changeColor(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 100);
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
			char key = _getch();
			if (key == 32) {
				while (true) {
					if (kbhit()) {
						key = _getch();
						if (key == 32) break;
					}
				}
			}
				else 
					if (key == 27) {
						break;
						system("cls");
					}
						else changeDirect(&pacman, key);
		}
	
		timeGame %= 5;
		if (timeGame == 0) {
			for (int i = 0; i < nGhost; ++i)
				changeDirect(&ghosts[i], moveGhost[rand() % 4]);
		}
		else
			for (int i = 0; i < nGhost; ++i)
				findPacman(&ghosts[i], &pacman);
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman, &ghosts, &score);
		
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		if (timeGame >= 20) {
			if (supperPacman == true) ++timeSupperpacman;
			if (timeSupperpacman == 70) {
				supperPacman = false; 
				timeSupperpacman = 0;
			}
			if (timeSupperpacman == false && timeGame % 50 == 0) {
				++nFood;
				if (xFood >= 0) map[xFood][yFood] = ' ';
				while (true) {
					xFood = rand() % consoleHeight;
					yFood = rand() % consoleWidth;
					if (buffer[xFood][yFood] == ' ') break;
				}
				map[xFood][yFood] = 'F';
			}
		}
		
		++timeGame;
		timeGame %= 100;
		Sleep(speed);
	}
}

void modeTwoplayers(int mapIndexCus, int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(0, 0);
	if (custom) setting(mapIndexCus, speedCus);
	nGhost = 1;
	nFood = 300;
	int scorePacman = 0 , prescorePacman = 0;
	int scoreGhost = 0 , prescoreGhost = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
	
		if (winGame == true) {
			system("cls");
			gotoxy(55,8);
			changeColor(4);
			if (scorePacman > scoreGhost)
				printf("PACMAN WIN !!!!\n");
			else
				printf("GHOST WIN !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;
		}
		
		// CLEAR----------------------------------
		
		gotoxy(47,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		if (scorePacman != prescorePacman) {
			changeColor(rand() % 16);
			prescorePacman = scorePacman;
		}
		printf("Pacman score : %d\n\n", scorePacman * 10);
		
		
		changeColor(2);
		if (scoreGhost != prescoreGhost) {
			changeColor(rand() % 16);
			prescoreGhost = scoreGhost;
		}
		printf("Pacman score : %d", scoreGhost * 10);
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
			char key = _getch();
			if (key == 'w' || key == 'W' || key == 'a' || key == 'A' || key == 's' || key == 'S' || key == 'd' || key == 'D') {
				changeDirect(&pacman, key);
			}
				else 
					if (key == 32) {
						while (true) {
							if (kbhit()) {
								key = _getch();
								if (key == 32) break;
							}
						}
					}
						else 
							if (key == 27) {
								break;
								system("cls");
							}
								else changeDirect(&ghosts[0], key);
		}
	
		
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman, &ghosts, &scorePacman);
		
		solvePacman(&ghosts[0], &ghosts, &scoreGhost);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(speed);
	}
}

void modeReverse(int mapIndexCus, int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(0, 1);
	if (custom) setting(mapIndexCus, speedCus);
	nGhost = 1;
	score = 0;
	preScore = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	int x, y;
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
		
		if (winGame == true) {	
			system("cls");
			gotoxy(55,13);
			changeColor(4);
			printf("YOU LOSE !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;	
		}
		
		if (loseGame == true) {
			system("cls");
			gotoxy(55,8);
			changeColor(4);
			printf("YOU WIN !!!!\n");
			int cnt = 0;
			char tmp;
			while (true) {
				if (kbhit()) {
					++cnt;
					tmp = _getch();
				}
				if (cnt >= 2) break;
			}
			system("cls");
			break;
		}
		
		// CLEAR----------------------------------
		
		gotoxy(47,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		// CACULATE SCORE-------------------------
		
		if (score != preScore) {
			changeColor(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", timeGame);
		
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
				char key = _getch();
				if (key == 32) {
					while (true) {
						if (kbhit()) {
							key = _getch();
							if (key == 32) break;
						}
					}
				}
					else 
						if (key == 27) {
							system("cls");
							break;
						}
							else changeDirect(&ghosts[0], key);
		}
		if (timeGame % 10 == 0 || abs(ghosts[0].pos.x - x) == 0 || abs(ghosts[0].pos.y - y) == 0 || (pacman.pos.x == x && pacman.pos.y == y)) {
			while (true) {
				x = rand() % consoleHeight;
				y = rand() % consoleWidth;
				if (abs(ghosts[0].pos.x - x) + abs(ghosts[0].pos.y - y) >= 20) {
					if (abs(ghosts[0].pos.x - x) != 0 && abs(ghosts[0].pos.y - y) != 0)
						if (buffer[x][y] == '*') break;
				}
			}
		}
		struct character ghostFake;
		ghostFake.pos.x = x;
		ghostFake.pos.y = y;
		ghostFake.shape = -101;
		char tmp = buffer[x][y];
		buffer[x][y] = ghostFake.shape;
		findPacman(&pacman, &ghostFake);
		buffer[x][y] = tmp;
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman, &ghosts, &score);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(speed);
	}
}

int main() {
	
	 //MENU
	
	printf("MENU :\n");
	printf("1.EASY\n");
	printf("2.HARD\n");
	printf("3.FAKER\n");
	printf("4.TWO PLAYERS\n");
	printf("5.REVERSE\n");
	
	char choice = _getch();
	
	if (choice == '1') {
		
		system("cls");
		// EASY MODE 
		modeEasy(0, 0, 0);

	}
	else
		if (choice == '2') {
			
			system("cls");
			// HARD MODE
			modeHard(0, 0, 0);
			
		}
			else 
				if (choice == '3') {
				
				system("cls");
				// FAKER MODE 
				modeFaker(0, 0, 0);
				
				}
					else  
						if (choice == '4') {
							system("cls"); 
							//TWO PLAYERS MODE
							modeTwoplayers(0, 0, 0);
						}
							else {
								system("cls");
								// REVERSE MODE
								modeReverse(0, 0, 0);
							}
	return 0;
}
