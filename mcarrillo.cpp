// Monique Carrillo
// 10/8/24
// Updated: 11/24/2024

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <algorithm>
#include <random>
#include "mcarrillo.h"
#include "images.h"
#include "fonts.h"
#include "jgaribay.h"

using namespace std;
int monique_show = 0;

string dealer[2] = {"None","None"};
string mplayer[2] = {"None", "None"};
string val = "None";

int deal[2] = {0, 0};
int play[2] = {0, 0};

int hit = 0;
float prize = 0.00;

class Global {
    public:
        float money;
        int xres, yres;
        int paused;
        int gamemode;
        int done;
        int win;
        GLuint bgTexture;
        Image *felt_image;
        GLuint felt_texture;
        Image *card_image;
        GLuint card_texture;
        GLuint card_img[52];
        Global() {
            //done = 0;
            //gamemode = MODE_MENU;
            //paused = 0;
            //win = 0;
            xres = 800;
            yres = 600;
        }
} m;

GLuint d[2];
GLuint p[2];

Image pic[2] = {
    "./images/felt.jpg",
    "./images/cards/c_A.jpg"
};

Image card[52] = {
    "./images/cards/c_A.jpg",
    "./images/cards/c_2.jpg",
    "./images/cards/c_3.jpg",
    "./images/cards/c_4.jpg",
    "./images/cards/c_5.jpg",
    "./images/cards/c_6.jpg",
    "./images/cards/c_7.jpg",
    "./images/cards/c_8.jpg",
    "./images/cards/c_9.jpg",
    "./images/cards/c_10.jpg",
    "./images/cards/c_K.jpg",
    "./images/cards/c_Q.jpg",
    "./images/cards/c_J.jpg",
    "./images/cards/d_A.jpg",
    "./images/cards/d_2.jpg",
    "./images/cards/d_3.jpg",
    "./images/cards/d_4.jpg",
    "./images/cards/d_5.jpg",
    "./images/cards/d_6.jpg",
    "./images/cards/d_7.jpg",
    "./images/cards/d_8.jpg",
    "./images/cards/d_9.jpg",
    "./images/cards/d_10.jpg",
    "./images/cards/d_K.jpg",
    "./images/cards/d_Q.jpg",
    "./images/cards/d_J.jpg",
    "./images/cards/h_A.jpg",
    "./images/cards/h_2.jpg",
    "./images/cards/h_3.jpg",
    "./images/cards/h_4.jpg",
    "./images/cards/h_5.jpg",
    "./images/cards/h_6.jpg",
    "./images/cards/h_7.jpg",
    "./images/cards/h_8.jpg",
    "./images/cards/h_9.jpg",
    "./images/cards/h_10.jpg",
    "./images/cards/h_K.jpg",
    "./images/cards/h_Q.jpg",
    "./images/cards/h_J.jpg",
    "./images/cards/h_A.jpg",
    "./images/cards/s_2.jpg",
    "./images/cards/s_3.jpg",
    "./images/cards/s_4.jpg",
    "./images/cards/s_5.jpg",
    "./images/cards/s_6.jpg",
    "./images/cards/s_7.jpg",
    "./images/cards/s_8.jpg",
    "./images/cards/s_9.jpg",
    "./images/cards/s_10.jpg",
    "./images/cards/s_K.jpg",
    "./images/cards/s_Q.jpg",
    "./images/cards/s_J.jpg"
};


struct Card {
    int value;  
    string suit;  
    string name;
};


Card deck[52];

void initializeDeck() 
{
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string names[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", 
        "Queen", "King", "Ace"};
    // Ace initially 11
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};  

    int index = 0;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            deck[index++] = {values[rank], suits[suit], names[rank]};
        }
    }
}

string con(int card_pos) {
    // Map card positions to human-readable names
    static const string suits[] = {"Clubs", "Diamonds", "Hearts", "Spades"};
    static const string values[] = {
        "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10",
        "Jack", "Queen", "King"
    };
    int value = (card_pos - 1) % 13;
    int suit = (card_pos - 1) / 13;
    return values[value] + " of " + suits[suit];
}

void shuffleDeck() 
{
    for (int i = 0; i < 52; ++i) {
        int randomIndex = rand() % 52;
        swap(deck[i], deck[randomIndex]);
    }
}

void dealCards(Card *hand, int &handSize, int cardsToDeal, int &deckIndex) 
{
    for (int i = 0; i < cardsToDeal; ++i) {
        hand[handSize++] = deck[deckIndex++];
    }
}

int calculateHandValue(Card *hand, int handSize) 
{
    int total = 0;
    int aceCount = 0;

    for (int i = 0; i < handSize; ++i) {
        total += hand[i].value;
        if (hand[i].name == "Ace") {
            ++aceCount;
        }
    }

    // Adjust Ace value if total > 21
    while (total > 21 && aceCount > 0) {
        total -= 10;  // Convert Ace from 11 to 1
        --aceCount;
    }

    return total;
}

void displayHand(Card *hand, int handSize) 
{
    for (int i = 0; i < handSize; ++i) {
        cout << hand[i].name << " of " << hand[i].suit << ", ";
    }
    cout << endl;
}

void mcarrillo_init(GLuint stack, int i) 
{
    Image *card_image = &card[i];
    glGenTextures(1, &stack);
    int w = card_image->width;
    int h = card_image->height;
    glBindTexture(GL_TEXTURE_2D, stack);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data = buildAlphaData(&card[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, card_data);
    free(card_data);
}

void mcarrillo_render() 
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, m.felt_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0,      0);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0,      m.yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(m.xres, m.yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(m.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, m.card_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(10,  200);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(100,  200);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(100, 10);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(10, 10);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    for (int i = 0; i < 52; i++) {
        glBindTexture(GL_TEXTURE_2D, m.card_img[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(10,  200);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(100,  200);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(100, 10);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(10, 10);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }   
}

void BJ(int x_res, int y_res) {
    Rect r;

    r.left = x_res / 2;
    r.bot = y_res - (y_res / 4);
    r.center = 1;
    ggprint40(&r, 15, 0x00000000, "Press 'b' to play a game!");

    r.left = x_res / 2;
    r.bot = y_res - (y_res / 2);
    r.center = 1;
    for (int i = 0; i < 2; i++) {
        ggprint16(&r, 20, 0x00000000, dealer[i].c_str());
        mcarrillo_init(d[i], deal[i]);
    }

    r.left = x_res / 2;
    r.bot = y_res - ((3 * y_res) / 4);
    r.center = 1;
    for (int i = 0; i < 2; i++) {
        ggprint16(&r, 20, 0x00000000, mplayer[i].c_str());
        mcarrillo_init(p[i], play[i]);
    }

    r.left = x_res / 2;
    r.bot = 50;
    r.center = 1;
    ggprint16(&r, 0, 0x00000000, val.c_str());
}

void mcarrilloFeature() 
{
    srand(static_cast<unsigned int>(time(0)));

    //mcarrillo_init();
    //mcarrillo_render();
    initializeDeck();
    shuffleDeck();

    // Game variables
    Card playerHand[10], dealerHand[10];
    int playerHandSize = 0, dealerHandSize = 0;
    int deckIndex = 0;

    // Initial deal
    dealCards(playerHand, playerHandSize, 2, deckIndex);
    dealCards(dealerHand, dealerHandSize, 2, deckIndex);
    
    for (int i=0; i<2; i++) {
        printf("%d ", dealerHand[i].value);
        dealer[i] = con(dealerHand[i].value);
    }

    for (int i=0; i<2; i++) {
        printf("%d ", playerHand[i].value);
        mplayer[i] = con(playerHand[i].value);
    }


    // Display initial hands
    cout << "\nPlayer's hand: ";
    displayHand(playerHand, playerHandSize);
    cout << "Dealer's hand: " << dealerHand[0].name << " of " 
        << dealerHand[0].suit << " and [Hidden]" << endl;

    // Player's turn
    char choice;
    bool playerBusted = false; 
    while (true) {
        int playerTotal = calculateHandValue(playerHand, playerHandSize);
        cout << "Player's total: " << playerTotal << endl;

        if (playerTotal > 21) {
            cout << "Player busts! Dealer wins." << endl;
            playerBusted = true;
            //m.win = 2;
            prize -= 15.00;
            break;
        }

        cout << "Hit (h) or Stand (s) or exit(e)? ";
        cin >> choice;

        if (choice == 's') { 
            break;
        }

        if (choice == 'e') { 
            exit(0);
        }

        dealCards(playerHand, playerHandSize, 1, deckIndex);
        cout << "Player's hand: ";
        displayHand(playerHand, playerHandSize);
    }

    // Dealer's turn
    if (!playerBusted) { 
        cout << "\nDealer's hand: ";
        displayHand(dealerHand, dealerHandSize);
        while (calculateHandValue(dealerHand, dealerHandSize) < 17) {
            dealCards(dealerHand, dealerHandSize, 1, deckIndex);
            cout << "Dealer's hand: ";
            displayHand(dealerHand, dealerHandSize);
        }
    }

    // Final results
    if (!playerBusted) { 
        int playerTotal = calculateHandValue(playerHand, playerHandSize);
        int dealerTotal = calculateHandValue(dealerHand, dealerHandSize);
        cout << "\nPlayer's total: " << playerTotal << endl;
        cout << "Dealer's total: " << dealerTotal << endl;

        if (playerBusted) {
            cout << "Dealer wins!" << endl;
            //m.win = 2;
            prize -= 15.00;
        } else if (dealerTotal > 21 || playerTotal > dealerTotal) {
            cout << "Player wins!" << endl;
            //m.win = 1;
            prize += 150.00;
        } else if (dealerTotal > playerTotal) {
            cout << "Dealer wins!" << endl;
            //m.win = 2;
            prize -= 15.00;
        } else {
            cout << "It's a tie!" << endl;
        }

    }
    
   // m.xres = 800;
   // m.yres = 600;
    Rect r;
    r.bot = (m.yres / 2) + 30;
    r.left = m.xres / 2;
    r.center = 1;
    glColor3f(1.0, 1.0, 1.0);
    if (m.win == 1) {
        glBegin(GL_QUADS);
        glColor4ub(0, 0, 0, 200);
            glVertex2i(0, 0);
            glVertex2i(0, m.yres);
            glVertex2i(m.xres, m.yres);
            glVertex2i(m.xres, 0);
        glEnd();

        r.bot = m.yres / 2;
        r.left = m.xres / 2;
        r.center = 1;
        ggprint40(&r, 50, 0x0000ff00, "YOU WIN :)");
        ggprint40(&r, 0, 0x0000ff00, "+$100");
    } else if (m.win == 2) {
        glBegin(GL_QUADS);
        glColor4ub(0, 0, 0, 200);
            glVertex2i(0, 0);
            glVertex2i(0, m.yres);
            glVertex2i(m.xres, m.yres);
            glVertex2i(m.xres, 0);
        glEnd();

        r.bot = m.yres / 2;
        r.left = m.xres / 2;
        r.center = 1;
        ggprint40(&r, 50, 0x00ff0000, "YOU LOST :(");
        ggprint40(&r, 0, 0x00ff0000, "-$50");
    }

    //glDisable(GL_BLEND);
   

}

