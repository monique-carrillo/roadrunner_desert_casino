// Name: Darren Benavides
// Date: 10/08/2024
// Filename: dbenavides.cpp
// Purpose: Project Feature
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
/*
using namespace std;

// Obtained Structure from: 
// https://www.geeksforgeeks.org/print-system-time-c-3-different-ways/
std::string background_time() {
    time_t my_time = time(0);
    tm * ltime = localtime(&my_time);
    
    // Extract Hours
    int hours = ltime->tm_hour;

    if (5 <= hours && hours < 6) {
        // Dawn
        return "./images/DawnBG.jpg";
    } else if (6 <= hours && hours < 12) {
        // Morning
        return "./images/MorningBG.jpg";
    } else if (12 <= hours && hours < 17) {
        // Afternoon
        return "./images/Afternoon.jpg";
    } else if (17 <= hours && hours < 20) {
        // Evening
        return "./images/EveningBG.webp";
    } else if (20 <= hours || hours < 5) {
        // Night
        return "./images/NightBG.jpg";
    }
}*/
#ifndef _dbenavides
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

    }
#endif
