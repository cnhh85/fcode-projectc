#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>


void Nocursortype(){//ancontronhapnhay
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
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
	changeColor(15);
	gotoXY( 115, 1);
	printf("score");
	gotoXY( 115, 3);
	printf("Help: Use WASD to MOVE");
	gotoXY( 115, 4);
	printf("EAT * to win");
}



char mapez[105][105] = { //33x19
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
};

char maphard[105][105] = { //58x23
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
	};



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
void setting(int choiceMenu){
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
			setting(choiceMenu);
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
	            choiceMenu --;
	        }
	        if(c == 80 && choiceMenu < 5){
	            choiceMenu ++;
	        }
	        if(c == 13){
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
	changeColor(15);;
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
					setting(num);
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

void pacmanWin(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	set_color("01;36");
	int x = 30, y = 10;
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
	gotoXY(x + 33,y + 13);printf("PLAY AGAIN!!");
	changeColor(15);
}

void ghostWin(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 219);
	set_color("01;36");
	int x = 30, y = 10;
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




