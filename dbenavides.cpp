// Name: Darren Benavides
// Date: 10/08/2024
// Filename: dbenavides.cpp
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
#include "dbenavides.h"
using namespace std;

int db_show = 0;

void show_my_feature_db(int x, int y)
{
    //draw a rectangle
    //show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x0000ff00, "Darren's Feature");

}

// Obtained Structure from: 
// https://www.geeksforgeeks.org/print-system-time-c-3-different-ways/
void background_time() {
    time_t my_time = time(0);
    tm * ltime = localtime(&my_time);

    // Extract Hours
    int hours = ltime->tm_hour;

    if (5 <= hours) {
        if (hours < 6){
            // Dawn
            system("eog ./images/DawnBG.jpg");
        }
    } else if (6 <= hours) { 
        if (hours < 12) {
            // Morning
            system("eog ./images/MorningBG.jpg");
        }
    } else if (12 <= hours) {
        if (hours < 17) {
            // Afternoon
            system("eog ./images/Afternoon.jpg");
        }
    } else if (17 <= hours) { 
        if (hours < 20) {
            // Evening
            system("eog ./images/EveningBG.webp");
        }
    } else if (20 <= hours) {
        // Night
        system("eog ./images/NightBG.jpg");
    } else if (hours < 5) {
        //Night
        system("eog ./images/NightBG.jpg");
    }
}
/*#ifndef _dbenavides
#define _dbenavides
int db_show = 0;

void show_my_feature_db(int x, int y)
{
    //draw a rectangle
    //show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x0000ff00, "Darren's Feature");

}*/
