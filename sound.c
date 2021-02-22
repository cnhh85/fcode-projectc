#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "sound.h"

void playTheme() {
	PlaySound(TEXT("C:\\fcode-projectc\\audio\\theme.wav"), NULL, SND_SYNC | SND_LOOP);
}
void playEating() {
	PlaySound(TEXT("C:\\fcode-projectc\\audio\\eating.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void playDead() {
	PlaySound(TEXT("C:\\fcode-projectc\\audio\\dead.wav"), NULL, SND_ASYNC | SND_LOOP);
}
