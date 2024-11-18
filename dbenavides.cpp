// Name: Darren Benavides
// Date: 10/08/2024
// Filename: dbenavides.cpp
// Purpose: Project Feature
// Last Edit: 11/18/2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include <ctime>
#include <cstdlib>
#include "dbenavides.h"
#include "images.h"
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
Image background_time() {
    time_t my_time = time(0);
    tm * ltime = localtime(&my_time);

    // Extract Hours
    int hours = ltime->tm_hour;

    if (hours >= 5 && hours < 7) {
        // Dawn
        return "./images/DawnBG.jpg";
    } else if (hours >= 7 && hours < 12) { 
        // Morning
        return "./images/MorningBG.jpg";
    } else if (hours >= 12 && hours < 17) {
        // Afternoon
        return "./images/AfternoonBG.jpg";
    } else if (hours >= 17 && hours < 20) { 
        // Evening
        return "./images/EveningBG.webp";
    } else {
        // Night
        return "./images/NightBG.jpg";
    }
    // Default if it can't pick anything up.
    return "./images/forest.png";
}






