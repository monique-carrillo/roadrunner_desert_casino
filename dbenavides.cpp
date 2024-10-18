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
using namespace std;

// Obtained Structure from: 
// https://www.geeksforgeeks.org/print-system-time-c-3-different-ways/
void background_time() {
    time_t my_time = time(0);
    tm * ltime = localtime(&my_time);
    
    // Extract Hours
    int hours = ltime->tm_hour;

    if (5 <= hours < 6) {
        // Dawn
        return "./images/DawnBG.jpg";
    } else if (6 <= hours < 12) {
        // Morning
        return "./images/MorningBG.jpg";
    } else if (12 <= hours < 17) {
        // Afternoon
        return "./images/Afternoon.jpg";
    } else if (17 <= hours < 20) {
        // Evening
        return "./images/EveningBG.webp";
    } else if (20 <= hours || hours < 5) {
        // Night
        return "./images/NightBG.jpg";
    }
}