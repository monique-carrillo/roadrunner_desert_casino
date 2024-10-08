//Josef Chicas
//Indiv. source file
//input and display names

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <string>

Display *dpy;
Window win;
std::string name;

void cleanupXWindows();
void displayname();

int display() {







return 0;
}


void cleanupXWindows() {
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void displayname() {
	

}
