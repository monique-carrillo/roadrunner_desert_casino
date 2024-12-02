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

class Global {
    public:
        float money;
        int xres, yres;
        int paused;
        int gamemode;
        int done;
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
            xres = 800;
            yres = 600;
        }
} m;

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


// Card structure
struct Card {
    int value;  // Card value (1-11, Ace can be 1 or 11)
    string suit;  // Card suit (e.g., Hearts, Spades, etc.)
    string name;  // Card name (e.g., 2, King, Ace)
};

// Deck of 52 cards
Card deck[52];

// Initialize deck
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

// Shuffle deck
void shuffleDeck() 
{
    for (int i = 0; i < 52; ++i) {
        int randomIndex = rand() % 52;
        swap(deck[i], deck[randomIndex]);
    }
}

// Deal cards to a player
void dealCards(Card *hand, int &handSize, int cardsToDeal, int &deckIndex) 
{
    for (int i = 0; i < cardsToDeal; ++i) {
        hand[handSize++] = deck[deckIndex++];
    }
}

// Calculate hand total
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

// Display hand
void displayHand(Card *hand, int handSize) 
{
    for (int i = 0; i < handSize; ++i) {
        cout << hand[i].name << " of " << hand[i].suit << ", ";
    }
    cout << endl;
}

void mcarrillo_init(void) 
{
    glViewport(0, 0, m.xres, m.yres);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, m.xres, 0, m.yres, -1, 1);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);

    // felt
    m.felt_image = &pic[0];
    glGenTextures(1, &m.felt_texture);
    int w = m.felt_image->width;
    int h = m.felt_image->height;
    glBindTexture(GL_TEXTURE_2D, m.felt_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 m.felt_image->data);

    // card - Ace of Clubs 
    m.card_image = &pic[1];
    glGenTextures(1, &m.card_texture);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data = buildAlphaData(&pic[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data);
    free(card_data);

    m.card_image = &card[0];
    glGenTextures(1, &m.card_img[0]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data1 = buildAlphaData(&card[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data1);
    free(card_data1);

    m.card_image = &card[1];
    glGenTextures(1, &m.card_img[1]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data2 = buildAlphaData(&card[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data2);
    free(card_data2);


}

void mcarrillo_render() 
{
    mcarrillo_init();
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

        glBindTexture(GL_TEXTURE_2D, m.card_img[0]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(10,  200);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(100,  200);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(100, 10);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(10, 10);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, m.card_img[1]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(10,  200);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(100,  200);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(100, 10);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(10, 10);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);

}

void mcarrilloFeature() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m.felt_texture);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);


    float xOffset = -0.95f;
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, m.card_img[i]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(xOffset, -0.3f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(xOffset + 0.2f, -0.3f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(xOffset + 0.2f, 0.3f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(xOffset, 0.3f);
        glEnd();
        xOffset += 0.25f;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    Rect r;
    r.bot = 290;
    r.left = 20;
    r.center = 0;
    glColor3f(1.0, 1.0, 1.0);

    ggprint16(&r, 16, 0x00ffffff, "BlackJack");

    srand(static_cast<unsigned int>(time(0)));

    initializeDeck();
    shuffleDeck();

    // Game variables
    Card playerHand[10], dealerHand[10];
    int playerHandSize = 0, dealerHandSize = 0;
    int deckIndex = 0;

    // Initial deal
    dealCards(playerHand, playerHandSize, 2, deckIndex);
    dealCards(dealerHand, dealerHandSize, 2, deckIndex);

    // Display initial hands
    cout << "Player's hand: ";
    displayHand(playerHand, playerHandSize);
    cout << "Dealer's hand: " << dealerHand[0].name << " of " 
         << dealerHand[0].suit << " and [Hidden]" << endl;

    // Player's turn
    char choice;
    while (true) {
        int playerTotal = calculateHandValue(playerHand, playerHandSize);
        cout << "Player's total: " << playerTotal << endl;

        if (playerTotal > 21) {
            cout << "Player busts! Dealer wins." << endl;
            break;
        }

        cout << "Hit (h) or Stand (s)? ";
        cin >> choice;

        if (choice == 's') break;

        dealCards(playerHand, playerHandSize, 1, deckIndex);
        cout << "Player's hand: ";
        displayHand(playerHand, playerHandSize);
    }

    // Dealer's turn
    cout << "\nDealer's hand: ";
    displayHand(dealerHand, dealerHandSize);
    while (calculateHandValue(dealerHand, dealerHandSize) < 17) {
        dealCards(dealerHand, dealerHandSize, 1, deckIndex);
        cout << "Dealer's hand: ";
        displayHand(dealerHand, dealerHandSize);
    }

    // Final results
    int playerTotal = calculateHandValue(playerHand, playerHandSize);
    int dealerTotal = calculateHandValue(dealerHand, dealerHandSize);
    cout << "\nPlayer's total: " << playerTotal << endl;
    cout << "Dealer's total: " << dealerTotal << endl;

    if (dealerTotal > 21 || playerTotal > dealerTotal) {
        cout << "Player wins!" << endl;
    } else if (dealerTotal > playerTotal) {
        cout << "Dealer wins!" << endl;
    } else {
        cout << "It's a tie!" << endl;
    }

}

