#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

void gotoXY(int x, int y)
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

void set_color ( int code ) {
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

const int consoleWidth = 41;
const int consoleHeight = 15;
const int nFood = 300;
int nGhost;

int score = 0, preScore = 0, timeGame = 0;
bool loseGame = false, winGame = false, mark[105][105];
char buffer[105][105];

// MAP

char map[105][105] = {
		"----------------------------------------",
		"|  |                           |       |",
		"|  |                           |       |",
		"|      |                       |       |",
		"|      |               |               |",
		"|      |     |   |     |               |",
		"|      |     |   |     |               |",
		"|      -------   |     |               |",
		"|                |     |               |",
		"|                |     ---------       |",
		"|                |             |       |",
		"|-----------------                     |",
		"|                                      |",
		"----------------------------------------"
	};

void initBuffer() {
	for (int i = 0; i < consoleHeight; ++i)
		for (int j = 0; j < consoleWidth; ++j)
			buffer[i][j] = map[i][j];
			
}

void initCharacter(struct character *pacman, struct character *ghost) {
	// INIT PACMAN--------------------------------------------------

	(*pacman).shape = 'O';
	(*pacman).competitorShape = 'G';
	(*pacman).pos.x = 1;
	(*pacman).pos.y = 1;
	(*pacman).direct = STOP;	
	
	// INIT GHOST----------------------------------------------------
	
	for (int i = 0; i < nGhost; ++i) {
		(*(ghost + i)).shape = 'G';
		(*(ghost + i)).competitorShape = 'O';
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
	set_color(2);
	for (int i = 0; i <= 15; ++i) {
		for (int j = 0; j <= 39; ++j) {
			if (buffer[i][j] == 'O') {
				set_color(7);
				putchar(buffer[i][j]);
				set_color(2);
			}
			else
				if (buffer[i][j] == 'G') {
					set_color(9);
					putchar(buffer[i][j]);
					set_color(2);
				}
				else
					putchar(buffer[i][j]);
		}
		putchar('\n');
	}
}

void checkFood(struct character *chart) {
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] == '*' && (*chart).direct == DOWN) {
		map[(*chart).pos.x + 1][(*chart).pos.y] = ' ';
		++score;
	}
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] == '*' && (*chart).direct == UP) {
		map[(*chart).pos.x - 1][(*chart).pos.y] = ' ';
		++score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] == '*' && (*chart).direct == RIGHT) {
		map[(*chart).pos.x][(*chart).pos.y + 1] = ' ';
		++score;
	}
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] == '*' && (*chart).direct == LEFT) {
		map[(*chart).pos.x][(*chart).pos.y - 1] = ' ';
		++score;
	}
	if (score == nFood) winGame = true;
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
	if (buffer[(*chart).pos.x + 1][(*chart).pos.y] != ' ' && buffer[(*chart).pos.x + 1][(*chart).pos.y] != '*' && (*chart).direct == DOWN) (*chart).direct = STOP;
	if (buffer[(*chart).pos.x - 1][(*chart).pos.y] != ' ' && buffer[(*chart).pos.x - 1][(*chart).pos.y] != '*' && (*chart).direct == UP) (*chart).direct = STOP;
	if (buffer[(*chart).pos.x][(*chart).pos.y + 1] != ' ' && buffer[(*chart).pos.x][(*chart).pos.y + 1] != '*' && (*chart).direct == RIGHT) (*chart).direct =  STOP;
	if (buffer[(*chart).pos.x][(*chart).pos.y - 1] != ' ' && buffer[(*chart).pos.x][(*chart).pos.y - 1] != '*' && (*chart).direct == LEFT) (*chart).direct = STOP;
}

void solvePacman(struct character *pacman) {
	
	// CHECK EATING FOOD--------------------------------------------------------------------------
	
	checkFood(pacman);
	
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
		if (buffer[tmp.x][tmp.y] == 'O') {
			ok = true;
			break;
		}
		for (int i = 1; i <= 4; ++i) {
			tmp1.x = tmp.x + move[i].x;
			tmp1.y = tmp.y + move[i].y;
			if ((buffer[tmp1.x][tmp1.y] != ' ' && buffer[tmp1.x][tmp1.y] != '*' && buffer[tmp1.x][tmp1.y] != 'O') || mark[tmp1.x][tmp1.y]) continue;
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
	struct character pacman;
	struct character ghosts[nGhost];
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
	
		if (loseGame == true) {	
			system("cls");
			set_color(4);
			printf("YOU LOSE !!!!\n");
			break;	
		}
		
		if (winGame == true) {
			system("cls");
			set_color(4);
			printf("YOU WIN !!!!\n");
			break;
		}
		
		// CLEAR----------------------------------
		
		gotoXY(0,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		// CACULATE SCORE-------------------------
		
		if (score != preScore) {
			set_color(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 100);
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
			char key = _getch();
			changeDirect(&pacman, key);
		}
		
		timeGame %= 5;
		if (timeGame == 0)
			for (int i = 0; i < nGhost; ++i)
				changeDirect(&ghosts[i], moveGhost[rand() % 4]);
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(100);
	}
	system("pause");
}

void modeHard() {
	// INIT--------------------------------------
	srand(time(NULL));
	nGhost = 1;
	struct character pacman;
	struct character ghosts[nGhost];
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
	
		if (loseGame == true) {	
			system("cls");
			set_color(4);
			printf("YOU LOSE !!!!\n");
			break;	
		}
		
		if (winGame == true) {
			system("cls");
			set_color(4);
			printf("YOU WIN !!!!\n");
			break;
		}
		
		// CLEAR----------------------------------
		
		gotoXY(0,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		// CACULATE SCORE-------------------------
		
		if (score != preScore) {
			set_color(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 100);
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
			char key = _getch();
			changeDirect(&pacman, key);
		}
		
		for (int i = 0; i < nGhost; ++i)
				findPacman(&ghosts[i], &pacman);
		
		// SOLVE----------------------------------
		
		solvePacman(&pacman);
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(100);
	}
	system("pause");
}

void modeFaker() {
	// INIT--------------------------------------
	srand(time(NULL));
	nGhost = 2;
	struct character pacman;
	struct character ghosts[nGhost];
	initBuffer();
	initCharacter(&pacman, &ghosts);
	Nocursortype();
	
	// RUN---------------------------------------
	
	while (true) {
		
		// CHECK ENDGAME-------------------------
	
		if (loseGame == true) {	
			system("cls");
			set_color(4);
			printf("YOU LOSE !!!!\n");
			break;	
		}
		if (winGame == true) {
			system("cls");
			set_color(4);
			printf("YOU WIN !!!!\n");
			break;
		}
		
		// CLEAR----------------------------------
		
		gotoXY(0,0);
		initBuffer();
		
		// SHOW-----------------------------------
		
		buffer[pacman.pos.x][pacman.pos.y] = pacman.shape;
		
		for (int i = 0; i < nGhost; ++i)
			buffer[ghosts[i].pos.x][ghosts[i].pos.y] = ghosts[i].shape; 
			
		showBuffer();
		
		if (score != preScore) {
			set_color(rand() % 16);
			preScore = score;
		}
		printf("Score : %d", score * 100);
		
		// CONTROL--------------------------------
		
		if (kbhit()) {
			char key = _getch();
			changeDirect(&pacman, key);
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
		
		solvePacman(&pacman);
		
		for (int i = 0; i < nGhost; ++i)
			solveGhost(&ghosts[i]);
		
		// MOVE-----------------------------------
		
		moveCharacter(&pacman);
		
		for (int i = 0; i < nGhost; ++i)
			moveCharacter(&ghosts[i]);
		
		// SYSTEM---------------------------------
		++timeGame;
		Sleep(100);
	}
	system("pause");
}

int main() {
	
	// MENU
	
	printf("MENU :\n");
	printf("1.EASY\n");
	printf("2.HARD\n");
	printf("3.FAKER\n");
	
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
			else {
				
				system("cls");
				// FAKER MODE 
				modeFaker();
				
			}
	return 0;
}
