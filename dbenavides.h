// Name: Darren Benavides
// Date: 10/08/2024
// Filename: dbenavides.h
// Purpose: Project Feature
// Last Edit: 10/28/2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
//#include "ppm.h"
#include "fonts.h"
#include <ctime>
#include <cstdlib>
using namespace std;

#ifndef _dbenavides
#define _dbenavides
extern int db_show;
void show_my_feature_db(int x, int y);

// Obtained Structure from: 
// https://www.geeksforgeeks.org/print-system-time-c-3-different-ways/
void background_time();
#endif