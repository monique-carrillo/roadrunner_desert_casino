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

string dealer[5] = {"None","None","None","None","None"};
string mplayer[5] = {"None","None","None","None","None"};
string val = "None";

int deal[5] = {0, 1, 2, 3, 4};
int play[5] = {0, 1, 2, 3, 4};
int mapvals[52] = {0,1,2,3,4,5,6,7,8,9,10,
                   11,12,13,14,15,16,17,18,19,20,
                   21,22,23,24,25,26,27,28,29,30,
                   31,32,33,34,35,36,37,38,39,40,
                   41,42,43,44,45,46,47,48,49,50,51};

int hit = 0;
float prize = 0.00;

class Global {
    public:
        float money;
        int xres, yres;
        GLuint bgTexture;
        Image *felt_image;
        GLuint felt_texture;
        Image *card_image;
        GLuint card_textures[52];
        GLuint card_img[52];
        Global() {
            xres = 800;
            yres = 600;
        }
} m;

GLuint d[5];
GLuint p[5];

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
    //int suit;
    string suit;  
    string name;
    int texmap;
    //int num;
};


Card deck[52];

void initializeDeck() 
{
    //int suits[4]; //= {"Hearts", "Diamonds", "Clubs", "Spades"};
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string names[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", 
        "Queen", "King", "Ace"};
    // Ace initially 11
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};  

    int index = 0;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            deck[index].value = values[rank];
            deck[index].suit = suits[suit]; 
            deck[index].name = names[rank]; 
            //deck[index].texmap = values[index];
            deck[index].texmap = mapvals[index];
            index++;
        }
    }
}

string con(int card_pos) 
{
    switch (card_pos) {
        case 1: return "Ace of Clubs";
        case 2: return "2 of Clubs";
        case 3: return "3 of Clubs";
        case 4: return "4 of Clubs";
        case 5: return "5 of Clubs";
        case 6: return "6 of Clubs";
        case 7: return "7 of Clubs";
        case 8: return "8 of Clubs";
        case 9: return "9 of Clubs";
        case 10: return "10 of Clubs";
        case 11: return "J of Clubs";
        case 12: return "Q of Clubs";
        case 13: return "K of Clubs";
        case 14: return "Ace of Diamonds";
        case 15: return "2 of Diamonds";
        case 16: return "3 of Diamonds";
        case 17: return "4 of Diamonds";
        case 18: return "5 of Diamonds";
        case 19: return "6 of Diamonds";
        case 20: return "7 of Diamonds";
        case 21: return "8 of Diamonds";
        case 22: return "9 of Diamonds";
        case 23: return "10 of Diamonds";
        case 24: return "J of Diamonds";
        case 25: return "Q of Diamonds";
        case 26: return "K of Diamonds";
        case 27: return "Ace of Hearts";
        case 28: return "2 of Hearts";
        case 29: return "3 of Hearts";
        case 30: return "4 of Hearts";
        case 31: return "5 of Hearts";
        case 32: return "6 of Hearts";
        case 33: return "7 of Hearts";
        case 34: return "8 of Hearts";
        case 35: return "9 of Hearts";
        case 36: return "10 of Hearts";
        case 37: return "J of Hearts";
        case 38: return "Q of Hearts";
        case 39: return "K of Hearts";
        case 40: return "Ace of Spades";
        case 41: return "2 of Spades";
        case 42: return "3 of Spades";
        case 43: return "4 of Spades";
        case 44: return "5 of Spades";
        case 45: return "6 of Spades";
        case 46: return "7 of Spades";
        case 47: return "8 of Spades";
        case 48: return "9 of Spades";
        case 49: return "10 of Spades";
        case 50: return "J of Spades";
        case 51: return "Q of Spades";
        case 52: return "K of Spades";
        default: return "Unknown Card";
    }
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
    while (total > 21 && aceCount > 0) {
        total -= 10;  // Convert Ace from 11 to 1
        --aceCount;
    }

    return total;
}

void sort(Card *hand, int size_hand) 
{
    if (size_hand == 5) {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4-i; j++) {
                if (hand[j].value > hand[j+1].value) {
                    swap(hand[j], hand[j+1]);
                }
            }
        }
    } else if (size_hand == 2) {
        for (int i=0; i<1; i++) {
            if (hand[i+1].value < hand[i].value) {
                swap(hand[i], hand[i+1]);
            }
        }
    }
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

void BJ(int x_res, int y_res) 
{
    Rect r;

    r.left = x_res / 2;
    r.bot = y_res - (y_res / 6);
    r.center = 1;
    ggprint40(&r, 15, 0x00000000, "Press 'b' to play a game!");
    r.bot = y_res - (y_res / 4);
    ggprint40(&r, 15, 0x00000000, "Press 'h' to grab a card!");
    r.bot = y_res - (y_res / 3);
    ggprint40(&r, 15, 0x00000000, "Press 's' to stand!");
}

void mcarrilloFeature() 
{
    srand(static_cast<unsigned int>(time(0)));
    
    //mcarrillo_init();
    //mcarrillo_render();
    initializeDeck();
    shuffleDeck();

    // Game variables
    Card playerHand[5], dealerHand[5];
    int playerHandSize = 0, dealerHandSize = 0;
    int deckIndex = 0;

    // Initial deal
    dealCards(playerHand, playerHandSize, 2, deckIndex);
    dealCards(dealerHand, dealerHandSize, 2, deckIndex);
  
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
            prize -= 50.00;
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
            //prize -= 50.00;
        } else if (dealerTotal > 21 || playerTotal > dealerTotal) {
            cout << "Player wins!" << endl;
            prize += 150.00;
        } else if (dealerTotal > playerTotal) {
            cout << "Dealer wins!" << endl;
            prize -= 50.00;
        } else {
            cout << "It's a tie!" << endl;
        }

    }
}

