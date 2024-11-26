//Josef Chicas
#include <algorithm>
#include <iostream>
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

using namespace std;

int ding()
{

int num = rand() % (6 - 1 + 1) + 1;
return num;
}


int ceelo() {

srand(time(NULL));
int count = 0;
int player[4][3];
int winplayer;
int max;
bool Wincheck = false;
int test1, test2, test3;
int compnum[4];


for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 3; j++) {

                player[i][j] = ding();
                if (j == 2) {

                        sort(player[i], player[i] + 3);
                        test1 = player[i][0];
                        test2 = player[i][1];
                        test3 = player[i][2];

                        if (test1 != test2 && test2 != test3) {
                                if (test1 == 4 && test2 == 5 && test3 == 6) {
                                        Wincheck = true;
                                        winplayer = i + 1;

                                }
                                else if (Wincheck == false) {
                                        j = 0;
                                }
                        }
                        else if (Wincheck == false) {

                                if (test1 == test2) {

                                        compnum[i] = test3;

                                }

                                else {
					 compnum[i] = test1;
                                }
                        }


                }

        }


cout << endl;
count++;
}

if (Wincheck == false) {

for (int i = 0; i < 4; i++) {
if (max < compnum[i]) {
        max = compnum[i];
        winplayer = i + 1;

}

}

}


return winplayer;
}
