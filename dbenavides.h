// Name: Darren Benavides
// Date: 10/08/2024
// Filename: dbenavides.h
// Purpose: Project Feature
// Last Edit: 11/18/2024
#include <iostream>
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
// Variables
extern int db_show;
extern string hand_values[10];

// Structures
struct Hand {
    int texmap;
    int num;
    int suit;
    int curr_hand;
};

// Prototypes
//void show_db(int x, int y);
void show_db();
void set_hand();
void shuffling(int *deck);
void sorting(Hand *hand);
void dealing(Hand *hand, int *deck);
int calculating(Hand *hand, Hand *hand2);
bool is_straight(int *hand);
bool is_sflush(Hand *hand, Hand *hand2);
bool is_rflush(Hand *hand, Hand *hand2);

// Obtained Structure from: 
// https://www.geeksforgeeks.org/print-system-time-c-3-different-ways/
Image background_time();

#endif
