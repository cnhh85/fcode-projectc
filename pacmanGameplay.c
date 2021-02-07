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
	int color;
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

const int consoleWidth = 58;
const int consoleHeight = 23;
int speed = 100;
int defaultColor = 2;
int nFood = 300;
int nGhost, choiceMode = 1;

int score = 0, preScore = 0, timeGame = 0;
bool loseGame = false, winGame = false, mark[105][105];
char buffer[105][105];

// MAP

char map[105][105];

char map1[105][105] = {
		{201, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205 ,187},
		{186,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0, 178, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0, 186},
		{186,   0,   0, 210,   0,   0, 174, 205, 205, 205, 205, 205, 205, 175,   0, 178,   0,   0, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0, 178,   0, 178, 178,   0, 178,   0, 178, 178,   0, 178,   0,   0, 186},
		{186,   0,   0, 186,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0,   0,   0, 178, 178, 178,   0,   0, 178, 178, 178, 178, 178,   0,   0, 178, 178, 178,   0,   0, 178, 178, 178,   0, 178,   0, 178, 178,   0, 178,   0,   0, 178,   0, 178,   0, 178, 186},
		{204, 205, 205, 202, 205, 187,   0, 201, 205, 205, 205, 205, 205, 187,   0,   0,   0, 178,   0,   0, 178,   0,   0, 178, 178, 178,   0,   0, 178, 178, 178, 178, 178,   0,   0, 178, 178, 178,   0,   0, 178, 178, 178,   0,   0,   0, 178,   0,   0, 178, 178,   0, 178,   0, 178,   0,   0, 186},
		{186,   0,   0,   0,   0, 186,   0, 186,   0,   0,   0,   0,   0, 186,   0, 178,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0, 186},
		{186,   0,   0,   0,   0, 186,   0, 186,   0,   0,   0,   0,   0, 186,   0, 178,   0,   0, 178,   0,   0, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0, 178,   0,   0, 178,   0, 178,   0, 178,   0, 178,   0, 178,   0,   0,   0,   0, 186},
		{204, 205, 205, 205, 205, 188,   0, 200, 205, 205, 205, 205, 205, 188,   0,   0,   0,   0, 178,   0, 178,   0,   0,   0,   0,   0, 201, 205, 205, 176,   0, 176, 205, 205, 187,   0,   0,   0,   0,   0, 178,   0,   0,   0, 178,   0,   0, 178,   0,   0,   0,   0, 178,   0, 178, 178,   0, 186},
		{186,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0, 178, 178,   0,   0,   0,   0,   0, 178,   0,   0,   0, 178,   0, 178,   0, 186,   0,   0,   0,   0,   0,   0,   0, 186,   0, 178,   0,   0, 178,   0, 178,   0,   0, 178,   0, 178,   0,   0, 178,   0, 178, 178,   0, 178, 178,   0, 186},
		{186,   0, 178, 178,   0, 178, 178,   0,   0,   0, 178,   0,   0,   0, 178, 178,   0, 178, 178,   0,   0,   0, 178,   0,   0,   0, 186,   0,   0,   0,   0,   0,   0,   0, 186,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0, 178,   0, 178,   0,   0, 178,   0, 178,   0,   0, 186},
		{186,   0, 178, 178,   0, 178, 178, 178,   0, 178, 178,   0, 178,   0, 178, 178,   0, 178,   0, 178,   0,   0,   0,   0, 178,   0, 186,   0,   0,   0,   0,   0,   0,   0, 186,   0,   0,   0, 178,   0,   0, 178,   0,   0, 178,   0, 178,   0,   0,   0,   0, 178,   0,   0, 178,   0,   0, 186},
		{186,   0,   0,   0,   0,   0, 178, 178,   0, 178, 178,   0, 178,   0,   0,   0,   0, 178,   0, 178,   0,   0, 178,   0,   0,   0, 186,   0,   0,   0,   0,   0,   0,   0, 186,   0, 178,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0, 178,   0,   0, 178,   0, 178,   0, 178, 178, 178, 186},
		{186,   0, 178,   0, 178,   0, 178, 178,   0,   0, 178,   0, 178,   0, 178, 178,   0, 178,   0,   0,   0,   0,   0,   0, 178,   0, 200, 205, 205, 205, 205, 205, 205, 205, 188,   0,   0,   0, 178, 178, 178,   0, 178,   0, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 186},
		{186,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0, 178,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0, 178, 178,   0, 178,   0, 178,   0, 178, 178, 178, 186},
		{186,   0,   0, 178,   0, 178, 178,   0, 178, 178, 178,   0, 178,   0, 178, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0, 178, 178, 178, 178,   0, 178, 178, 178,   0,   0,   0,   0, 178,   0, 178,   0, 178, 178,   0, 178,   0,   0,   0, 178,   0,   0,   0,   0, 178,   0,   0, 186},
		{186, 178,   0, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0, 178,   0, 178, 178,   0, 178,   0, 178, 178,   0,   0,   0, 178, 178,   0, 178,   0, 178,   0, 178, 178,   0, 186},
		{186,   0,   0, 178,   0, 178,   0, 178,   0, 178,   0,   0, 178,   0,   0, 178,   0, 178,   0,   0, 178, 178,   0, 178,   0, 178, 178, 178,   0, 178,   0, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0, 178,   0,   0,   0, 178,   0,   0,   0,   0,   0, 186},
		{186,   0,   0, 178,   0, 178,   0, 178,   0, 178, 178,   0,   0, 178,   0,   0,   0,   0, 178,   0,   0, 178,   0,   0,   0, 178,   0, 178,   0, 178,   0, 178,   0, 178,   0,   0,   0, 178,   0, 178, 178,   0, 178, 178,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0,   0, 178, 178, 186},
		{186, 178,   0, 178,   0, 178,   0, 178,   0,   0, 178,   0,   0, 178,   0, 178,   0, 178,   0,   0, 178,   0,   0,   0,   0, 178,   0, 178,   0, 178,   0, 178,   0,   0,   0, 178,   0,   0,   0,   0,   0, 178, 178, 178,   0, 178,   0, 178, 178, 178, 178, 178,   0,   0,   0, 178, 178, 186},
		{186,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178,   0, 178,   0,   0, 178,   0, 178,   0, 178,   0, 178, 178, 178,   0,   0,   0, 178,   0, 178, 178, 178,   0,   0,   0, 178,   0,   0,   0, 178,   0,   0,   0, 178,   0,   0,   0, 186},
		{186,   0, 178,  178,   0, 178, 178, 178, 178, 178,  0, 178,  0,  178, 178, 178, 178, 178,   0, 178,   0,   0, 178,   0,   0, 178,   0, 178, 178,   0,   0,   0, 178, 178,   0, 178,   0,   0,   0,   0,   0,   0,   0, 178, 178, 178, 178, 178,   0, 178,   0, 178, 178, 178, 178, 178,   0, 186},
		{186,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 178, 178, 178, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 186},				
		{200, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205 ,188}
	};

void initMap() {
	for (int i = 0; i < consoleHeight; ++i)
		for (int j = 0; j < consoleWidth; ++j)
			map[i][j] = map1[i][j];
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
	(*pacman).color = 7;	
	
	// INIT GHOST----------------------------------------------------
	
	for (int i = 0; i < nGhost; ++i) {
		(*(ghost + i)).shape = -114;
		(*(ghost + i)).competitorShape = -108;
		(*(ghost + i)).pos.x = consoleHeight - 3;
		(*(ghost + i)).pos.y = consoleWidth - 2;
		(*(ghost + i)).direct = STOP;
		(*(ghost + i)).color = i + 3;
	}
	
	// INIT FOOD----------------------------------------------------
	
	int x, y;
	for (int i = 1; i <= nFood; ++i) {
		do {
			x = rand() % consoleHeight;
			y = rand() % consoleWidth;
		}
		while ((x == 1 && y == 1) || (x == consoleHeight && y == consoleWidth) || map[x][y] != 0);
		map[x][y] = '*';
	}

}

void changeDirect(struct character *chacr, char key) {
	if (key == 'W' || key == 'w' || key == 72) (*chacr).direct = UP;
	if (key == 'S' || key == 's' || key == 80) (*chacr).direct = DOWN;
	if (key == 'a' || key == 'A' || key == 75) (*chacr).direct = LEFT;
	if (key == 'd' || key == 'D' || key == 77) (*chacr).direct = RIGHT;
}

void showCharacter(struct character *chart) {
	changeColor((*chart).color);
	putchar((*chart).shape);
	changeColor(defaultColor);
}

void showBuffer() {
	changeColor(defaultColor);
	int x, y = 0;
	for (int i = 0; i <= consoleHeight; ++i) {
		x = 30;
		for (int j = 0; j <= consoleWidth; ++j) {
			gotoxy(x, y);
			if (buffer[i][j] != -108 && buffer[i][j] != -114)
				putchar(buffer[i][j]);
			++x;
		}
		++y;
		putchar('\n');
	}
}

void checkFood(struct character *chart, int *score) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == '*' && (*chart).direct == DOWN) {
		map[(*chart).pos.x + 1][(*chart).pos.y] = 0;
		buffer[(*chart).pos.x + 1][(*chart).pos.y] = 0;
		++ *score;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == '*' && (*chart).direct == UP) {
		map[(*chart).pos.x - 1][(*chart).pos.y] = 0;
		buffer[(*chart).pos.x + 1][(*chart).pos.y] = 0;
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == '*' && (*chart).direct == RIGHT) {
		map[(*chart).pos.x][(*chart).pos.y + 1] = 0;
		buffer[(*chart).pos.x + 1][(*chart).pos.y] = 0;
		++ *score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == '*' && (*chart).direct == LEFT) {
		map[(*chart).pos.x][(*chart).pos.y - 1] = 0;
		buffer[(*chart).pos.x + 1][(*chart).pos.y] = 0;
		++ *score;
	}
}

void checkTouchCompetitor(struct character *chart) {
	if (buffer[(*chart).pos.x][(*chart).pos.y] == (*chart).competitorShape) {
		loseGame = true;
	}
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == (*chart).competitorShape && (*chart).direct == DOWN) {
		loseGame = true;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == (*chart).competitorShape && (*chart).direct == UP) {
		loseGame = true;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == (*chart).competitorShape && (*chart).direct == RIGHT) {
		loseGame = true;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == (*chart).competitorShape && (*chart).direct == LEFT) {
		loseGame = true;
	}
}

void checkTouchWalls(struct character *chart) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] != 0 && buffer[(*chart).pos.x + 1][(*chart).pos.y] != '*' && (*chart).direct == DOWN) (*chart).direct = STOP;
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] != 0 && buffer[(*chart).pos.x - 1][(*chart).pos.y] != '*' && (*chart).direct == UP) (*chart).direct = STOP;
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] != 0 && buffer[(*chart).pos.x][(*chart).pos.y + 1] != '*' && (*chart).direct == RIGHT) (*chart).direct =  STOP;
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] != 0 && buffer[(*chart).pos.x][(*chart).pos.y - 1] != '*' && (*chart).direct == LEFT) (*chart).direct = STOP;
}

void solvePacman(struct character *pacman, int *score) {
	
	// CHECK EATING FOOD--------------------------------------------------------------------------
	
	checkFood(pacman, score);
	if (*score == nFood) winGame = true;
	
	// CHECK TOUCHING GHOST-----------------------------------------------------------------------
	
	checkTouchCompetitor(pacman);
	
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
	// DELETE OLD POSITION
	
	buffer[(*chart).pos.x][(*chart).pos.y] = map[(*chart).pos.x][(*chart).pos.y];
	gotoxy(30 + (*chart).pos.y , (*chart).pos.x);
	putchar(buffer[(*chart).pos.x][(*chart).pos.y]);
	
	// UPDATE POSITION
	
	(*chart).pos.x += move[(int) (*chart).direct].x;
	(*chart).pos.y += move[(int) (*chart).direct].y;
	
	// SHOW NEW POSITION
	
	buffer[(*chart).pos.x][(*chart).pos.y] = (*chart).shape;
	gotoxy(30 + (*chart).pos.y , (*chart).pos.x);
	showCharacter(chart);
}

void findPacman(struct character *ghost,struct character *pacman) {
	// INIT 
	
	if (buffer[(*pacman).pos.x][(*pacman).pos.y] == -114) return;
	
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
		if (buffer[tmp.x][tmp.y] == -108) {
			ok = true;
			break;
		}
		for (int i = 1; i <= 4; ++i) {
			tmp1.x = tmp.x + move[i].x;
			tmp1.y = tmp.y + move[i].y;
			if ((buffer[tmp1.x][tmp1.y] != 0 && buffer[tmp1.x][tmp1.y] != '*' && buffer[tmp1.x][tmp1.y] != -108 && buffer[tmp1.x][tmp1.y] != -114) || mark[tmp1.x][tmp1.y]) continue;
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

void modeEasy() {
	// INIT--------------------------------------
	srand(time(NULL));
	nGhost = 3;
	score = 0;
	preScore = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap();
	initCharacter(&pacman, &ghosts);
	initBuffer();
	showBuffer();
	buffer[pacman.pos.x][pacman.pos.y] = -108;
	gotoxy(31,1);
	showCharacter(&pacman);
	for (int i = 0; i < nGhost; ++i) {
		buffer[ghosts[i].pos.x][ghosts[i].pos.y] = -114;
		gotoxy(30 + ghosts[i].pos.y, ghosts[i].pos.x);
		showCharacter(&ghosts[i]);
	}
	Nocursortype();
	
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
				if (kbhit()) ++cnt;
				tmp = _getch();
				if (cnt == 2) break;
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
				if (kbhit()) ++cnt;
				tmp = _getch();
				if (cnt == 2) break;
			}
			system("cls");
			break;
		}
		
		// CACULATE SCORE-------------------------
		
		gotoxy(0,0);
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
		
		timeGame %= 10;
		if (timeGame == 0) {
			for (int i = 0; i < nGhost; ++i)
				changeDirect(&ghosts[i], moveGhost[rand() % 4]);
		}		
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman , &score);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		gotoxy(47,0);
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(speed);
	}
}

void modeHard() {
	// INIT--------------------------------------
	srand(time(NULL));
	nGhost = 1;
	score = 0;
	preScore = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap();
	initCharacter(&pacman, &ghosts);
	initBuffer();
	showBuffer();
	buffer[pacman.pos.x][pacman.pos.y] = -108;
	gotoxy(31,1);
	showCharacter(&pacman);
	for (int i = 0; i < nGhost; ++i) {
		buffer[ghosts[i].pos.x][ghosts[i].pos.y] = -114;
		gotoxy(30 + ghosts[i].pos.y, ghosts[i].pos.x);
		showCharacter(&ghosts[i]);
	}
	Nocursortype();
	
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
				if (kbhit()) ++cnt;
				tmp = _getch();
				if (cnt == 2) break;
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
				if (kbhit()) ++cnt;
				tmp = _getch();
				if (cnt == 2) break;
			}
			system("cls");
			break;
		}
		
		// CACULATE SCORE-------------------------
		
		gotoxy(0,0);
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
		
		solvePacman(&pacman , &score);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		gotoxy(47,0);
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		
		++timeGame;
		Sleep(speed);
	}
}


void modeFaker() {
	// INIT--------------------------------------
	srand(time(NULL));
	nGhost = 2;
	score = 0;
	preScore = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap();
	initCharacter(&pacman, &ghosts);
	initBuffer();
	showBuffer();
	buffer[pacman.pos.x][pacman.pos.y] = -108;
	gotoxy(31,1);
	showCharacter(&pacman);
	for (int i = 0; i < nGhost; ++i) {
		buffer[ghosts[i].pos.x][ghosts[i].pos.y] = -114;
		gotoxy(30 + ghosts[i].pos.y, ghosts[i].pos.x);
		showCharacter(&ghosts[i]);
	}
	Nocursortype();
	
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
				if (kbhit()) ++cnt;
				tmp = _getch();
				if (cnt == 2) break;
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
				if (kbhit()) ++cnt;
				tmp = _getch();
				if (cnt == 2) break;
			}
			system("cls");
			break;
		}
		
		// CACULATE SCORE-------------------------
		
		gotoxy(0,0);
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
		
		timeGame %= 5;
		if (timeGame == 0) {
			for (int i = 0; i < nGhost; ++i)
				changeDirect(&ghosts[i], moveGhost[rand() % 4]);
		}
		else
			for (int i = 0; i < nGhost; ++i)
				findPacman(&ghosts[i], &pacman);
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman , &score);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		gotoxy(47,0);
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(speed);
	}
}


void modeTwoplayers() {
	// INIT--------------------------------------
	srand(time(NULL));
	nGhost = 1;
	nFood = 300;
	int scorePacman = 0 , prescorePacman = 0;
	int scoreGhost = 0 , prescoreGhost = 0;
	loseGame = winGame = false;
	struct character pacman;
	struct character ghosts[nGhost];
	initMap();
	initCharacter(&pacman, &ghosts);
	initBuffer();
	showBuffer();
	buffer[pacman.pos.x][pacman.pos.y] = -108;
	gotoxy(31,1);
	showCharacter(&pacman);
	for (int i = 0; i < nGhost; ++i) {
		buffer[ghosts[i].pos.x][ghosts[i].pos.y] = -114;
		gotoxy(30 + ghosts[i].pos.y, ghosts[i].pos.x);
		showCharacter(&ghosts[i]);
	}
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
				if (kbhit()) ++cnt;
				tmp = _getch();
				if (cnt == 2) break;
			}
			system("cls");
			break;
		}
		
		// CACULATE SCORE-------------------------
		
		gotoxy(0,0);
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
		
		solvePacman(&pacman, &scorePacman);
		
		solvePacman(&ghosts[0], &scoreGhost);
		
		// MOVE-----------------------------------
		
		gotoxy(47,0);
		moveCharacter(&pacman);
		
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(speed);
	}
}


int main() {
	
	// MENU
	
	printf("MENU :\n");
	printf("1.EASY\n");
	printf("2.HARD\n");
	printf("3.FAKER\n");
	printf("4.TWO PLAYERS\n");
	
	char choice = _getch();
	
	if (choice == '1') {
		
		system("cls");
		// EASY MODE 
		modeEasy();

	}
	else
		if (choice == '2') {
			
			system("cls");
			// HARD MODE
			modeHard();
			
		}
			else 
				if (choice == '3') {
				
				system("cls");
				// FAKER MODE 
				modeFaker();
				
				}
					else  {
						system("cls");
						//TWO PLAYERS MODE
						modeTwoplayers();
						
					}
	
	return 0;
}
