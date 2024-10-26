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
#include "log.h"
#include "fonts.h"
//render main menu text
//In progress - when mouse cursor is over an option,
//change the text color for that option
void render_main_menu()
{

	Rect r;
	unsigned int c;
	c = 0x00ffff44;
	r.bot = 600 - 350;
	r.left = 300;
	r.center = 400;
	
	
	
	Rect j;
        j.bot = 600 - 400;
        j.left = 300;
        j.center = 400;

	Rect k;
	k.bot = 600 - 450;
        k.left = 300;
        k.center = 400;

	ggprint40(&r, 20, c, "Blackjack");
	ggprint40(&j, 20, c, "Poker");
	ggprint40(&k, 20, 0x00ff0000, "Exit");


}
