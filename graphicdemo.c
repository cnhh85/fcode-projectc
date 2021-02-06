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
	int x = 47, y = 5;
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
//win lose//
void lose(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 200);
	set_color("01;91");
	int x = 30, y = 10;
	gotoxy(x,y);    printf(" ______________________________________________________________________________");      delay(500);
	gotoxy(x,y + 1);printf("|                                                                              |");     delay(100);
	gotoxy(x,y + 2);printf("| YYY      YYY   OOOOO    UUU    UUU     LLL       OOOOO      SSSSS   EEEEEEEE | ");    delay(100);
	gotoxy(x,y + 3);printf("|   YYY   YYY  OOO   OOO  UUU    UUU     LLL     OOO   OOO  SSS   SSS EEE      | ");	delay(100);
	gotoxy(x,y + 4);printf("|    YYY YYY  OOO     OOO UUU    UUU     LLL    OOO     OOO  SSS      EEE      | ");	delay(100);
	gotoxy(x,y + 5);printf("|      YYY    OOO     OOO UUU    UUU     LLL    OOO     OOO     SSS   EEEEEEEE | ");	delay(100);
	gotoxy(x,y + 6);printf("|      YYY    OOO     OOO UUU    UUU     LLL    OOO     OOO      SSS  EEE      | ");	delay(100);
	gotoxy(x,y + 7);printf("|      YYY     OOO   OOO   UUU  UUU      LLL     OOO   OOO   SSS  SSS EEE      | ");	delay(100);
	gotoxy(x,y + 8);printf("|      YYY       OOOOO      UUUUUU       LLLLLLLL  OOOOO       SSSS   EEEEEEEE | ");	delay(100);
	gotoxy(x,y + 9);printf("|______________________________________________________________________________| ");	delay(100);
	gotoxy(x + 33,y + 13);printf("PLAY AGAIN!!");
	changeColor(15);
}

void win(){
	clock_t timeS,timeE;//thoi gian xuat hien
	timeS=clock();
	do
	{
		timeE=clock();
	}while(timeE-timeS <= 200);
	set_color("01;36");
	int x = 30, y = 10;
	gotoxy(x,y);    printf(" ______________________________________________________________________________");  delay(500);
	gotoxy(x,y + 1);printf("|                                                                              |"); delay(100);
	gotoxy(x,y + 2);printf("| YYY      YYY   OOOOO    UUU    UUU   www  www    www  www iiiii NNNN    NNNN |"); delay(100);
	gotoxy(x,y + 3);printf("|   YYY   YYY  OOO   OOO  UUU    UUU    www  ww    ww  www   iii  NNNNNN  NNN  |");	delay(100);
	gotoxy(x,y + 4);printf("|    YYY YYY  OOO     OOO UUU    UUU     www  ww  ww  www    iii  NNN NNN NNN  |");	delay(100);
	gotoxy(x,y + 5);printf("|      YYY    OOO     OOO UUU    UUU      www  wwww  www     iii  NNN  NNNNNN  |");	delay(100);
	gotoxy(x,y + 6);printf("|      YYY    OOO     OOO UUU    UUU       www wwww www      iii  NNN   NNNNN  |");	delay(100);
	gotoxy(x,y + 7);printf("|      YYY     OOO   OOO   UUU  UUU         wwww  wwww       iii  NNN    NNNN  |");	delay(100);
	gotoxy(x,y + 8);printf("|      YYY       OOOOO      UUUUUU           www  www       iiiii NNN     NNN  |");	delay(100);
	gotoxy(x,y + 9);printf("|______________________________________________________________________________|");	delay(100);
	gotoxy(x + 33,y + 13);printf("PLAY AGAIN!!");
	changeColor(15);
}


//***khung***
void frame(){
	int x = 20, y = 0, j;
	changeColor(12);
	gotoxy(x,y);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 187);
	for(j = 1; j < 35; j++){
		gotoxy(x,y + j);printf("%c", 186); 
		gotoxy(x+ 119,y + j);printf("%c", 186);
	}
	gotoxy(x,y+j);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 205,205, 205, 205, 205, 205, 188);
	changeColor(15);
}

//***EXIT***///
void exitGame(int choiceMenu){
	int enterMenu, y = 18;
	frame();
	gotoxy(65, y);
	changeColor(252);
	printf("Do you want to exit game?");
	changeColor(15);
	if(choiceMenu == 1){
		changeColor(71);
	}
	gotoxy(70, y + 1);
	printf("|     YES      |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoxy(70, y + 2);
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
///*MODE*///
void mode(int choiceMenu){
	int enterMenu = 0, y = 18;
	frame();	
	if(choiceMenu == 1)
		changeColor(30);
	gotoxy(67, y);
	printf("|     *NORMAL*      |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(71);
	gotoxy(67, y + 1);
	printf("|     **HARD**      |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(95);	
	gotoxy(67, y + 2);
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
//****MENU***///
int printMenu(int choiceMenu){	
	int enterMenu = 0, y = 18 ;
	if(choiceMenu == 1)
		changeColor(30);
	gotoxy(70, y);
	printf("|     PLAY     |");
	changeColor(15);
	if(choiceMenu == 2)
		changeColor(30);
	gotoxy(70, y + 1);
	printf("|     MODE     |");
	changeColor(15);
	if(choiceMenu == 3)
		changeColor(30);
	gotoxy(70, y + 2);
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
	frame();
	printMenu(choiceMenu);
	system("cls");	
	win();
	return 0;
}
