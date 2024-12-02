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
#include "images.h"
#include "fonts.h"

using namespace std;

int xres, yres;
int player[4][3];
int winplayer;
void render_num();
void render_win(int num);

int ding() {

	int num = rand() % (6 - 1 + 1) + 1;
	return num;
}




void render_word() {
	Rect r, s;
	Rect one, two, three, four;

	r.bot = yres - 30;
	r.left = 20;
	r.center = 0;
	ggprint13(&r, 16, 0x00FFFFFF, "Cee-lo");

	s.bot = yres - 50;
	s.left = 20;
	s.center = 0;
	ggprint13(&s, 16, 0x00FFFFFF, "press space to roll");


	one.bot = yres - 100;
	one.left = 20;
	one.center = 0;

	two.bot = yres - 200;
	two.left = 20;
	two.center = 0;

	three.bot = yres - 300;
	three.left = 20;
	three.center = 0;

	
	   four.bot = yres - 400;
	   four.left = 20;
	   four.center = 0;
	   

	ggprint40(&one, 16, 0x00FFFFFF, "Player one(you):");
	ggprint40(&two, 16, 0x00FFFFFF, "Player two:");
	ggprint40(&three, 16, 0x00FFFFFF, "Player three:");
	ggprint40(&four, 16, 0x00FFFFFF, "Player four:");

	render_num();
	render_win(winplayer);

}


void render_win(int num) {
	Rect w;

	w.bot = yres - 420;
	w.left = 20;
	w.center = 0;

	if (num == 1) {
		ggprint13(&w, 16, 0x00FFFFFF, "You win!");
	}

	if (num == 2) {
		ggprint13(&w, 16, 0x00FFFFFF, "Player two wins!");
	}

	if (num == 3) {
		ggprint13(&w, 16, 0x00FFFFFF, "Player three wins!");
	}

	
	   if (num == 4) {
	   ggprint13(&w, 16, 0x00FFFFFF, "Player four wins!");

	   }
	   


}

int ceelo(int num1, int num2) {

	xres = num1;
	yres = num2;

	srand(time(NULL));
	int count = 0;
	int winplayer;
	int max;
	bool Wincheck = false;
	int test1, test2, test3;
	int compnum[4];


	for (int i = 0; i < 4; i++) {

		cout << "Player " << i + 1 << " has rolled: ";

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

		cout << test1 << " " << test2 << " " << test3;
		cout << endl;
		count++;
	}

	if (Wincheck == false) {

		for (int i = 0; i < 3; i++) {
			if (max < compnum[i]) {
				max = compnum[i];
				winplayer = i + 1;

			}

		}

	}

	//cout << "The winner is " << winplayer << endl;
	return winplayer;
}

void render_num() {

	Rect num[12];
	int left = 350;
	int bot = 100;

	for (int i = 0; i < 3; i++) {
		num[i].bot = yres - bot;
		num[i].left = left;
		num[i].center = 0;

		left = left + 100;
	}

	for (int i = 0; i < 3; i++) {
		if(player[0][i] == 1) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "1");
		}
		if(player[0][i] == 2) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "2");
		}
		if(player[0][i] == 3) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "3");
		}
		if(player[0][i] == 4) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "4");
		}
		if(player[0][i] == 5) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "5");
		}
		if(player[0][i] == 6) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "6");
		}

	}

	left = 350;
	bot = 200;

	for (int i = 3; i < 6; i++) {
		num[i].bot = yres - bot;
		num[i].left = left;
		num[i].center = 0;

		left = left + 100;
	}

	int count = 0;

	for (int i = 3; i < 6; i++) {
		if(player[1][count] == 1) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "1");
		}
		if(player[1][count] == 2) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "2");
		}
		if(player[1][count] == 3) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "3");
		}
		if(player[1][count] == 4) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "4");
		}
		if(player[1][count] == 5) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "5");
		}
		if(player[1][count] == 6) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "6");
		}
		count++;
	}

	count = 0;
	left = 350;
	bot = 300;

	for (int i = 6; i < 9; i++) {
		num[i].bot = yres - bot;
		num[i].left = left;
		num[i].center = 0;

		left = left + 100;
	}


	for (int i = 6; i < 9; i++) {
		if(player[2][count] == 1) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "1");
		}
		if(player[2][count] == 2) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "2");
		}
		if(player[2][count] == 3) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "3");
		}
		if(player[2][count] == 4) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "4");
		}
		if(player[2][count] == 5) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "5");
		}
		if(player[2][count] == 6) {
			ggprint40(&num[i], 16, 0x00FFFFFF, "6");
		}
		count++;
	}

	
	   left = 350;
	   bot = 400;
	   count = 0;

	   for (int i = 9; i < 12; i++) {
	   num[i].bot = yres - bot;
	   num[i].left = left;
	   num[i].center = 0;

	   left = left + 100;
	   }


	   for (int i = 9; i < 12; i++) {
	   if(player[3][count] == 1) {
	   ggprint40(&num[i], 16, 0x00FFFFFF, "1");
	   }
	   if(player[3][count] == 2) {
	   ggprint40(&num[i], 16, 0x00FFFFFF, "2");
	   }
	   if(player[3][count] == 3) {
	   ggprint40(&num[i], 16, 0x00FFFFFF, "3");
	   }
	   if(player[3][count] == 4) {
	   ggprint40(&num[i], 16, 0x00FFFFFF, "4");
	   }
	   if(player[3][count] == 5) {
	   ggprint40(&num[i], 16, 0x00FFFFFF, "5");
	   }
	   if(player[3][count] == 6) {
	   ggprint40(&num[i], 16, 0x00FFFFFF, "6");
	   }
	   if(player[3][count] == 0) {
	   ggprint40(&num[i], 16, 0x00FFFFFF, "0");
	   }
	   count++;
	   }




}




