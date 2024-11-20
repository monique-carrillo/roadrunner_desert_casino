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
#include "fonts.h"
#include <ctime>
#include <cstdlib>
#include "dbenavides.h"
#include "images.h"
using namespace std;

int db_show = 0;
string hand_values[10] = {"None","Pair","Two Pair","Three of a Kind",
                          "Straight","Flush","Full House", 
                          "Four of a Kind","Straight Flush","Royal Flush"};

void show_db()
{
    srand(time(NULL));
    int deck[52];

    for (int i=0; i<52; i++) {
        deck[i] = i + 1;
    }

    // Shuffle
    shuffling(deck);
    for (int i=0; i<52; i++) {
        cout << deck[i] << " ";
        cout << endl;
    }
    cout << endl;

    Hand table[5];
    Hand player[2];

    // Dealing
    dealing(table, deck);
    dealing(player, deck);
    cout << endl;

    // Sort
    sorting(table);
    sorting(player);
    for (int i=0; i<5; i++) {
        cout << table[i].num << " ";
        cout << endl;
    }
    for (int i=0; i<2; i++) {
        cout << player[i].num << " ";
        cout << endl;
    }

    // Calculate
    cout << hand_values[calculating(table, player)] << " ";
    cout << endl;

    //draw a rectangle
    //show some text
    //Rect r;
    //r.bot = y;
    //r.left = x;
    //r.center = 0;
    //ggprint8b(&r, 16, 0x0000ff00, "Darren's Feature");
}

void shuffling(int *deck) {
    int n = rand() % 1000 + 1000;
    for (int i=0; i<n; i++) {
        int a = rand() % 52;
        int b = rand() % 52;
        swap(deck[a], deck[b]);
    }
}

void sorting(Hand *hand) {
    // Info found at:
    // https://www.w3schools.com/cpp/cpp_arrays_size.asp
    if ((sizeof(hand)) == 5) {
        for (int i=0; i<4; i++) {
            for (int j=0; i<4; i++) {
                if (hand[j].num > hand[j+1].num) {
                    swap(hand[j].num, hand[j+1].num);
                }
            }
        }
    } else if ((sizeof(hand)) == 2) {
        for (int i=0; i<1; i++) {
            if (hand[i+1].num < hand[i].num) {
                swap(hand[i].num, hand[i+1].num);
            }
        }
    }
}

void dealing(Hand *hand, int *deck) {
    if ((sizeof(hand)) == 1) {
        for (int i=0; i<5; i++) {
            hand[i].texmap = deck[i];
            hand[i].num = deck[i] % 13;
            hand[i].suit = deck[i] % 4;
        }
    } else if ((sizeof(hand)) == 2) {
        for (int i=5; i<7; i++) {
            hand[i].texmap = deck[i];
            hand[i].num = deck[i] % 13;
            hand[i].suit = deck[i] % 4;
        }
    }
}

int calculating(Hand *hand, Hand *hand2) {
    int temphand[7];
    int tempsuits[4] = {0};
    // Hands: None, Pairs, 2Pair, 3oKind, Straight, Flush, FullHouse,
    //        4oKind, StraightFlush, RoyalFlush
    int possible_hands[10] = {1,0,0,0,0,0,0,0,0,0};

    // Setting Temp Hand
    for (int i=0; i<5; i++) {
        temphand[i] = hand[i].num;
    }
    for (int i=0; i<2; i++) {
        temphand[i+5] = hand2[i].num;
    }

    // Suit counting
    for (int i=0; i<5; i++) {
        tempsuits[hand[i].suit]++;
    }
    for (int i=0; i<2; i++) {
        tempsuits[hand2[i].suit]++;
    }

    // Sorting Temp Hand
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; i++) {
            if (temphand[j] > temphand[j+1]) {
                swap(temphand[j], temphand[j+1]);
            }
        }
    }

    // Finding out value of best hand
    
    // Pairs possible_hands[1];
    for (int i=0; i<6; i++) {
       if (temphand[i] == temphand[i+1]) {
           possible_hands[1]++;
       }
       // 2 Pairs possible_hands[2]; 
       if (possible_hands[1] % 3 == 0) {
           possible_hands[2]++;
       }
    }

    // 3oKind possible_hands[3];
    for (int i=0; i<5; i++) {
        if (temphand[i] == temphand[i+1] && temphand[i+1] == temphand[i+2]){
            possible_hands[3]++;
        }
    }

    // Full House possible_hands[6];
    if (possible_hands[1] && possible_hands[3]) {
        possible_hands[6]++;
    }

    // 4oKind possbile_hands[7];
    for (int i=0; i<3; i++) {
        if (temphand[i] == temphand[i+1] &&
            temphand[i+1] == temphand[i+2] &&
            temphand[i+2] == temphand[i+3]) {
            possible_hands[7]++;
        }
    }

    // Straight possible_hands[4];
    if (is_straight(temphand)) {
        possible_hands[4]++;
    }

    // Flush possible_hands[5]
    for (int i=0; i<7; i++) {
        if (i < 5) {
            tempsuits[hand[i].suit]++;
        } else if (i >= 5) {
            tempsuits[hand2[i-5].suit]++;
        }
    }
    for (int i=0; i<4; i++) {
        if (tempsuits[i] >= 5) {
            possible_hands[5]++;
        }
    }

    // Straight Flush possible_hands[8];
    if (is_sflush(hand, hand2)) {
        possible_hands[8]++;
    }

    // Royal Flush possible_hands[9];
    if (is_rflush(hand, hand2)) {
        possible_hands[9]++;
    }
    
    // Sending Int Position of Highest Hand
    for (int i=9; i>=0; i--) {
        if (possible_hands[i]) {
            return i;
        }
    }
    return 0;
// End of Calculating
}

bool is_straight(int *hand) {
    // Will check for Straight
    // Low Ace
    for (int i=0; i<3; i++) {
        if (hand[i] + 1 == hand[i+1] && hand[i+1] + 1 == hand[i+2] &&
            hand[i+2] + 1 == hand[i+3] && hand[i+3] + 1 == hand[i+4]) {
            return true;
        }
    }
    // High Ace
    if (hand[3] == 10 && hand[4] == 11 && hand[5] == 12 && hand[6] == 13 &&
        hand[0] == 1) {
        return true;
    }
    return false;
}

bool is_sflush(Hand *hand, Hand *hand2) {
    // Will check for Straight Flush
    int suits[4] = {0};

    for (int i=0; i<5; i++) {
        suits[hand[i].suit]++;
    }
    for (int i=0; i<2; i++) {
        suits[hand2[i].suit]++;
    }

    // Checking each suit for Straight
    for (int i=0; i<4; i++) {
        if (suits[i] >= 5) {
            // Collect the cards
            int suited[7], k=0;
            for (int j=0; j<5; j++) {
                if (hand[j].suit == i) {
                    suited[k++] = hand[j].num;
                }
            }
            for (int j=0; j<2; j++) {
                if (hand2[j].suit == i) {
                    suited[k++] = hand2[j].num;
                }
            }
            // Checking for straight here
            if (is_straight(suited)) {
                return true;
            }
        }
    }

    return false;
}

bool is_rflush(Hand *hand, Hand *hand2) {
    // Will check for Royal Flush
    int suits[4] = {0};

    for (int i = 0; i < 5; i++) { 
        suits[hand[i].suit]++; 
    } 
    for (int i = 0; i < 2; i++) { 
        suits[hand2[i].suit]++; 
    } 

    // Check for flush 
    for (int i = 0; i < 4; i++) { 
        if (suits[i] >= 5) { 
            // Collect cards of the same suit 
            int suited[7], k = 0; 
            for (int j = 0; j < 5; j++) { 
                if (hand[j].suit == i) { 
                    suited[k++] = hand[j].num; 
                } 
            } 
            for (int j = 0; j < 2; j++) { 
                if (hand2[j].suit == i) { 
                    suited[k++] = hand2[j].num; 
                } 
            } 

            // Check for a royal flush 
            bool hasTen = false, hasJack = false, hasQueen = false, 
                 hasKing = false, hasAce = false; 
            for (int j = 0; j < 7; j++) { 
                if (suited[j] == 10) 
                    hasTen = true; 
                if (suited[j] == 11) 
                    hasJack = true; 
                if (suited[j] == 12) 
                    hasQueen = true; 
                if (suited[j] == 13) 
                    hasKing = true; 
                if (suited[j] == 1) 
                    hasAce = true; 
            } 
            if (hasTen && hasJack && hasQueen && 
                    hasKing && hasAce) { 
                return true; 
            } 
        } 
    }

    return false;
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
        return "./images/dawn.jpg";
    } else if (hours >= 7 && hours < 12) { 
        // Morning
        return "./images/morning.jpg";
    } else if (hours >= 12 && hours < 17) {
        // Afternoon
        return "./images/afternoon.jpg";
    } else if (hours >= 17 && hours < 20) { 
        // Evening
        return "./images/evening.jpg";
    } else {
        // Night
        return "./images/night.jpg";
    }
    // Default if it can't pick anything up.
    return "./images/dawn.png";
}






