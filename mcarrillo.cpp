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
string player[2] = {"None", "None"};
string val = "None";

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

    // card - Ace of Clubs for starter 
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

    // Ace of Clubs 
    m.card_image = &card[0];
    glGenTextures(1, &m.card_img[0]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_cA = buildAlphaData(&card[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_cA);
    free(card_data_cA);

    // 2 of Clubs 
    m.card_image = &card[1];
    glGenTextures(1, &m.card_img[1]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c2 = buildAlphaData(&card[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c2);
    free(card_data_c2);

    // 3 of Clubs 
    m.card_image = &card[2];
    glGenTextures(1, &m.card_img[2]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c3 = buildAlphaData(&card[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c3);
    free(card_data_c3);

    // 4 of Clubs 
    m.card_image = &card[3];
    glGenTextures(1, &m.card_img[3]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c4 = buildAlphaData(&card[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c4);
    free(card_data_c4);

    // 5 of Clubs 
    m.card_image = &card[4];
    glGenTextures(1, &m.card_img[4]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c5 = buildAlphaData(&card[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c5);
    free(card_data_c5);

    // 6 of Clubs 
    m.card_image = &card[5];
    glGenTextures(1, &m.card_img[5]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c6 = buildAlphaData(&card[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c6);
    free(card_data_c6);

    // 7 of Clubs 
    m.card_image = &card[6];
    glGenTextures(1, &m.card_img[6]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c7 = buildAlphaData(&card[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c7);
    free(card_data_c7);

    // 8 of Clubs 
    m.card_image = &card[7];
    glGenTextures(1, &m.card_img[7]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[7]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c8 = buildAlphaData(&card[7]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c8);
    free(card_data_c8);

    // 9 of Clubs 
    m.card_image = &card[8];
    glGenTextures(1, &m.card_img[8]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[8]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c9 = buildAlphaData(&card[8]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c9);
    free(card_data_c9);

    // 10 of Clubs 
    m.card_image = &card[9];
    glGenTextures(1, &m.card_img[9]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[9]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_c10 = buildAlphaData(&card[9]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_c10);
    free(card_data_c10);

    // King of Clubs 
    m.card_image = &card[10];
    glGenTextures(1, &m.card_img[10]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[10]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_cK = buildAlphaData(&card[10]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_cK);
    free(card_data_cK);

    // Queen of Clubs 
    m.card_image = &card[11];
    glGenTextures(1, &m.card_img[11]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[11]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_cQ = buildAlphaData(&card[11]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_cQ);
    free(card_data_cQ);

    // Jack of Clubs 
    m.card_image = &card[12];
    glGenTextures(1, &m.card_img[12]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[12]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_cJ = buildAlphaData(&card[12]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_cJ);
    free(card_data_cJ);

    // Ace of Diamond 
    m.card_image = &card[13];
    glGenTextures(1, &m.card_img[13]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[13]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_dA = buildAlphaData(&card[13]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_dA);
    free(card_data_dA);

    // 2 of Diamond
    m.card_image = &card[14];
    glGenTextures(1, &m.card_img[14]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[14]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d2 = buildAlphaData(&card[14]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d2);
    free(card_data_d2);

    // 3 of Diamond
    m.card_image = &card[15];
    glGenTextures(1, &m.card_img[15]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[15]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d3 = buildAlphaData(&card[15]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d3);
    free(card_data_d3);

    // 4 of Diamond
    m.card_image = &card[16];
    glGenTextures(1, &m.card_img[16]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[16]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d4 = buildAlphaData(&card[16]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d4);
    free(card_data_d4);

    // 5 of Diamond
    m.card_image = &card[17];
    glGenTextures(1, &m.card_img[17]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[17]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d5 = buildAlphaData(&card[17]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d5);
    free(card_data_d5);

    // 6 of Diamond
    m.card_image = &card[18];
    glGenTextures(1, &m.card_img[18]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[18]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d6 = buildAlphaData(&card[18]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d6);
    free(card_data_d6);

    // 7 of Diamond
    m.card_image = &card[19];
    glGenTextures(1, &m.card_img[19]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[19]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d7 = buildAlphaData(&card[19]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d7);
    free(card_data_d7);

    // 8 of Diamond
    m.card_image = &card[20];
    glGenTextures(1, &m.card_img[20]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[20]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d8 = buildAlphaData(&card[20]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d8);
    free(card_data_d8);

    // 9 of Diamond
    m.card_image = &card[21];
    glGenTextures(1, &m.card_img[21]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[21]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d9 = buildAlphaData(&card[21]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d9);
    free(card_data_d9);

    // 10 of Diamond
    m.card_image = &card[22];
    glGenTextures(1, &m.card_img[22]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[22]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_d10 = buildAlphaData(&card[22]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_d10);
    free(card_data_d10);

    // King of Diamond 
    m.card_image = &card[23];
    glGenTextures(1, &m.card_img[23]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[23]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_dK = buildAlphaData(&card[23]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_dK);
    free(card_data_dK);

    // Queen of Diamond 
    m.card_image = &card[24];
    glGenTextures(1, &m.card_img[24]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[24]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_dQ = buildAlphaData(&card[24]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_dQ);
    free(card_data_dQ);

    // Jack of Diamond
    m.card_image = &card[25];
    glGenTextures(1, &m.card_img[25]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[25]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_dJ = buildAlphaData(&card[25]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_dJ);
    free(card_data_dJ);

    // Ace of Hearts
    m.card_image = &card[26];
    glGenTextures(1, &m.card_img[26]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[26]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_hA = buildAlphaData(&card[26]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_hA);
    free(card_data_hA);

    // 2 of Hearts 
    m.card_image = &card[27];
    glGenTextures(1, &m.card_img[27]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[27]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h2 = buildAlphaData(&card[27]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h2);
    free(card_data_h2);

    // 3 of Hearts 
    m.card_image = &card[28];
    glGenTextures(1, &m.card_img[28]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[28]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h3 = buildAlphaData(&card[28]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h3);
    free(card_data_h3);

    // 4 of Hearts 
    m.card_image = &card[29];
    glGenTextures(1, &m.card_img[29]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[29]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h4 = buildAlphaData(&card[29]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h4);
    free(card_data_h4);

    // 5 of Hearts 
    m.card_image = &card[30];
    glGenTextures(1, &m.card_img[30]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[30]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h5 = buildAlphaData(&card[30]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h5);
    free(card_data_h5);

    // 6 of Hearts 
    m.card_image = &card[31];
    glGenTextures(1, &m.card_img[31]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[31]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h6 = buildAlphaData(&card[31]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h6);
    free(card_data_h6);

    // 7 of Hearts 
    m.card_image = &card[32];
    glGenTextures(1, &m.card_img[32]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[32]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h7 = buildAlphaData(&card[32]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h7);
    free(card_data_h7);

    // 8 of Hearts 
    m.card_image = &card[33];
    glGenTextures(1, &m.card_img[33]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[33]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h8 = buildAlphaData(&card[33]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h8);
    free(card_data_h8);

    // 9 of Hearts 
    m.card_image = &card[34];
    glGenTextures(1, &m.card_img[34]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[34]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h9 = buildAlphaData(&card[34]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h9);
    free(card_data_h9);

    // 10 of Hearts 
    m.card_image = &card[35];
    glGenTextures(1, &m.card_img[35]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[35]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_h10 = buildAlphaData(&card[35]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_h10);
    free(card_data_h10);

    // King of Hearts  
    m.card_image = &card[36];
    glGenTextures(1, &m.card_img[36]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[36]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_hK = buildAlphaData(&card[36]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_hK);
    free(card_data_hK);

    // Queen of Hearts  
    m.card_image = &card[37];
    glGenTextures(1, &m.card_img[37]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[37]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_hQ = buildAlphaData(&card[37]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_hQ);
    free(card_data_hQ);

    // Jack of Hearts  
    m.card_image = &card[38];
    glGenTextures(1, &m.card_img[38]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[38]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_hJ = buildAlphaData(&card[38]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_hJ);
    free(card_data_hJ);

    // Ace of Spades 
    m.card_image = &card[39];
    glGenTextures(1, &m.card_img[39]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[39]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_sA = buildAlphaData(&card[39]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_sA);
    free(card_data_sA);

    // 2 of Spades
    m.card_image = &card[40];
    glGenTextures(1, &m.card_img[40]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[40]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s2 = buildAlphaData(&card[40]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s2);
    free(card_data_s2);

    // 3 of Spades
    m.card_image = &card[41];
    glGenTextures(1, &m.card_img[41]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[41]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s3 = buildAlphaData(&card[41]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s3);
    free(card_data_s3);

    // 4 of Spades
    m.card_image = &card[42];
    glGenTextures(1, &m.card_img[42]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[42]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s4 = buildAlphaData(&card[42]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s4);
    free(card_data_s4);

    // 5 of Spades
    m.card_image = &card[43];
    glGenTextures(1, &m.card_img[43]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[43]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s5 = buildAlphaData(&card[43]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s5);
    free(card_data_s5);

    // 6 of Spades
    m.card_image = &card[44];
    glGenTextures(1, &m.card_img[44]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[44]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s6 = buildAlphaData(&card[44]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s6);
    free(card_data_s6);

    // 7 of Spades
    m.card_image = &card[45];
    glGenTextures(1, &m.card_img[45]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[45]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s7 = buildAlphaData(&card[45]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s7);
    free(card_data_s7);

    // 8 of Spades
    m.card_image = &card[46];
    glGenTextures(1, &m.card_img[46]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[46]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s8 = buildAlphaData(&card[46]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s8);
    free(card_data_s8);

    // 9 of Spades
    m.card_image = &card[47];
    glGenTextures(1, &m.card_img[47]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[47]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s9 = buildAlphaData(&card[47]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s9);
    free(card_data_s9);

    // 10 of Spades
    m.card_image = &card[48];
    glGenTextures(1, &m.card_img[48]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[48]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_s10 = buildAlphaData(&card[48]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_s10);
    free(card_data_s10);

    // King of Spades 
    m.card_image = &card[49];
    glGenTextures(1, &m.card_img[49]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[49]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_sK = buildAlphaData(&card[49]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_sK);
    free(card_data_sK);

    // Queen of Spades 
    m.card_image = &card[50];
    glGenTextures(1, &m.card_img[50]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[50]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_sQ = buildAlphaData(&card[50]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_sQ);
    free(card_data_sQ);

    // Jack of Spades
    m.card_image = &card[51];
    glGenTextures(1, &m.card_img[51]);
    w = m.card_image->width;
    h = m.card_image->height;
    glBindTexture(GL_TEXTURE_2D, m.card_img[51]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *card_data_sJ = buildAlphaData(&card[51]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data_sJ);
    free(card_data_sJ);

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
    }

    r.left = x_res / 2;
    r.bot = y_res - ((3 * y_res) / 4);
    r.center = 1;
    for (int i = 0; i < 2; i++) {
        ggprint16(&r, 20, 0x00000000, player[i].c_str());
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
   /* 
    for (int i=0; i<2; i++) {
        printf("%d ", dealerHand[i].value);
        dealer[i] = con(dealerHand[i].value);
    }

    for (int i=0; i<2; i++) {
        printf("%d ", playerHand[i].value);
        player[i] = con(playerHand[i].value);
    }
*/

    // Display initial hands
    cout << "Player's hand: ";
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

