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
#include "jgaribay.h"
using namespace std;

GLuint felt_texture;
int db_show = 0;
string hand_values[10] = {"None","Pair","Two Pair","Three of a Kind",
                          "Straight","Flush","Full House", 
                          "Four of a Kind","Straight Flush","Royal Flush"};

GLuint card_textures[52];
const char* card_filenames[52] = { 
    "./images/cards/c_2.jpg", "./images/cards/c_3.jpg", 
    "./images/cards/c_4.jpg", "./images/cards/c_5.jpg", 
    "./images/cards/c_6.jpg", "./images/cards/c_7.jpg", 
    "./images/cards/c_8.jpg", "./images/cards/c_9.jpg", 
    "./images/cards/c_10.jpg", "./images/cards/c_J.jpg", 
    "./images/cards/c_Q.jpg", "./images/cards/c_K.jpg", 
    "./images/cards/c_A.jpg", "./images/cards/d_2.jpg", 
    "./images/cards/d_3.jpg", "./images/cards/d_4.jpg", 
    "./images/cards/d_5.jpg", "./images/cards/d_6.jpg", 
    "./images/cards/d_7.jpg", "./images/cards/d_8.jpg", 
    "./images/cards/d_9.jpg", "./images/cards/d_10.jpg", 
    "./images/cards/d_J.jpg", "./images/cards/d_Q.jpg", 
    "./images/cards/d_K.jpg", "./images/cards/d_A.jpg", 
    "./images/cards/h_2.jpg", "./images/cards/h_3.jpg", 
    "./images/cards/h_4.jpg", "./images/cards/h_5.jpg", 
    "./images/cards/h_6.jpg", "./images/cards/h_7.jpg", 
    "./images/cards/h_8.jpg", "./images/cards/h_9.jpg", 
    "./images/cards/h_10.jpg", "./images/cards/h_J.jpg", 
    "./images/cards/h_Q.jpg", "./images/cards/h_K.jpg", 
    "./images/cards/h_A.jpg", "./images/cards/s_2.jpg", 
    "./images/cards/s_3.jpg", "./images/cards/s_4.jpg", 
    "./images/cards/s_5.jpg", "./images/cards/s_6.jpg", 
    "./images/cards/s_7.jpg", "./images/cards/s_8.jpg", 
    "./images/cards/s_9.jpg", "./images/cards/s_10.jpg", 
    "./images/cards/s_J.jpg", "./images/cards/s_Q.jpg", 
    "./images/cards/s_K.jpg", "./images/cards/s_A.jpg" };

GLuint load_texture(const char *filename) {
    Image img(filename);
    if (img.data == NULL) {
        printf("Failed to load texture: %s\n", filename);
        return 0;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, img.data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

void init_card_textures() {
    for (int i = 0; i < 52; ++i) {
        card_textures[i] = load_texture(card_filenames[i]);
        if (card_textures[i] == 0) {
            printf("Failed to load texture: %s\n", card_filenames[i]);
        } else {
            printf("Successfully loaded texture for card %d\n", i);
        }
    }
}

void show_db()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, felt_texture);
    glColor3f(1.0, 1.0, 1.0);

    
    glBegin(GL_QUADS); 
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f); 
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f); 
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f); 
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_TEXTURE_2D);

    // Render cards (for example, first 7 cards) 
    float xOffset = -0.8f; 
    for (int i = 0; i < 7; ++i) { 
        glBindTexture(GL_TEXTURE_2D, card_textures[i]); 
        if (card_textures[i] == 0) { 
            printf("Failed to bind texture for card %d\n", i); 
            continue; 
        } 
        glBegin(GL_QUADS); 
            glTexCoord2f(0.0f, 0.0f); glVertex2f(xOffset, -0.3f); 
            glTexCoord2f(1.0f, 0.0f); glVertex2f(xOffset + 0.2f, -0.3f); 
            glTexCoord2f(1.0f, 1.0f); glVertex2f(xOffset + 0.2f, 0.3f); 
            glTexCoord2f(0.0f, 1.0f); glVertex2f(xOffset, 0.3f); 
        glEnd(); 
        xOffset += 0.25f; 
    } 
    glBindTexture(GL_TEXTURE_2D, 0); 
    glDisable(GL_TEXTURE_2D);

    Rect r;
    r.bot = 290;
    r.left = 20;
    r.center = 0;
    glColor3f(1.0, 1.0, 1.0);

    ggprint16(&r, 16, 0x00ffffff, "Poker Game");

    srand(time(NULL));
    int deck[52];

    for (int i=0; i<52; i++) {
        deck[i] = i + 1;
    }

    // Shuffle
    shuffling(deck);
    for (int i=0; i<52; i++) {
        ggprint16(&r, 16, 0x00ffffff, "%d", deck[i]);
    }

    ggprint16(&r, 16, 0x00ffffff, "Money: ");

    Hand table[5];
    Hand player[2];

    // Dealing
    dealing(table, deck, 0, 5);
    dealing(player, deck, 5, 2);
    //cout << endl;

    // Sort
    sorting(table, 5);
    sorting(player, 2);
    for (int i=0; i<5; i++) {
        //cout << table[i].num << " ";
        ggprint16(&r, 16, 0x00ffffff, "%d", table[i].num);
    }
    for (int i=0; i<2; i++) {
        //cout << player[i].num << " ";
        ggprint16(&r, 16, 0x00ffffff, "%d", player[i].num);
    }

    // Calculate
    string highhand = hand_values[calculating(table, player)];
    //cout<< "Highest Hand: " << hand_values[calculating(table, player)] << 
    //    " " << endl;
    ggprint16(&r, 16, 0x00ffffff, "Highest Hand Value: %s", highhand);
    glDisable(GL_BLEND);
}

void shuffling(int *deck) {
    //cout << "I am shuffling\n";
    int n = rand() % 1000 + 1000;
    for (int i=0; i<n; i++) {
        int a = rand() % 52;
        int b = rand() % 52;
        swap(deck[a], deck[b]);
    }
}

void sorting(Hand *hand, int size_of_hand) {
    //cout << "I am sorting.\n";
    // Info found at:
    // https://www.w3schools.com/cpp/cpp_arrays_size.asp
    if (size_of_hand == 5) {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4-i; j++) {
                if (hand[j].num > hand[j+1].num) {
                    swap(hand[j], hand[j+1]);
                }
            }
        }
    } else if (size_of_hand == 2) {
        for (int i=0; i<1; i++) {
            if (hand[i+1].num < hand[i].num) {
                swap(hand[i], hand[i+1]);
            }
        }
    }
}

void dealing(Hand *hand, int *deck, int start, int size_of_hand) {
    //cout << "I am dealing.\n";
    for (int i=0; i<size_of_hand; i++) {
        hand[i].texmap = deck[start + i];
        hand[i].num = (deck[start + i] - 1) % 13 + 1;
        hand[i].suit = (deck[start + i] - 1) / 13;
        //cout<<"Dealt card:"<<hand[i].num<<" of suit "<<hand[i].suit<<endl;
    }
}

int calculating(Hand *hand, Hand *hand2) {
    //cout << "I am calculating.\n";
    int temphand[7];
    int tempsuits[4] = {0};
    // Hands: None(0), Pairs(1), 2Pair(2), 3oKind(3), Straight(4), Flush(5),
    //        FullHouse(6), 4oKind(7), StraightFlush(8), RoyalFlush(9)
    int possible_hands[10] = {0};

    // Setting Temp Hand
    //cout << "Setting Temp Hand\n";
    for (int i=0; i<5; i++) {
        temphand[i] = hand[i].num;
        //cout << temphand[i] << " ";
    }
    //cout << endl;
    for (int i=0; i<2; i++) {
        temphand[i+5] = hand2[i].num;
        //cout << temphand[i+5] << " ";
    }
    //cout << endl;

    // Suit counting
    //cout << "Suit counting\n";
    for (int i=0; i<5; i++) {
        tempsuits[hand[i].suit]++;
    }
    for (int i=0; i<2; i++) {
        tempsuits[hand2[i].suit]++;
    }
    for (int i=0; i<4; i++) {
        //cout<<"Suits count for suit "<<i<<": "<<tempsuits[i]<<endl;
    }

    // Sorting Temp Hand
    //cout << "Sorting Temp Hand\n";
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            if (temphand[j] > temphand[j+1]) {
                swap(temphand[j], temphand[j+1]);
            }
        }
    }

    // Finding out value of best hand
    
    // Pairs possible_hands[1];
    //cout << "Finally Calculating Best Hand.\n";
    for (int i=0; i<6; i++) {
       if (temphand[i] == temphand[i+1]) {
           possible_hands[1]++;
           i++;
       }
       // 2 Pairs possible_hands[2]; 
       if (possible_hands[1] >= 2) {
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
    //cout << "I am testing for straight.\n";
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
    //cout << "I am testing for straight flush.\n";
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
    //cout << "I am testing for royal flush.\n";
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
    return "./images/dawn.jpg";
}

void init_felttex() {
    int w, h;
    Image img = "./images/felt.jpg";
    if (img.data == NULL) {
        printf("Failed to load felt texture\n");
        return;
    }
    w = img.width;
    h = img.height;

    glGenTextures(1, &felt_texture);
    glBindTexture(GL_TEXTURE_2D, felt_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, 
                 GL_UNSIGNED_BYTE, img.data);
    glBindTexture(GL_TEXTURE_2D, 0);
}














