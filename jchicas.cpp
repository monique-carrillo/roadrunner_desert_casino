//Josef Chicas

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
//render main menu text
//In progress - when mouse cursor is over an option,
//change the text color for that option
void render_main_menu(int thingy, int thingx, int count)
{
	Rect r;
	unsigned int A;
	unsigned int B;
	unsigned int C;
	unsigned int D;
	if (count == 1) {
		A = 0x000000FF;
	}
	else {
		A = 0x00FFFF00;
	}
	r.bot = thingy - 250;
	r.left = thingx - 300;
	r.center = 400;
	
	if (count == 2) {
		B = 0x000000FF;
	}
	else {
		B = 0x00FFFF00;
	}
	
	Rect j;
        j.bot = thingy - 300;
        j.left = thingx - 300;
        j.center = 400;

	if (count == 3) {
		D = 0x000000FF;
	}
	else {
		D = 0x00FFFF00;
	}

	Rect f;
	f.bot = thingy - 350;
	f.left = thingx = 300;
	f.center = 400;

	if (count == 4) {
		C = 0x000000FF;
	}
	else {
		C = 0x00FFFF00;
	}


	Rect k;
	k.bot = thingy - 400;
        k.left = thingx + 10;
        k.center = 400;

	ggprint40(&r, 20, A, "Blackjack");
	ggprint40(&j, 20, B, "Poker");
	ggprint40(&f, 20, D, "Roadrunner Racing");
	ggprint40(&k, 20, C, "Exit");


}
