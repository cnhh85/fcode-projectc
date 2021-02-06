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

void gotoxy(int x, int y){
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
	}while(timeE-timeS <= 200);
	set_color("01;33");
	int x = 30, y = 10;
	gotoxy(x,y);    printf(" ______________________________________________________________________________________ ");     delay(500);
	gotoxy(x,y + 1);printf("|                                                                                      |");     delay(300);
	gotoxy(x,y + 2);printf("|      ########    #####     ########      ######     #######       ##### ######   ####| ");   	delay(300);
	gotoxy(x,y + 3);printf("|     ###     ##  ######   ###     ###     ### ###  ###  ###       ###### #######  ### | ");	delay(300);
	gotoxy(x,y + 4);printf("|    ###     ##  ### ###  ###       ###    ###   #####   ###      ### ### ###  ### ### | ");	delay(300);
	gotoxy(x,y + 5);printf("|   #########   ###  ###  ###              ###    ###    ###     ###  ### ###   ###### | ");	delay(300);
	gotoxy(x,y + 6);printf("|  ###         #########  ###       ###    ###     #     ###    ######### ###    ##### | ");	delay(300);
	gotoxy(x,y + 7);printf("| ###        ###     ###   ###     ###     ###           ###   ###    ### ###     #### | ");	delay(300);
	gotoxy(x,y + 8);printf("|####       ###      ####   ########       ####          ### ####     ### ###      ### | ");	delay(300);
	gotoxy(x,y + 9);printf("|______________________________________________________________________________________| ");	delay(300);
	changeColor(15);
	timeS=clock();
	do{
		timeE=clock();
	}while(timeE-timeS <= 700);
}

void printMenuBanner(){
	set_color("01;31");
	int x = 47, y = 0;
	gotoxy(x,y);    printf(" _______________________________________________________________ ");
	gotoxy(x,y + 1);printf("|                                                               |");
	gotoxy(x,y + 2);printf("|     $$$$$$     $$$$$ $$$$$$$$$$$ $$$$$$   $$$ $$$      $$$    |");
	gotoxy(x,y + 3);printf("|     $$$ $$$   $$$$$$ $$$$$$$$$$$ $$$ $$$  $$$ $$$      $$$    |");
	gotoxy(x,y + 4);printf("|     $$$  $$$ $$$ $$$ $$$         $$$  $$$ $$$ $$$      $$$    |");
	gotoxy(x,y + 5);printf("|     $$$   $$$$$  $$$ $$$$$$$$$$  $$$   $$$$$$ $$$      $$$    |");
	gotoxy(x,y + 6);printf("|     $$$    $$$   $$$ $$$         $$$    $$$$$ $$$      $$$    |");
	gotoxy(x,y + 7);printf("|     $$$          $$$ $$$$$$$$$$$ $$$     $$$$ $$$      $$$    |");
	gotoxy(x,y + 8);printf("|     $$$          $$$ $$$$$$$$$$$ $$$      $$$  $$$$$$$$$$     |");
	gotoxy(x,y + 9);printf("|_______________________________________________________________|");
	changeColor(15);
}
//***EXIT***///
void exitGame(int choiceMenu){
	int enterMenu;
	gotoxy(65, 13);
	changeColor(252);
	printf("Do you want to exit game?");
	changeColor(15);
	if(choiceMenu == 1){
		changeColor(71);
	}
	gotoxy(70, 15);
	printf("|     YES      |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoxy(70, 16);
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
						gotoxy(i, j);
			        	printf("Goodbye!!!");
						j += 0.25;
			        	system("cls");	
					}
			     	exit(0);
				}
				if(choiceMenu == 2){
					choiceMenu = 0;
		        	printMenuBanner();
					printMenu(choiceMenu);
				}	
			}	
			exitGame(choiceMenu);
		}
	}
	while (enterMenu != 13);
}
///*MODE*///
void mode(int choiceMenu){
	int enterMenu = 0;	
	if(choiceMenu == 1)
		changeColor(30);
	gotoxy(67, 15);
	printf("|     *NORMAL*      |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoxy(67, 16);
	printf("|     **HARD**      |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(95);	
	gotoxy(67, 17);
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
	        	system("cls");
	        	printMenuBanner();
	        	printMenu(enterMenu);
	        	enterMenu = 13;
			}
			mode(choiceMenu);
		}
	}
	while (enterMenu != 13);
}
//****MENU***///
int printMenu(int choiceMenu){	
	int enterMenu = 0 ;
	if(choiceMenu == 1)
		changeColor(30);
	gotoxy(70, 15);
	printf("|     PLAY     |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(30);
	gotoxy(70, 16);
	printf("|     MODE     |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(30);
	gotoxy(70, 17);
	printf("|    SETTING   |");
	changeColor(15);
	Nocursortype();
	char c;
	do{
		if(kbhit()){
		    c = getch();
		    if(c == 72 && choiceMenu > 1){
		        choiceMenu --;
		    }
		    if(c == 80 && choiceMenu < 3){
		        choiceMenu ++;
		    }
		    if(c == 13){
		    	enterMenu = 13;
				if(choiceMenu == 1){
				}
				if(choiceMenu == 2){
					system("cls");
					int num = 0;
					mode(num);
				}
				if(choiceMenu == 3){
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

int main(){
	int choiceMenu = 0;
	resizeConsole(1270, 800);
	Nocursortype();
	intro();
	Nocursortype();
	system("cls");// làm sach màn hinh
	printMenuBanner();
	printMenu(choiceMenu);
	system("cls");	
	return 0;
}