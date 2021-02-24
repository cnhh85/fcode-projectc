#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

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

void changeColorr ( int code ) {
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , code );
}
// UI


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
int speed, mapChoice = 0, modeGame = 1;

int score = 0, preScore = 0, timeGame = 0;
bool loseGame = false, winGame = false, mark[105][105];
char buffer[105][105];

// MAP

char mapCurrent[105][105];

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

void setting(int speedTmp) {
	speed = speedTmp;
}

void initMap(int mapIndex) {
	consoleHeight = mapLen[mapIndex].y;
	consoleWidth = mapLen[mapIndex].x;
	for (int i = 0; i < consoleHeight; ++i)
		for (int j = 0; j < consoleWidth; ++j)
			mapCurrent[i][j] = maps[mapIndex][i][j];
}

void initBuffer() {
	for (int i = 0; i < consoleHeight; ++i)
		for (int j = 0; j < consoleWidth; ++j)
			buffer[i][j] = mapCurrent[i][j];
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
		while ((x == 1 && y == 1) || (x == consoleHeight && y == consoleWidth) || mapCurrent[x][y] != ' ');
		mapCurrent[x][y] = '*';
	}

}

void changeDirect(struct character *chacr, char key) {
	if (key == 'W' || key == 'w' || key == 72) (*chacr).direct = UP;
	if (key == 'S' || key == 's' || key == 80) (*chacr).direct = DOWN;
	if (key == 'a' || key == 'A' || key == 75) (*chacr).direct = LEFT;
	if (key == 'd' || key == 'D' || key == 77) (*chacr).direct = RIGHT;
}

void showBuffer() {
	changeColorr(2);
	int x, y = 10;
	if (mapChoice == 1) y = 5;
	for (int i = 0; i <= consoleHeight; ++i) {
		x = 50;
		if (mapChoice == 1) x = 37;
		for (int j = 0; j <= consoleWidth; ++j) {
			gotoxy(x, y);
			if (buffer[i][j] == -108) {
				// CHANGE COLOR PACMAN
				if (supperPacman) changeColorr(5);
					else changeColorr(7);
				putchar(buffer[i][j]);
				changeColorr(2);
			}
			else
				if (buffer[i][j] == -114) {
					// CHANGE COLOR GHOST 
					changeColorr(9 + rand() % 4);
					putchar(buffer[i][j]);
					changeColorr(2);
				}
				else
					if (buffer[i][j] == '*') {
						// CHANGE COLOR FOOD ( WHITE)
						changeColorr(15);
						putchar(buffer[i][j]);
						changeColorr(2);
					}
					else 
						if (buffer[i][j] == 'F') {
							changeColorr(9 + rand() % 4);
							putchar(buffer[i][j]);
							changeColorr(2);
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
		mapCurrent[(*chart).pos.x + 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == '*' && (*chart).direct == UP) {
		mapCurrent[(*chart).pos.x - 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == '*' && (*chart).direct == RIGHT) {
		mapCurrent[(*chart).pos.x][(*chart).pos.y + 1] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == '*' && (*chart).direct == LEFT) {
		mapCurrent[(*chart).pos.x][(*chart).pos.y - 1] = ' ';
		++ *score;
	}
}

void checkSupperfood(struct character *chart, int *score) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == 'F' && (*chart).direct == DOWN) {
		supperPacman = true;
		mapCurrent[(*chart).pos.x + 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == 'F' && (*chart).direct == UP) {
		supperPacman = true;
		mapCurrent[(*chart).pos.x - 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == 'F' && (*chart).direct == RIGHT) {
		supperPacman = true;
		mapCurrent[(*chart).pos.x][(*chart).pos.y + 1] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == 'F' && (*chart).direct == LEFT) {
		supperPacman = true;
		mapCurrent[(*chart).pos.x][(*chart).pos.y - 1] = ' ';
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

void modeEasy(int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(50);
	if (custom) setting(speedCus);
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
			lose();
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
			win();
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
		gotoXY( 115, 1);
		if (score != preScore) {
			changeColorr(rand() % 16);
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
				if (xFood >= 0) mapCurrent[xFood][yFood] = ' ';
				while (true) {
					xFood = rand() % consoleHeight;
					yFood = rand() % consoleWidth;
					if (buffer[xFood][yFood] == ' ') break;
				}
				mapCurrent[xFood][yFood] = 'F';
			}
		}
		
		++timeGame;
		timeGame %= 100;
		Sleep(speed);
	}
}

void modeHard(int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(50);
	if (custom) setting(speedCus);
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
			lose();
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
			win();
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
		gotoXY( 115, 1);
		if (score != preScore) {
			changeColorr(rand() % 16);
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
				if (xFood >= 0) mapCurrent[xFood][yFood] = ' ';
				while (true) {
					xFood = rand() % consoleHeight;
					yFood = rand() % consoleWidth;
					if (buffer[xFood][yFood] == ' ') break;
				}
				mapCurrent[xFood][yFood] = 'F';
			}
		}
		
		++timeGame;
		timeGame %= 100;
		Sleep(speed);
	}
}

void modeFaker(int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(0);
	if (custom) setting(speedCus);
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
			lose();
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
			win();
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
		gotoXY( 115, 1);
		if (score != preScore) {
			changeColorr(rand() % 16);
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
				if (xFood >= 0) mapCurrent[xFood][yFood] = ' ';
				while (true) {
					xFood = rand() % consoleHeight;
					yFood = rand() % consoleWidth;
					if (buffer[xFood][yFood] == ' ') break;
				}
				mapCurrent[xFood][yFood] = 'F';
			}
		}
		
		++timeGame;
		timeGame %= 100;
		Sleep(speed);
	}
}

void modeTwoplayers(int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(0);
	if (custom) setting(speedCus);
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
			changeColorr(4);
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
		gotoXY( 115, 1);
		if (scorePacman != prescorePacman) {
			changeColorr(rand() % 16);
			prescorePacman = scorePacman;
		}
		printf("Pacman score : %d\n\n", scorePacman * 10);
		
		gotoXY( 115, 4);
		changeColorr(2);
		if (scoreGhost != prescoreGhost) {
			changeColorr(rand() % 16);
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

void modeReverse(int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(0);
	if (custom) setting(speedCus);
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
			lose();
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
			win();
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
		gotoXY( 115, 1);
		if (score != preScore) {
			changeColorr(rand() % 16);
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

void set_color(const char *color) {//doi mau chu
    char cmd[512];
    sprintf(cmd, "echo|set /p=\"\e[%sm\"", color);
    system(cmd);
}

void changeColor(int color){//doi mau cua phan tu sau no ( doi mau nen)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoXY(int x, int y){
  static HANDLE h = NULL;  
  if(!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = { x, y };  
  SetConsoleCursorPosition(h,c);
}

void resizeConsole(int width, int height){
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void delay(int num){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= num);
}
///***INTRO***////
void intro(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	set_color("01;33");
	int x = 5, y = 10;
	gotoXY(x,y);    printf("PPPPPPPPPPPPPPPPP        AAA                  CCCCCCCCCCCCCMMMMMMMM               MMMMMMMM               AAA               NNNNNNNN        NNNNNNNN"); delay(500);
	gotoXY(x,y + 1);printf("P::::::::::::::::P      A:::A              CCC::::::::::::CM:::::::M             M:::::::M              A:::A              N:::::::N       N::::::N");	delay(100);
	gotoXY(x,y + 2);printf("P::::::PPPPPP:::::P    A:::::A           CC:::::::::::::::CM::::::::M           M::::::::M             A:::::A             N::::::::N      N::::::N");	delay(100);
	gotoXY(x,y + 3);printf("PP:::::P     P:::::P  A:::::::A         C:::::CCCCCCCC::::CM:::::::::M         M:::::::::M            A:::::::A            N:::::::::N     N::::::N");	delay(100);
  	gotoXY(x,y + 4);printf("  P::::P     P:::::P A:::::::::A       C:::::C       CCCCCCM::::::::::M       M::::::::::M           A:::::::::A           N::::::::::N    N::::::N"); 	delay(100);
  	gotoXY(x,y + 5);printf("  P::::P     P:::::PA:::::A:::::A     C:::::C              M:::::::::::M     M:::::::::::M          A:::::A:::::A          N:::::::::::N   N::::::N");	delay(100);
  	gotoXY(x,y + 6);printf("  P::::PPPPPP:::::PA:::::A A:::::A    C:::::C              M:::::::M::::M   M::::M:::::::M         A:::::A A:::::A         N:::::::N::::N  N::::::N");	delay(100);
  	gotoXY(x,y + 7);printf("  P:::::::::::::PPA:::::A   A:::::A   C:::::C              M::::::M M::::M M::::M M::::::M        A:::::A   A:::::A        N::::::N N::::N N::::::N");	delay(100);
  	gotoXY(x,y + 8);printf("  P::::PPPPPPPPP A:::::A     A:::::A  C:::::C              M::::::M  M::::M::::M  M::::::M       A:::::A     A:::::A       N::::::N  N::::N:::::::N");	delay(100);
  	gotoXY(x,y + 9);printf("  P::::P        A:::::AAAAAAAAA:::::A C:::::C              M::::::M   M:::::::M   M::::::M      A:::::AAAAAAAAA:::::A      N::::::N   N:::::::::::N");	delay(100);
  	gotoXY(x,y + 10);printf("  P::::P       A:::::::::::::::::::::AC:::::C              M::::::M    M:::::M    M::::::M     A:::::::::::::::::::::A     N::::::N    N::::::::::N");	delay(100);
  	gotoXY(x,y + 11);printf("  P::::P      A:::::AAAAAAAAAAAAA:::::AC:::::C       CCCCCCM::::::M     MMMMM     M::::::M    A:::::AAAAAAAAAAAAA:::::A    N::::::N     N:::::::::N");	delay(100);
	gotoXY(x,y + 12);printf("PP::::::PP   A:::::A             A:::::AC:::::CCCCCCCC::::CM::::::M               M::::::M   A:::::A             A:::::A   N::::::N      N::::::::N");	delay(100);
	gotoXY(x,y + 13);printf("P::::::::P  A:::::A               A:::::ACC:::::::::::::::CM::::::M               M::::::M  A:::::A               A:::::A  N::::::N       N:::::::N");	delay(100);
	gotoXY(x,y + 14);printf("P::::::::P A:::::A                 A:::::A CCC::::::::::::CM::::::M               M::::::M A:::::A                 A:::::A N::::::N        N::::::N");	delay(100);
	gotoXY(x,y + 15);printf("PPPPPPPPPPAAAAAAA                   AAAAAAA   CCCCCCCCCCCCCMMMMMMMM               MMMMMMMMAAAAAAA                   AAAAAAANNNNNNNN         NNNNNNN");	delay(100);
	changeColor(15);
	timeS=clock();
	do{
		timeE=clock();
	}while(timeE-timeS <= 700);
}


void printMenuBanner(){
	set_color("01;31");
	int x = 47, y = 5;
	gotoXY(x,y);    printf(" _______________________________________________________________ ");
	gotoXY(x,y + 1);printf("|                                                               |");
	gotoXY(x,y + 2);printf("|     $$$$$$     $$$$$ $$$$$$$$$$$ $$$$$$   $$$ $$$      $$$    |");
	gotoXY(x,y + 3);printf("|     $$$ $$$   $$$$$$ $$$$$$$$$$$ $$$ $$$  $$$ $$$      $$$    |");
	gotoXY(x,y + 4);printf("|     $$$  $$$ $$$ $$$ $$$         $$$  $$$ $$$ $$$      $$$    |");
	gotoXY(x,y + 5);printf("|     $$$   $$$$$  $$$ $$$$$$$$$$  $$$   $$$$$$ $$$      $$$    |");
	gotoXY(x,y + 6);printf("|     $$$    $$$   $$$ $$$         $$$    $$$$$ $$$      $$$    |");
	gotoXY(x,y + 7);printf("|     $$$          $$$ $$$$$$$$$$$ $$$     $$$$ $$$      $$$    |");
	gotoXY(x,y + 8);printf("|     $$$          $$$ $$$$$$$$$$$ $$$      $$$  $$$$$$$$$$     |");
	gotoXY(x,y + 9);printf("|_______________________________________________________________|");
	changeColor(15);
}
//***khung***
void frame(){
	int x = 20, y = 0, j;
	changeColor(12);
	gotoXY(x,y);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 187);
	for(j = 1; j < 35; j++){
		gotoXY(x,y + j);printf("%c", 186); 
		gotoXY(x+ 119,y + j);printf("%c", 186);
		}
	gotoXY(x,y+j);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 188);
	changeColor(15);
}

void framePlay(){
	system("cls");
	int x = 20, y = 0, j;
	changeColor(12);
	gotoXY(x,y);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,203, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 187);
	for(j = 1; j < 35; j++){
		gotoXY(x,y + j);printf("%c", 186); 
		if( j >= 1 && j < 35){
			gotoXY(x + 90,y + j);printf("%c", 186);
		}
		gotoXY(x+ 119,y + j);printf("%c", 186);
	}
	gotoXY(x,y+j);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,202 ,205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 188);
	gotoXY( 115, 6);
	printf("Help: Use WASD to MOVE");
	gotoXY( 115, 7);
	printf("EAT * to win");
	
	if (modeGame == 1) {
		
		// EASY MODE 
		modeEasy(0, 0);

	}
	else
		if (modeGame == 2) {
			
			// HARD MODE
			modeHard(0, 0);
			
		}
			else 
				if (modeGame == 3) {
				
				// FAKER MODE 
				modeFaker(0, 0);
				
				}
					else  
						if (modeGame == 4) {
							//TWO PLAYERS MODE
							modeTwoplayers(0, 0);
						}
							else {
								// REVERSE MODE
								modeReverse(0, 0);
							}
}


//***EXIT***///
void exitGame(int choiceMenu){
	int enterMenu, y = 18;
	frame();
	gotoXY(65, y);
	changeColor(252);
	printf("Do you want to exit game?");
	changeColor(15);
	if(choiceMenu == 1){
		changeColor(71);
	}
	gotoXY(70, y + 1);
	printf("|     YES      |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoXY(70, y + 2);
	printf("|     NO       |");
	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 72 && choiceMenu > 1){
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 2){
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	system("cls");
	        	int i; 
				float j = 0;
	        	if(choiceMenu == 1){
	        		for(i = 0; i < 90; i ++){
						gotoXY(i, j);
			        	printf("Goodbye!!!");
						j += 0.25;
			        	system("cls");	
					}
			     	exit(0);
				}
				if(choiceMenu == 2){
					choiceMenu = 0;
					frame();
		        	printMenuBanner();
					printMenu(choiceMenu);
				}	
			}	
			exitGame(choiceMenu);
		}
	}
	while (enterMenu != 13);
}
//setting
void settingUI(int choiceMenu){
	int enterMenu = 0, y = 18;
	frame();	

	gotoXY(67, y);
	printf("|     SPEED      |");	
	if(choiceMenu == 1)
		changeColor(30);
	gotoXY(69,y+1);
	printf("[ 1 |");
	changeColor(15);	
	printf(" 2 | 3 ]");
	if(choiceMenu == 2){
		changeColor(71);
		gotoXY(69, y + 1);
		printf("[ 1 | 2 |");
	}
	changeColor(15);
	if(choiceMenu == 3){
		changeColor(95);	
		gotoXY(69, y + 1);
		printf("[ 1 |");
		printf(" 2 ");
		printf("| 3 ]");
	}
	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 75 && choiceMenu > 1){
	            choiceMenu --;
	        }
	        if(c == 77 && choiceMenu < 3){
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	system("cls");
	        	frame();
	        	printMenuBanner();
	        	printMenu(enterMenu);
	        	enterMenu = 13;
			}
			settingUI(choiceMenu);
		}
	}
	while (enterMenu != 13);
}

///*MODE*///
void mode(int choiceMenu){
	int enterMenu = 0, y = 18;
	frame();	
	if(choiceMenu == 1)
		changeColor(30);
	gotoXY(67, y);
	printf("|     *NORMAL*      |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoXY(67, y + 1);
	printf("|     **HARD**      |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(95);	
	gotoXY(67, y + 2);
	printf("|    ***FAKER***    |");
	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 72 && choiceMenu > 1){
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 3){
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	modeGame = choiceMenu;
	        	system("cls");
	        	frame();
	        	printMenuBanner();
	        	printMenu(enterMenu);
	        	enterMenu = 13;
			}
			mode(choiceMenu);
		}
	}
	while (enterMenu != 13);
}

void play(int choiceMenu){
	int enterMenu = 0, y = 18;
	frame();
	if(choiceMenu == 1)
		changeColor(30);
	gotoXY(67, y);
	printf("|       START       |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoXY(67, y + 1);
	printf("|        MAP        |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(95);	
	gotoXY(67, y + 2);
	printf("|       SKIN        |");
	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 72 && choiceMenu > 1){
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 3){
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	system("cls");
	        	frame();
	        	if(choiceMenu == 1){	
	        		framePlay();
				}
				if(choiceMenu == 2){
					system("cls");
					int num = 1;
					map(num);
				}
				if(choiceMenu == 3){
					system("cls");
					int num = 1;
					skin(num);
				}
			}
			if(c == 27){
				system("cls");
				frame();
	        	printMenuBanner();
	        	printMenu(enterMenu);
	        	enterMenu = 13;
			}
			play(choiceMenu);
		}
	}
	while (enterMenu != 13);
}

void map(int choiceMenu){
	int enterMenu = 0, y = 18;
	frame();	
	gotoXY(70, y);
	printf("|     MAP     |");	
	if(choiceMenu == 1){
		gotoXY(75, y + 1);
		printf("< 1 >");
	}
	if(choiceMenu == 2){
		gotoXY(75, y + 1);
		printf("< 2 >");
	}
	if(choiceMenu == 3){
		gotoXY(75, y + 1);
		printf("< 3 >");
	}

	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 75 && choiceMenu > 1){
	            choiceMenu --;
	        }
	        if(c == 77 && choiceMenu < 3){
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	system("cls");
	        	mapChoice = choiceMenu - 1;
	        	frame();
	        	play(1);
	        	enterMenu = 13;
			}
			map(choiceMenu);
		}
	}
	while (enterMenu != 13);
}


void skin(int choiceMenu){
	int enterMenu = 0, y = 18;
	frame();	
	gotoXY(70, y);
	printf("|     SKIN     |");	
	if(choiceMenu == 1){
		gotoXY(75, y + 1);
		printf("< 1 >");
	}
	if(choiceMenu == 2){
		gotoXY(75, y + 1);
		printf("< 2 >");
	}
	if(choiceMenu == 3){
		gotoXY(75, y + 1);
		printf("< 3 >");
	}

	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 75 && choiceMenu > 1){
	            choiceMenu --;
	        }
	        if(c == 77 && choiceMenu < 3){
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	system("cls");
	        	frame();
	        	play(1);
	        	enterMenu = 13;
			}
			if(c == 27){
				system("cls");
	        	frame();
	        	play(1);
			}
			skin(choiceMenu);
		}
	}
	while (enterMenu != 13);
}


//****MENU***///
int printMenu(int choiceMenu){	
	int enterMenu = 0, y = 18 ;
	if(choiceMenu == 1)
		changeColor(30);
	gotoXY(70, y);
	printf("|     PLAY     |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(30);
	gotoXY(70, y + 1);
	printf("|     MODE     |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(30);
	gotoXY(70, y + 2);
	printf("|    SETTING   |");
	changeColor(15);
	if(choiceMenu == 4)
		changeColor(30);
	gotoXY(70, y + 3);
	printf("|    RANKING   |");
	changeColor(15);
	Nocursortype();
	char c;
	do{
		if(kbhit()){
		    c = getch();
		    if(c == 72 && choiceMenu > 1){
		        choiceMenu --;
		    }
		    if(c == 80 && choiceMenu < 4){
		        choiceMenu ++;
		    }
		    if(c == 13){
		    	enterMenu = 13;
				if(choiceMenu == 1){
					system("cls");
					int num = 1;
					play(num);
				}
				if(choiceMenu == 2){
					system("cls");
					int num = 0;
					mode(num);
				}
				if(choiceMenu == 3){
					system("cls");
					int num = 0;
					settingUI(num);
				}
				if(choiceMenu == 4){
					system("cls");
					int num = 0;
//					setting(num);
				}
			}
			if(c == 27){
				system("cls");
				int num = 0;
				exitGame(num);
			}
			printMenu(choiceMenu);
		}
	}
	while (enterMenu != 13);
	return 0;
}


void lose(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	set_color("01;91");
	int x = 30, y = 10;
	gotoXY(x,y);    printf(" ______________________________________________________________________________");      delay(500);
	gotoXY(x,y + 1);printf("|                                                                              |");     delay(100);
	gotoXY(x,y + 2);printf("| YYY      YYY   OOOOO    UUU    UUU     LLL       OOOOO      SSSSS   EEEEEEEE | ");    delay(100);
	gotoXY(x,y + 3);printf("|   YYY   YYY  OOO   OOO  UUU    UUU     LLL     OOO   OOO  SSS   SSS EEE      | ");	delay(100);
	gotoXY(x,y + 4);printf("|    YYY YYY  OOO     OOO UUU    UUU     LLL    OOO     OOO  SSS      EEE      | ");	delay(100);
	gotoXY(x,y + 5);printf("|      YYY    OOO     OOO UUU    UUU     LLL    OOO     OOO     SSS   EEEEEEEE | ");	delay(100);
	gotoXY(x,y + 6);printf("|      YYY    OOO     OOO UUU    UUU     LLL    OOO     OOO      SSS  EEE      | ");	delay(100);
	gotoXY(x,y + 7);printf("|      YYY     OOO   OOO   UUU  UUU      LLL     OOO   OOO   SSS  SSS EEE      | ");	delay(100);
	gotoXY(x,y + 8);printf("|      YYY       OOOOO      UUUUUU       LLLLLLLL  OOOOO       SSSS   EEEEEEEE | ");	delay(100);
	gotoXY(x,y + 9);printf("|______________________________________________________________________________| ");	delay(100);
	gotoXY(x + 33,y + 13);printf("PLAY AGAIN!!");
	changeColor(15);
}

void win(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	set_color("01;36");
	int x = 30, y = 10;
	gotoXY(x,y);    printf(" ______________________________________________________________________________");  delay(500);
	gotoXY(x,y + 1);printf("|                                                                              |"); delay(100);
	gotoXY(x,y + 2);printf("| YYY      YYY   OOOOO    UUU    UUU   www  www    www  www iiiii NNNN    NNNN |"); delay(100);
	gotoXY(x,y + 3);printf("|   YYY   YYY  OOO   OOO  UUU    UUU    www  ww    ww  www   iii  NNNNNN  NNN  |");	delay(100);
	gotoXY(x,y + 4);printf("|    YYY YYY  OOO     OOO UUU    UUU     www  ww  ww  www    iii  NNN NNN NNN  |");	delay(100);
	gotoXY(x,y + 5);printf("|      YYY    OOO     OOO UUU    UUU      www  wwww  www     iii  NNN  NNNNNN  |");	delay(100);
	gotoXY(x,y + 6);printf("|      YYY    OOO     OOO UUU    UUU       www wwww www      iii  NNN   NNNNN  |");	delay(100);
	gotoXY(x,y + 7);printf("|      YYY     OOO   OOO   UUU  UUU         wwww  wwww       iii  NNN    NNNN  |");	delay(100);
	gotoXY(x,y + 8);printf("|      YYY       OOOOO      UUUUUU           www  www       iiiii NNN     NNN  |");	delay(100);
	gotoXY(x,y + 9);printf("|______________________________________________________________________________|");	delay(100);
	gotoXY(x + 33,y + 13);printf("PLAY AGAIN!!");
	changeColor(15);
}


int main(){
	int choiceMenu = 0;
	resizeConsole(1280, 720);
	Nocursortype();
	intro();
	Nocursortype();
	system("cls");// làm sach màn hinh
	printMenuBanner();
	frame();
	printMenu(choiceMenu);
	system("cls");	
	win();
	return 0;
}
