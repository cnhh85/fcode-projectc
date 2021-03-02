#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "sound.h"

void playTheme() {
	PlaySound(TEXT(".audio\\theme.wav"), NULL, SND_SYNC | SND_LOOP);
}
void playEating() {
	PlaySound(TEXT(".audio\\eating.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void playDead() {
	PlaySound(TEXT(".audio\\dead.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void playMenuScroll() {
	PlaySound(TEXT(".audio\\menuscroll.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void playMenuSellect() {
	PlaySound(TEXT(".audio\\menuselect.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void playMenuTheme() {
	PlaySound(TEXT(".audio\\menutheme.wav"), NULL, SND_ASYNC | SND_LOOP);
}
