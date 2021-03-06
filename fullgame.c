#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>

//AUDIO
void playTheme() {
	PlaySound(TEXT(".\\audio\\theme.wav"), NULL, SND_ASYNC  | SND_LOOP); //PACMAN THEME
}
void playEating() {
	PlaySound(TEXT(".\\audio\\eating.wav"), NULL, SND_ASYNC); //EATING FOOD
}
void playDead() {
	PlaySound(TEXT(".\\audio\\dead.wav"), NULL, SND_SYNC  | SND_LOOP); //PACMAN DIES
}
void playMenuScroll() {
	PlaySound(TEXT(".\\audio\\menuscroll.wav"), NULL, SND_ASYNC ); //SCROLL MENU
}
void playMenuSellect() {
	PlaySound(TEXT(".\\audio\\menuselect.wav"), NULL, SND_ASYNC); //MENU SELECT
}
void playMenuTheme() {
	PlaySound(TEXT(".\\audio\\menutheme.wav"), NULL, SND_ASYNC); //MENU MUSIC
}
void playWin() {
	PlaySound(TEXT(".\\audio\\audio_gamewin.wav"), NULL, SND_ASYNC  | SND_LOOP); //WIN GAME
}
void playEatingSupperfood() {
	PlaySound(TEXT(".\\audio\\audio_powerup.wav"), NULL, SND_ASYNC); // EAT SUPPER FOOD
}

void pauseSound() {
	PlaySound(NULL, NULL, 0);
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
int nFood = 300, nPlayereasy = 0, nPlayerhard = 0, nPlayerfaker = 0, nPlayerreverse = 0;
int nGhost, choiceMode = 1;
bool supperPacman = false, cus = false;
int speed, mapChoice = 0, modeGame = 1, pacmanSkin = 0;
FILE *fEasy, *fHard, *fReverse, *fFaker;
int score = 0, preScore = 0, timeGame = 0, timeSupperpacman = 0;
bool loseGame = false, winGame = false, mark[105][105];
char buffer[105][105];
char **playersEasy = NULL, **playersHard = NULL, **playersFaker = NULL, **playersReverse = NULL;
int scoresEasy[500], lenEasy[500];
int scoresHard[500], lenHard[500];
int scoresFaker[500], lenFaker[500];
int scoresReverse[500], lenReverse[500];

// MAP

char mapCurrent[105][105];

struct point mapLen[5] = {
	{35, 18},

	{33, 19},
	{58, 23},
};


maps[5][105][105] = {
		{
	//  1     2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35
		{219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219},  //1
		{219, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', 178, ' ', ' ', ' ', 178, 178, 178, 178, ' ', ' ', ' ', ' ', 219},  //2
		{219, ' ', 178, 178, 178, 178, 178, 178, 178, 178, 178, ' ', 178, ' ', 178, 178, 178, 178, 178, ' ', 178, ' ', ' ', ' ', 178, ' ', ' ', 178, 178, ' ', ' ', 178, 178, 178, 219},  //3
		{219, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, ' ', 178, ' ', 178, 178, 178, 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 219},  //4	
		{219, 178, 178, 178, 178, ' ', 178, 178, 178, 178, 178, ' ', 178, 178, 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', ' ', ' ', ' ', 178, 178, ' ', 178, 178, 178, ' ', 219},  //5	
		{219, ' ', ' ', ' ', 178, ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', 178, 178, ' ', 178, 178, ' ', ' ', ' ', ' ', ' ', 219},  //6
		{219, ' ', 178, ' ', 178, ' ', 178, 178, 178, 178, 178, ' ', 178, ' ', 178, ' ', 178, ' ', ' ', ' ', 178, ' ', 178, ' ', 178, 178, ' ', 178, 178, ' ', 178, ' ', 178, ' ', 219},  //7
		{219, ' ', 178, 178, 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', 178, ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, 178, ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', 219},  //8
		{219, ' ', ' ', ' ', ' ', ' ', 178, 178, 178, 178, 178, ' ', 178, ' ', 178, ' ', 178, 178, 178, 178, 178, ' ', 178, 178, 178, 178, 178, 178, 178, ' ', 178, 178, 178, ' ', 219},  //9
		{219, 178, 178, ' ', 178, ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, 178, ' ', ' ', 219},  //10
		{219, ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, 178, 178, ' ', 178, 178, 178, ' ', 178, ' ', 178, 178, 178, ' ', 178, 178, 178, ' ', 178, ' ', 178, 178, 178, 178, ' ', 178, 219},  //11
		{219, ' ', 178, 178, 178, 178, 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', 178, ' ', ' ', ' ', 219},  //12
		{219, ' ', 178, ' ', ' ', ' ', 178, 178, 178, 178, 178, ' ', 178, ' ', 178, 178, 178, 178, 178, 178, 178, ' ', 178, ' ', 178, ' ', 178, 178, 178, ' ', ' ', ' ', 178, ' ', 219},  //13
		{219, ' ', 178, ' ', 178, ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', ' ', ' ', 178, 178, 178, ' ', 219},  //14
		{219, ' ', ' ', ' ', 178, ' ', 178, ' ', ' ', ' ', ' ', ' ', 178, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 178, ' ', 178, ' ', 178, ' ', 178, ' ', ' ', ' ', 178, 178, 178, ' ', 219},  //14
		{219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219},  //18
		//18 x 35
	},
	
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


void sortInc(int *score, char **players, int *len, int n) {
	char tmp1[500];
	for (int i = 0; i < n - 1; ++i)
		for (int j = i + 1; j < n; ++j)
			if (score[i] > score[j]) {
				int tmp = score[i];
				score[i] = score[j];
				score[j] = tmp;
				tmp = len[i];
				len[i] = len[j];
				len[j] = tmp;
				for (int k = 0; k < max(len[i],len[j]); ++k) tmp1[k] = players[i][k];
				for (int k = 0; k < max(len[i],len[j]); ++k) players[i][k] = players[j][k];
				for (int k = 0; k < max(len[i],len[j]); ++k) players[j][k] = tmp1[k]; 
			}
}

void sortDec(int *score, char **players, int *len, int n) {
	char tmp1[500];
	for (int i = 0; i < n - 1; ++i)
		for (int j = i + 1; j < n; ++j)
			if (score[i] < score[j]) {
				int tmp = score[i];
				score[i] = score[j];
				score[j] = tmp;
				tmp = len[i];
				len[i] = len[j];
				len[j] = tmp;
				for (int k = 0; k < max(len[i],len[j]); ++k) tmp1[k] = players[i][k];
				for (int k = 0; k < max(len[i],len[j]); ++k) players[i][k] = players[j][k];
				for (int k = 0; k < max(len[i],len[j]); ++k) players[j][k] = tmp1[k]; 
			}
}

void copyData() {
	fEasy = fopen(".\\ranking\\rankingEasy.txt","r");
	char c;
	nPlayereasy = 0;
	scoresEasy[0] = 0;
	lenEasy[0] = 0;
	bool ok = false;
	while(!feof(fEasy) ) {
		c = fgetc(fEasy);
		if (ok && c != '\n') scoresEasy[nPlayereasy] = scoresEasy[nPlayereasy] * 10 + c - 48;
		if (c == ' ') ok = true;
		if (ok == false && c != '\n') playersEasy[nPlayereasy][lenEasy[nPlayereasy]++] = c; 
		if (c == '\n') {
			++ nPlayereasy;
			ok = false;
		}
	}	
	fclose(fEasy);
	
	fHard = fopen(".\\ranking\\rankingHard.txt","r");
	nPlayerhard = 0;
	scoresHard[0] = 0;
	lenHard[0] = 0;
	ok = false;
	while(!feof(fHard) ) {
		c = fgetc(fHard);
		if (ok && c != '\n') scoresHard[nPlayerhard] = scoresHard[nPlayerhard] * 10 + c - 48;
		if (c == ' ') ok = true;
		if (ok == false && c != '\n') playersHard[nPlayerhard][lenHard[nPlayerhard]++] = c; 
		if (c == '\n') {
			++ nPlayerhard;
			ok = false;
		}
	}	
	fclose(fHard);
	
	fFaker = fopen(".\\ranking\\rankingFaker.txt","r");
	nPlayerfaker = 0;
	scoresFaker[0] = 0;
	lenFaker[0] = 0;
	ok = false;
	while(!feof(fFaker) ) {
		c = fgetc(fFaker);
		if (ok && c != '\n') scoresFaker[nPlayerfaker] = scoresFaker[nPlayerfaker] * 10 + c - 48;
		if (c == ' ') ok = true;
		if (ok == false && c != '\n') playersFaker[nPlayerfaker][lenFaker[nPlayerfaker]++] = c; 
		if (c == '\n') {
			++ nPlayerfaker;
			ok = false;
		}
	}	
	fclose(fFaker);
	
	fReverse = fopen(".\\ranking\\rankingReverse.txt","r");
	nPlayerreverse = 0;
	scoresReverse[0] = 0;
	lenReverse[0] = 0;
	ok = false;
	while(!feof(fReverse) ) {
		c = fgetc(fReverse);
		if (ok && c != '\n') scoresReverse[nPlayerreverse] = scoresReverse[nPlayerreverse] * 10 + c - 48;
		if (c == ' ') ok = true;
		if (ok == false && c != '\n') playersReverse[nPlayerreverse][lenReverse[nPlayerreverse]++] = c; 
		if (c == '\n') {
			++ nPlayerreverse;
			ok = false;
		}
	}	
	fclose(fReverse);
}

void setting(int speedTmp) {
	speed = speedTmp;
}

void initMap(int mapIndex) {
	consoleHeight = mapLen[mapIndex].y;
	consoleWidth = mapLen[mapIndex].x;
	for (int i = 0; i <= consoleHeight; ++i)
		for (int j = 0; j <= consoleWidth; ++j)
			mapCurrent[i][j] = maps[mapIndex][i][j];
}

void removeMap() {

	for (int i = 0; i < 100; ++i)
		for (int j = 0; j < 100; ++j)

			buffer[i][j] = ' ';
}

void initBuffer() {
	for (int i = 0; i < consoleHeight; ++i)
		for (int j = 0; j < consoleWidth; ++j)
			buffer[i][j] = mapCurrent[i][j];
}

void initCharacter(struct character *pacman, struct character *ghost) {
	// INIT PACMAN--------------------------------------------------

	if (pacmanSkin == 0) (*pacman).shape = -108;
	if (pacmanSkin == 1) (*pacman).shape = 11;
	if (pacmanSkin == 2) (*pacman).shape = 12;
	if (pacmanSkin == 3) (*pacman).shape = 15;
	(*pacman).competitorShape = -114;
	(*pacman).pos.x = 1;
	(*pacman).pos.y = 1;
	(*pacman).direct = STOP;	
	
	// INIT GHOST----------------------------------------------------
	
	for (int i = 0; i < nGhost; ++i) {
		(*(ghost + i)).shape = -114;
		if (pacmanSkin == 0) (*(ghost + i)).competitorShape = -108;
		if (pacmanSkin == 1) (*(ghost + i)).competitorShape = 11;
		if (pacmanSkin == 2) (*(ghost + i)).competitorShape = 12;
		if (pacmanSkin == 3) (*(ghost + i)).competitorShape = 15;
		(*(ghost + i)).pos.x = consoleHeight - 4;
		(*(ghost + i)).pos.y = consoleWidth - 6;
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
	if (key == 'W' || key == 'w' || key == 'I' || key == 'i') (*chacr).direct = UP;
	if (key == 'S' || key == 's' || key == 'k' || key == 'K') (*chacr).direct = DOWN;
	if (key == 'a' || key == 'A' || key == 'j' || key == 'J') (*chacr).direct = LEFT;
	if (key == 'd' || key == 'D' || key == 'l' || key == 'L') (*chacr).direct = RIGHT;
}

void showBuffer() {
	changeColorr(2);
	int x, y = 10;
	if (mapChoice == 2) y = 5;
	for (int i = 0; i <= consoleHeight; ++i) {
		x = 50;
		if (mapChoice == 2) x = 37;
		for (int j = 0; j <= consoleWidth; ++j) {
			gotoxy(x, y);
			if (buffer[i][j] == -108 || buffer[i][j] == 11 || buffer[i][j] == 12 || buffer[i][j] == 15) {
				// CHANGE COLOR PACMAN
				if (supperPacman) {
					if (timeSupperpacman >= 70) {
						if (timeSupperpacman % 2 == 0) changeColorr(11);
						else changeColorr(0);
					}
					else changeColorr(11);
				}
				else changeColorr(6);
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
		if ((*chart).shape != -114 && (move[(int) (*chart).direct].x != 0 ||  move[(int) (*chart).direct].y != 0)) playEating();
		mapCurrent[(*chart).pos.x + 1][(*chart).pos.y] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == '*' && (*chart).direct == UP) {
		if ((*chart).shape != -114 && (move[(int) (*chart).direct].x != 0 ||  move[(int) (*chart).direct].y != 0)) playEating();
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == '*' && (*chart).direct == RIGHT) {
		if ((*chart).shape != -114 && (move[(int) (*chart).direct].x != 0 ||  move[(int) (*chart).direct].y != 0)) playEating();
		mapCurrent[(*chart).pos.x][(*chart).pos.y + 1] = ' ';
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == '*' && (*chart).direct == LEFT) {
		if ((*chart).shape != -114 && (move[(int) (*chart).direct].x != 0 ||  move[(int) (*chart).direct].y != 0)) playEating();
		mapCurrent[(*chart).pos.x][(*chart).pos.y - 1] = ' ';
		++ *score;
	}
}

void checkSupperfood(struct character *chart, int *score) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == 'F' && (*chart).direct == DOWN) {
		playEatingSupperfood();
		supperPacman = true;
		mapCurrent[(*chart).pos.x + 1][(*chart).pos.y] = ' ';
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == 'F' && (*chart).direct == UP) {
		playEatingSupperfood();
		supperPacman = true;
		mapCurrent[(*chart).pos.x - 1][(*chart).pos.y] = ' ';
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == 'F' && (*chart).direct == RIGHT) {
		playEatingSupperfood();
		supperPacman = true;
		mapCurrent[(*chart).pos.x][(*chart).pos.y + 1] = ' ';
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == 'F' && (*chart).direct == LEFT) {
		playEatingSupperfood();
		supperPacman = true;
		mapCurrent[(*chart).pos.x][(*chart).pos.y - 1] = ' ';
	}
}

void checkTouchCompetitor(struct character *chart) {
	if (buffer[(*chart).pos.x][(*chart).pos.y] == (*chart).competitorShape) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 4;
				(*chart).pos.y = consoleWidth - 6;
			}
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == (*chart).competitorShape && (*chart).direct == DOWN) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 4;
				(*chart).pos.y = consoleWidth - 6;
			}
			return;
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == (*chart).competitorShape && (*chart).direct == UP) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 4;
				(*chart).pos.y = consoleWidth - 6;
			}
			return;
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == (*chart).competitorShape && (*chart).direct == RIGHT) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 4;
				(*chart).pos.y = consoleWidth - 6;
			}
			return;
		}
		loseGame = true;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == (*chart).competitorShape && (*chart).direct == LEFT) {
		if (supperPacman == true) {
			if ((*chart).shape == -114) {
				(*chart).pos.x = consoleHeight - 4;
				(*chart).pos.y = consoleWidth - 6;
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
				(*(ghosts + i)).pos.x = consoleHeight - 4;
				(*(ghosts + i)).pos.y = consoleWidth - 6;
				return;
			}
	}
	if (buffer[(*pacman).pos.x + 1][(*pacman).pos.y] == -114 && (*pacman).direct == DOWN) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x + 1 && (*(ghosts + i)).pos.y == (*pacman).pos.y) {
					(*(ghosts + i)).pos.x = consoleHeight - 4;
					(*(ghosts + i)).pos.y = consoleWidth - 6;
				return;
			}
	}
	if (buffer[(*pacman).pos.x - 1][(*pacman).pos.y] == -114 && (*pacman).direct == UP) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x - 1 && (*(ghosts + i)).pos.y == (*pacman).pos.y) {
					(*(ghosts + i)).pos.x = consoleHeight - 4;
					(*(ghosts + i)).pos.y = consoleWidth - 6;
				return;
			}
	}
	if (buffer[(*pacman).pos.x][(*pacman).pos.y + 1] == -114 && (*pacman).direct == RIGHT) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x && (*(ghosts + i)).pos.y == (*pacman).pos.y + 1) {
					(*(ghosts + i)).pos.x = consoleHeight - 4;
					(*(ghosts + i)).pos.y = consoleWidth - 6;
				return;
			}
	}
	if (buffer[(*pacman).pos.x][(*pacman).pos.y - 1] == -114 && (*pacman).direct == LEFT) {
		for (int i = 0; i < nGhost; ++i)
			if ((*(ghosts + i)).pos.x == (*pacman).pos.x && (*(ghosts + i)).pos.y == (*pacman).pos.y - 1) {
				(*(ghosts + i)).pos.x = consoleHeight - 4;
				(*(ghosts + i)).pos.y = consoleWidth - 6;
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
	removeMap();
	nGhost = 3;
	score = 0;
	preScore = 0;
	timeGame = 0;
	if (mapChoice == 0) nFood = 260;
	if (mapChoice == 1) nFood = 300;
	if (mapChoice == 2) nFood = 500;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	int xFood = -1, yFood = -1;
	timeSupperpacman = 0;
	supperPacman = false; 
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
		if (loseGame == true) {	
			//system("cls");
			removeMap();
			showBuffer();
			lose(score, 1);
			system("cls");
			break;	
		}
		
		if (winGame == true) {
			//system("cls");
			removeMap();
			showBuffer();
			win(score, 1);
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
		if (supperPacman == true) {
			if (timeSupperpacman % 10 == 0) {
				gotoXY( 115, 9);
				changeColorr(12);
				printf("TIME SUPPER FOOD : ");
				changeColorr(7);
				printf("%d", 8 - timeSupperpacman / 10);
			}
		}
		else {
				gotoXY( 115, 9);
				printf("                     ");
		}
		
		// CACULATE SCORE-------------------------
		gotoXY( 115, 1);
		if (score != preScore) {
			changeColorr(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 10);
		
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
			if (supperPacman == true) {
				++timeSupperpacman;
			}
			if (timeSupperpacman == 90) {
				supperPacman = false; 
				timeSupperpacman = 0;
			}
			if (timeSupperpacman == false && timeGame % 50 == 0) {
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
	removeMap();
	nGhost = 1;
	score = 0;
	preScore = 0;
	timeGame = 0;
	if (mapChoice == 0) nFood = 240;
	if (mapChoice == 1) nFood = 300;
	if (mapChoice == 2) nFood = 500;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	int xFood = -1, yFood;
	timeSupperpacman = 0;
	supperPacman = false; 
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
		
		if (loseGame == true) {	
			//system("cls");
			removeMap();
			showBuffer();
			lose(score, 2);
			system("cls");
			break;	
		}
		
		if (winGame == true) {
			//system("cls");
			removeMap();
			showBuffer();
			win(score, 2);
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
		if (supperPacman == true) {
			if (timeSupperpacman % 10 == 0) {
				gotoXY( 115, 9);
				changeColorr(12);
				printf("TIME SUPPER FOOD : ");
				changeColorr(7);
				printf("%d", 8 - timeSupperpacman / 10);
			}
		}
		else {
				gotoXY( 115, 9);
				printf("                     ");
		}
		
		// CACULATE SCORE-------------------------
		gotoXY( 115, 1);
		if (score != preScore) {
			changeColorr(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 10);
		
		
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
	removeMap();
	nGhost = 2;
	score = 0;
	timeGame = 0;
	preScore = 0;
	if (mapChoice == 0) nFood = 240;
	if (mapChoice == 1) nFood = 300;
	if (mapChoice == 2) nFood = 500;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap(mapChoice);
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	int xFood = -1, yFood;
	timeSupperpacman = 0;
	supperPacman = false; 
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
	
		if (loseGame == true) {	
			//system("cls");
			removeMap();
			showBuffer();
			lose(score, 3);
			system("cls");
			break;	
		}
		
		if (winGame == true) {
			//system("cls");
			removeMap();
			showBuffer();
			win(score, 3);
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
		
		if (supperPacman == true) {
			if (timeSupperpacman % 10 == 0) {
				gotoXY( 115, 9);
				changeColorr(12);
				printf("TIME SUPPER FOOD : ");
				changeColorr(7);
				printf("%d", 8 - timeSupperpacman / 10);
			}
		}
		else {
				gotoXY( 115, 9);
				printf("                     ");
		}
		
		gotoXY( 115, 1);
		if (score != preScore) {
			changeColorr(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 10);
		
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
	removeMap();
	nGhost = 1;
	if (mapChoice == 0) nFood = 260;
	if (mapChoice == 1) nFood = 300;
	if (mapChoice == 2) nFood = 500;
	timeGame = 0;
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
		if (scorePacman + scoreGhost == nFood) winGame = true;
		if (winGame == true) {
			//system("cls");
			gotoxy(55,8);
			changeColorr(4);
			if (scorePacman > scoreGhost) {
				removeMap();
				showBuffer();
				pacmanWin();
			}
			else {
				removeMap();
				showBuffer();
				ghostWin();
			}
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
		printf("Ghost score : %d", scoreGhost * 10);
		
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
		timeGame %= 100;
		Sleep(speed);
	}
}

void modeReverse(int speedCus,bool custom) {
	// INIT--------------------------------------
	srand(time(NULL));
	setting(0);
	if (custom) setting(speedCus);
	removeMap();
	nGhost = 1;
	score = 0;
	preScore = 0;
	timeGame = 0;
	loseGame = winGame = false;
	if (mapChoice == 0) nFood = 240;
	if (mapChoice == 1) nFood = 300;
	if (mapChoice == 2) nFood = 500;
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
			//system("cls");
			removeMap();
			showBuffer();
			lose(score, 4);
			system("cls");
			break;	
		}
		
		if (loseGame == true) {
			//system("cls");
			removeMap();
			showBuffer();
			win(score, 4);
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
		printf("Score : %d", score * 10);
		
		
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
						if (score <= 290)  
							if (buffer[x][y] == '*') break;
						if (buffer[x][y] == ' ') break;
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
		timeGame %= 100;
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
	playTheme();
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
	delay(2800);
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
	gotoXY( 113, 29);
	printf("Help:");
	changeColor(5);
	gotoXY( 113, 30);
	printf("Use WASD and JIKL to MOVE");
	gotoXY( 113, 31);
	printf("EAT * to win");
	gotoXY( 113, 32);
	printf("EAT F to get the POWER");
	changeColor(12);
	
	if (modeGame == 1) {
		
		// EASY MODE 
		modeEasy(speed, cus);

	}
	else
		if (modeGame == 2) {
			
			// HARD MODE
			modeHard(speed, cus);
			
		}
			else 
				if (modeGame == 3) {
				
				// FAKER MODE 
				modeFaker(speed, cus);
				
				}
					else  
						if (modeGame == 4) {
							//TWO PLAYERS MODE
							modeTwoplayers(speed, cus);
						}
							else {
								// REVERSE MODE
								modeReverse(speed, cus);
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
	        	playMenuScroll();
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 2){
	        	playMenuScroll();
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	playMenuSellect();
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

void settingUI(int choiceMenu){
	int enterMenu = 0, y = 18, x = 53;
	frame();	
	set_color("01;91");
	gotoXY(x,y-7);    printf("  _________       __    __  .__               "); 
	gotoXY(x,y-6);    printf(" /   _____/ _____/  |__/  |_|__| ____    ____  ");
	gotoXY(x,y-5);    printf(" \\_____  \\_/ __ \\   __\\   __\\  |/    \\  / ___\\ ");
	gotoXY(x,y-4);    printf(" /        \\  ___/|  |  |  | |  |   |  \\/ /_/  >");
	gotoXY(x,y-3);    printf("/_______  /\\___  >__|  |__| |__|___|  /\\___  / ");
	gotoXY(x,y-2);    printf("        \\/     \\/                   \\//_____/  ");
	changeColor(15);
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
	        	playMenuScroll();
	            choiceMenu --;
	        }
	        if(c == 77 && choiceMenu < 3){
	        	playMenuScroll();
	            choiceMenu ++;
	        }
	        if(c == 13 || c == 27){
	        	playMenuSellect();
	        	if (c == 13) {
	        		speed = (3 - choiceMenu) * 100;
	        		cus = true;
	        	}
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
	int enterMenu = 0, y = 18,x=55;
	frame();	
	set_color("01;91");
	gotoXY(x,y-7);    printf("  __    __     ______     _____     ______  ");  
	gotoXY(x,y-6);    printf(" /\\ *-./  \\   /\\  __ \\   /\\  __-.  /\\  ___\\   ");
	gotoXY(x,y-5);    printf(" \\ \\ \\-./\\ \\  \\ \\ \\/\\ \\  \\ \\ \\/\\ \\ \\ \\  __\\   ");
 	gotoXY(x,y-4);    printf("  \\ \\_\\ \\ \\_\\  \\ \\_____\\  \\ \\____-  \\ \\_____\\ ");
  	gotoXY(x,y-3);    printf("   \\/_/  \\/_/   \\/_____/   \\/____/   \\/_____/ ");           
	changeColor(15);                                 	
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
	if(choiceMenu == 4)
		changeColor(95);	
	gotoXY(67, y + 3);
	printf("|  *PACMANandGHOST* |");
	changeColor(15);
	if(choiceMenu == 5)
		changeColor(95);	
	gotoXY(67, y + 4);
	printf("|  ***playGHOST***  |");
	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 72 && choiceMenu > 1){
	        	playMenuScroll();
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 5){
	        	playMenuScroll();
	            choiceMenu ++;
	        }
	        if(c == 13 || c == 27){
	        	playMenuSellect();
	        	if (c == 13) modeGame = choiceMenu;
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
	int enterMenu = 0, y = 18,x = 63;
	frame();	
	set_color("01;91");
	gotoXY(x,y-7);    printf("__________.__                ");
	gotoXY(x,y-6);    printf("\\______   \\  | _____  ___.__.");
 	gotoXY(x,y-5);    printf(" |     ___/  | \\__  \\<   |  |");
 	gotoXY(x,y-4);    printf(" |    |   |  |__/ __ \\\\___  |");
 	gotoXY(x,y-3);    printf(" |____|   |____(____  / ____|");
	gotoXY(x,y-2);    printf("                    \\/\\/   ");
	changeColor(15);
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
	        	playMenuScroll();
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 3){
	        	playMenuScroll();
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	playMenuSellect();
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
			if(c == 27) {
				playMenuSellect();
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
	int enterMenu = 0, y = 18,x = 63;
	frame();	
	set_color("01;91");
	gotoXY(x,y-7);    printf("   _____      _____ __________ ");
	gotoXY(x,y-6);    printf("  /     \\    /  _  \\\\______   \\");
	gotoXY(x,y-5);    printf(" /  \\ /  \\  /  /_\\  \\|     ___/");
	gotoXY(x,y-4);    printf("/    Y    \\/    |    \\    |    ");
	gotoXY(x,y-3);    printf("\\____|__  /\\____|__  /____|    ");
	gotoXY(x,y-2);    printf("        \\/         \\/         ");
	changeColor(15);
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
	        	playMenuScroll();
	            choiceMenu --;
	        }
	        if(c == 77 && choiceMenu < 3){
	        	playMenuScroll();
	            choiceMenu ++;
	        }
	        if(c == 13 || c == 27){
	        	playMenuSellect();
	        	if (c == 13) {
	        		mapChoice = choiceMenu - 1;
	        	}
	        	system("cls");
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
	int enterMenu = 0, y = 18,x = 63;
	frame();	
	set_color("01;91");
	gotoXY(x,y-7);    printf("  ___________   .__        ");
	gotoXY(x,y-6);    printf(" /   _____/  | _|__| ____  ");
	gotoXY(x,y-5);    printf(" \\_____  \\|  |/ /  |/    \\ ");
	gotoXY(x,y-4);    printf(" /        \\    <|  |   |  \\");
	gotoXY(x,y-3);    printf("/_______  /__|_ \\__|___|  /");
	gotoXY(x,y-2);    printf("        \\/     \\/       \\/");
	changeColor(15);
	
	gotoXY(70, y);
	printf("|     SKIN     |");	
	if(choiceMenu == 1){
		gotoXY(75, y + 1);
		printf("< %c >", 11);
	}
	if(choiceMenu == 2){
		gotoXY(75, y + 1);
		printf("< %c >", 12);
	}
	if(choiceMenu == 3){
		gotoXY(75, y + 1);
		printf("< %c >", 15);
	}

	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 75 && choiceMenu > 1){
	        	playMenuScroll();
	            choiceMenu --;
	        }
	        if(c == 77 && choiceMenu < 3){
	        	playMenuScroll();
	            choiceMenu ++;
	        }
	        if(c == 13){
	        	playMenuSellect();
	        	system("cls");
	        	pacmanSkin = choiceMenu;
	        	frame();
	        	play(1);
	        	enterMenu = 13;
			}
			if(c == 27){
				playMenuSellect();
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
	if (choiceMenu == 0) playMenuTheme();
	pacmanSkin = 0;
	mapChoice = 0;
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
		        playMenuScroll();
		        
		    }
		    if(c == 80 && choiceMenu < 4){
		        choiceMenu ++;
		        playMenuScroll();
		    }
		    if(c == 13){
		    	playMenuSellect();
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
					ranking(num);
				}
			}
			if(c == 27){
				playMenuSellect();
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

void ranking(int choiceMenu) {
	int y= 18,x = 50, enterMenu = 0;
	frame();	
	set_color("01;91");
	gotoXY(x,y-7);    printf("    //   ) )                                                  ");
	gotoXY(x,y-6);    printf("   //___/ /   _       _     / __     ( )   _      __    ");
	gotoXY(x,y-5);    printf("  / _ (   //   ) ) //   ) ) //\\ \\     / / //   ) ) //   ) ) ");
	gotoXY(x,y-4);    printf(" //   | |  //   / / //   / / //  \\ \\   / / //   / / ((___/ /  ");
	gotoXY(x,y-3);    printf("//    | | ((__( ( //   / / //    \\ \\ / / //   / /   //_    ");
	changeColor(15);
	if(choiceMenu == 1)
		changeColor(30);
	gotoXY(67, y);
	printf("|     NORMAL      |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoXY(67, y + 1);
	printf("|     *HARD*      |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(95);	
	gotoXY(67, y + 2);
	printf("|    **FAKER**    |");
	changeColor(15);
	if(choiceMenu == 4)
		changeColor(95);	
	gotoXY(67, y + 3);
	printf("|  **playGHOST**  |");
	changeColor(15);
	do{
		Nocursortype();
		char c;
		if(kbhit()){
	        c = getch();
	        if(c == 72 && choiceMenu > 1){
	        	playMenuScroll();
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 4){
	        	playMenuScroll();
	            choiceMenu ++;
	        }
	        if(c == 13 ){
	        	playMenuSellect();
	        	if (choiceMenu == 0) continue;
        		system("cls");
        		showRanking(choiceMenu);
			}
			if (c == 27) {
				playMenuSellect();
				system("cls");
				frame();
				printMenuBanner();
				printMenu(0);
				return;
			}
			ranking(choiceMenu);
		}
	}
	while (enterMenu != 13);
}

void showRanking(int choice) {
	int y= 18,x = 50, enterMenu = 0;
	frame();	
	set_color("01;91");
	gotoXY(x,y-7);    printf("    //   ) )                                                  ");
	gotoXY(x,y-6);    printf("   //___/ /   _       _     / __     ( )   _      __    ");
	gotoXY(x,y-5);    printf("  / _ (   //   ) ) //   ) ) //\\ \\     / / //   ) ) //   ) ) ");
	gotoXY(x,y-4);    printf(" //   | |  //   / / //   / / //  \\ \\   / / //   / / ((___/ /  ");
	gotoXY(x,y-3);    printf("//    | | ((__( ( //   / / //    \\ \\ / / //   / /   //_    ");
	changeColor(15);
	y = 18, x = 66;
	gotoXY(x, y);
	printf("Top     Users          Scores");
	y += 3;
	if (choice == 1) {
		sortDec(&scoresEasy, playersEasy, &lenEasy, nPlayereasy);
		for (int i = 0; i < min(5, nPlayereasy); ++i) {
			gotoXY(x, y);
			printf("%d        ", i + 1);
			for (int j = 0; j < lenEasy[i]; ++j) printf("%c", playersEasy[i][j]);
			for (int j = 0; j < 15 - lenEasy[i]; ++j) printf(" ");
			printf(" %d", scoresEasy[i]);
			++y;
		}
		while (true) {
			if (kbhit()) {
				char c = getch();
				if (c == 27) {
					system("cls");
					ranking(choice);
		        	return;
				}
			}
		}
	}
	if (choice == 2) {
		sortDec(&scoresHard, playersHard, &lenHard, nPlayerhard);
		for (int i = 0; i < min(5, nPlayerhard); ++i) {
			gotoXY(x, y);
			printf("%d       ", i + 1);
			for (int j = 0; j < lenHard[i]; ++j) printf("%c", playersHard[i][j]);
			for (int j = 0; j < 15 - lenHard[i]; ++j) printf(" ");
			printf(" %d", scoresHard[i]);
			++y;
		}
		while (true) {
			if (kbhit()) {
				char c = getch();
				if (c == 27) {
					system("cls");
					ranking(choice);
		        	return;
				}
			}
		}
	}
	if (choice == 3) {
		sortDec(&scoresFaker, playersFaker, &lenFaker, nPlayerfaker);
		for (int i = 0; i < min(5, nPlayerfaker); ++i) {
			gotoXY(x, y);
			printf("%d       ", i + 1);
			for (int j = 0; j < lenFaker[i]; ++j) printf("%c", playersFaker[i][j]);
			for (int j = 0; j < 15 - lenFaker[i]; ++j) printf(" ");
			printf(" %d", scoresFaker[i]);
			++y;
		}
		while (true) {
			if (kbhit()) {
				char c = getch();
				if (c == 27) {
					system("cls");
					ranking(choice);
		        	return;
				}
			}
		}
	}
	if (choice == 4) {
		sortInc(&scoresReverse, playersReverse, &lenReverse, nPlayerreverse);
		for (int i = 0; i < min(5, nPlayerreverse); ++i) {
			gotoXY(x, y);
			printf("%d       ", i + 1);
			for (int j = 0; j < lenReverse[i]; ++j) printf("%c", playersReverse[i][j]);
			for (int j = 0; j < 15 - lenReverse[i]; ++j) printf(" ");
			printf(" %d", scoresReverse[i]);
			++y;
		}
		while (true) {
			if (kbhit()) {
				char c = getch();
				if (c == 27) {
					system("cls");
					ranking(choice);
		        	return;
				}
			}
		}
	}
}

void lose(int score, int mode){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	playDead();
	set_color("01;91");
	removeMap();
	int x = 25, y = 10;
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
	gotoXY(x + 33,y + 13);
	printf("ENTER YOUR NAME : ");
	
	char c, player[100];
	int cnt = 0;
	if (mode == 1) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayereasy; ++i) {
			bool flag = false;
			for (int j = 0; j < lenEasy[i]; ++j)
				if (player[j] != playersEasy[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresEasy[i] = max(score, scoresEasy[i]);
				return;
			}
		}
		lenEasy[nPlayereasy] = cnt;
		scoresEasy[nPlayereasy] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersEasy[nPlayereasy][i] = player[i];
		}
		fEasy = fopen(".\\ranking\\rankingEasy.txt","w");
		++nPlayereasy;
		for (int i = 0; i < nPlayereasy; ++i) {
			for (int j = 0; j < lenEasy[i]; ++j) {
				fprintf(fEasy, "%c", playersEasy[i][j]);
			}
			fprintf(fEasy, " %d\n", scoresEasy[i]);
		}
		fclose(fEasy);
	}
	if (mode == 2) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayerhard; ++i) {
			bool flag = false;
			for (int j = 0; j < lenHard[i]; ++j)
				if (player[j] != playersHard[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresHard[i] = max(score, scoresHard[i]);
				return;
			}
		}
		lenHard[nPlayerhard] = cnt;
		scoresHard[nPlayerhard] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersHard[nPlayerhard][i] = player[i];
		}
		++nPlayerhard;
		fHard = fopen(".\\ranking\\rankingHard.txt","w");
		for (int i = 0; i < nPlayerhard; ++i) {
			for (int j = 0; j < lenHard[i]; ++j) {
				fprintf(fHard, "%c", playersHard[i][j]);
			}
			fprintf(fHard, " %d\n", scoresHard[i]);
		}
		fclose(fHard);
	}
	if (mode == 3) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayerfaker; ++i) {
			bool flag = false;
			for (int j = 0; j < lenFaker[i]; ++j)
				if (player[j] != playersFaker[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresFaker[i] = max(score, scoresFaker[i]);
				return;
			}
		}
		lenFaker[nPlayerfaker] = cnt;
		scoresFaker[nPlayerfaker] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersFaker[nPlayerfaker][i] = player[i];
		}
		fFaker = fopen(".\\ranking\\rankingFaker.txt","w");
		++nPlayerfaker;
		for (int i = 0; i < nPlayerfaker; ++i) {
			for (int j = 0; j < lenFaker[i]; ++j) {
				fprintf(fFaker, "%c", playersFaker[i][j]);
			}
			fprintf(fFaker, " %d\n", scoresFaker[i]);
		}
		fclose(fFaker);
	}
	if (mode == 4) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayerreverse; ++i) {
			bool flag = false;
			for (int j = 0; j < lenReverse[i]; ++j)
				if (player[j] != playersReverse[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresReverse[i] = min(score , scoresReverse[i]);
				return;
			}
		}
		lenReverse[nPlayerreverse] = cnt;
		scoresReverse[nPlayerreverse] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersReverse[nPlayerreverse][i] = player[i];
		}
		fReverse = fopen(".\\ranking\\rankingReverse.txt","w");
		++nPlayerreverse;
		for (int i = 0; i < nPlayerreverse; ++i) {
			for (int j = 0; j < lenReverse[i]; ++j) {
				fprintf(fReverse, "%c", playersReverse[i][j]);
			}
			fprintf(fReverse, " %d\n", scoresReverse[i]);
		}
		fclose(fReverse);
	}
	changeColor(15);
}

void win(int score, int mode){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	playWin();
	set_color("01;91");
	removeMap();
	int x = 25, y = 10;
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
	gotoXY(x + 33,y + 13);
	printf("ENTER YOUR NAME : ");
	
	char c, player[100];
	int cnt = 0;
	if (mode == 1) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayereasy; ++i) {
			bool flag = false;
			for (int j = 0; j < lenEasy[i]; ++j)
				if (player[j] != playersEasy[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresEasy[i] = max(score, scoresEasy[i]);
				return;
			}
		}
		lenEasy[nPlayereasy] = cnt;
		scoresEasy[nPlayereasy] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersEasy[nPlayereasy][i] = player[i];
		}
		fEasy = fopen(".\\ranking\\rankingEasy.txt","w");
		++nPlayereasy;
		for (int i = 0; i < nPlayereasy; ++i) {
			for (int j = 0; j < lenEasy[i]; ++j) {
				fprintf(fEasy, "%c", playersEasy[i][j]);
			}
			fprintf(fEasy, " %d\n", scoresEasy[i]);
		}
		fclose(fEasy);
	}
	if (mode == 2) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayerhard; ++i) {
			bool flag = false;
			for (int j = 0; j < lenHard[i]; ++j)
				if (player[j] != playersHard[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresHard[i] = max(score, scoresHard[i]);
				return;
			}
		}
		lenHard[nPlayerhard] = cnt;
		scoresHard[nPlayerhard] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersHard[nPlayerhard][i] = player[i];
		}
		fHard = fopen(".\\ranking\\rankingHard.txt","w");
		++nPlayerhard;
		for (int i = 0; i < nPlayerhard; ++i) {
			for (int j = 0; j < lenHard[i]; ++j) {
				fprintf(fHard, "%c", playersHard[i][j]);
			}
			fprintf(fHard, " %d\n", scoresHard[i]);
		}
		fclose(fHard);
	}
	if (mode == 3) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayerfaker; ++i) {
			bool flag = false;
			for (int j = 0; j < lenFaker[i]; ++j)
				if (player[j] != playersFaker[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresFaker[i] = max(score, scoresFaker[i]);
				return;
			}
		}
		lenFaker[nPlayerfaker] = cnt;
		scoresFaker[nPlayerfaker] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersFaker[nPlayerfaker][i] = player[i];
		}
		fFaker = fopen(".\\ranking\\rankingFaker.txt","w");
		++nPlayerfaker;
		for (int i = 0; i < nPlayerfaker; ++i) {
			for (int j = 0; j < lenFaker[i]; ++j) {
				fprintf(fFaker, "%c", playersFaker[i][j]);
			}
			fprintf(fFaker, " %d\n", scoresFaker[i]);
		}
		fclose(fFaker);
	}
	if (mode == 4) {
		while (true) {
			c = getchar();
			if (c == ' ') continue;
			if (c == '\n') break;
			player[cnt++] = c;
		}
		for (int i = 0; i < nPlayerreverse; ++i) {
			bool flag = false;
			for (int j = 0; j < lenReverse[i]; ++j)
				if (player[j] != playersReverse[i][j]) {
					flag = true;
					break;
				}
			if (flag == false) {
				scoresReverse[i] = min(score, scoresReverse[i]);
				return;
			}
		}
		lenReverse[nPlayerreverse] = cnt;
		scoresReverse[nPlayerreverse] = score * 10;
		for (int i = 0; i < cnt; ++i) {
			playersReverse[nPlayerreverse][i] = player[i];
		}
		fReverse = fopen(".\\ranking\\rankingReverse.txt","w");
		++nPlayerreverse;
		for (int i = 0; i < nPlayerreverse; ++i) {
			for (int j = 0; j < lenReverse[i]; ++j) {
				fprintf(fReverse, "%c", playersReverse[i][j]);
			}
			fprintf(fReverse, " %d\n", scoresReverse[i]);
		}
		fclose(fReverse);
	}
	changeColor(15);
}

void pacmanWin(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	playWin();
	set_color("01;36");
	removeMap();
	int x = 52, y = 10;
	gotoXY(x,y);    printf("          )         )       ) " );      
	gotoXY(x,y +1);    printf("    )   ( /(   (    (     ( /(   (  ");
	gotoXY(x,y +2);    printf(" /(/(   )(_))  )\\   )\\  ' )(_))  )\\ ) "); 
	gotoXY(x,y +3);    printf("((_)_\\ ((_)_  ((_)_((_)) ((_)_  _(_/(  ");
	gotoXY(x,y +4);    printf("| '_ \\)/ _` |/ _|| '  \\()/ _` || ' \\)) ");
	gotoXY(x,y +5);    printf("| .__/ \\__,_|\\__||_|_|_| \\__,_||_||_|  ");
	gotoXY(x,y +6);    printf("|(| (   (                              ");
	gotoXY(x,y +7);    printf("             )\\))(  )\\   (                         ");
	gotoXY(x,y +8);    printf("            ((_)()\\((_)  )\\ )                      ");
	gotoXY(x,y +9);    printf("          _(()(_)(_) _(_/(                      ");
	gotoXY(x,y +10);    printf("         \\ \V  \V /| || ' \\))                     ");
	gotoXY(x,y +11);    printf("          \\_/\\_/ |_||_||_| ");
	gotoXY(x + 10,y + 16);printf("PLAY AGAIN!!");
	changeColor(15);
}

void ghostWin(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	playWin();
	set_color("01;36");
	removeMap();
	int x = 52, y = 10;n
	gotoXY(x,y);    printf(" (          )              ) "); 
	gotoXY(x,y +1);    printf(" )\\ )    ( /(           ( /(  ");
	gotoXY(x,y +2);    printf("(()/(    )\\())  (   (   )\\()) ");
	gotoXY(x,y +3);    printf(" /(_))_ ((_)\\   )\\  )\\ (_))/  ");
	gotoXY(x,y +4);    printf("(_)) __|| |(_) ((_)((_)| |_   ");
	gotoXY(x,y +5);    printf("  | (_ || ' \\ / _ \\(_-<|  _|  ");
	gotoXY(x,y +6);    printf(" (\\ (__||_||_|\\___//__/ \\__|  ");
	gotoXY(x,y +7);    printf("    )\\))(   ' (                  ");
	gotoXY(x,y +8);    printf("   ((_)()\\ )  )\\   (             ");
	gotoXY(x,y +9);    printf("   _(())\\_)()((_)  )\\ )          ");
	gotoXY(x,y +10);    printf("   \\ \\((_)/ / (_) _(_/(          ");
	gotoXY(x,y +11);    printf("    \\ \\/\\/ /  | || ' \\))         ");
	gotoXY(x,y +12);    printf("     \\_/\\_/   |_||_||_|   	");	
	gotoXY(x + 10,y + 16);printf("PLAY AGAIN!!");
	changeColor(15);
}


int main(){
	SetConsoleTitle("Pacman");
	int choiceMenu = 0;
	playersEasy = (char **)malloc(100 * sizeof(int *));
	for (int i = 0; i < 100; ++i) {
		playersEasy[i] = (char *)malloc(500 * sizeof(int));
	}
	playersHard = (char **)malloc(100 * sizeof(int *));
	for (int i = 0; i < 100; ++i) {
		playersHard[i] = (char *)malloc(500 * sizeof(int));
	}
	playersFaker = (char **)malloc(100 * sizeof(int *));
	for (int i = 0; i < 100; ++i) {
		playersFaker[i] = (char *)malloc(500 * sizeof(int));
	}
	playersReverse = (char **)malloc(100 * sizeof(int *));
	for (int i = 0; i < 100; ++i) {
		playersReverse[i] = (char *)malloc(500 * sizeof(int));
	}
	copyData();
	resizeConsole(1280, 720);
	Nocursortype();
	intro();
	Nocursortype();
	system("cls");// làm sach màn hinh
	printMenuBanner();
	frame();
	printMenu(choiceMenu);
	system("cls");	
	return 0;
}
