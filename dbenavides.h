// Name: Darren Benavides
// Date: 10/08/2024
// Filename: dbenavides.h
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
#include "fonts.h"
#include <ctime>
#include <cstdlib>
#include "images.h"
using namespace std;

#ifndef _dbenavides
#define _dbenavides
extern int db_show;
enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING, ACE };

void show_my_feature_db(int x, int y);

// Obtained Structure from: 
// https://www.geeksforgeeks.org/print-system-time-c-3-different-ways/
Image background_time();
#endif
