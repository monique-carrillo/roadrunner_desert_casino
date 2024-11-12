//Monique Carrillo
// 10/8/24
// Updated: 11/08/2024
/*
#include <stdlib.h>
#include <sstream>
#include <string>
#include "cards.h"
#include "hand.h"
#include "deck.h"

Deck deck;
Hand dealer;
Hand player;

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            break;

        case 'd':
        case 'D':
            if(!in_play){
                lost = false;
                winner = false;
                in_play = true;
                deck = Deck();
                deck.shuffle();
                dealer = Hand();
                player = Hand();
                //give two cards to each player
                dealer.addCard(deck.dealCard());
                player.addCard(deck.dealCard());
                dealer.addCard(deck.dealCard());
                player.addCard(deck.dealCard());

                cout << "DEALER" << endl;
                dealer.str();
                cout << endl;
                cout << "PLAYER" << endl;
                player.str();
                cout << endl;
            } else {
                contDealer++;
                lost = true;
                in_play = false;
            }


            break;

        case 'h':
        case 'H':
            if(in_play){
                if(player.getValue() <= 21){
                    player.addCard(deck.dealCard());
                    if(player.getValue() > 21){
                        player.str();
                        cout << endl;
                        cout << "YOU LOSE!" << endl;
                        contDealer++;
                        lost = true;
                        in_play = false;
                    }
                } else {
                    player.str();
                    cout << endl;
                    cout << "YOU LOSE!" << endl;
                    contDealer++;
                    lost = true;
                    in_play = false;
                }
            }
            break;

        case 's':
        case 'S':
            if(!in_play){
                cout << "YOU LOST!" << endl;
            } else {
                while(dealer.getValue() < 17){
                    dealer.addCard(deck.dealCard());
                }
                if(dealer.getValue() > 21){
                    contPlayer++;
                    winner = true;
                    cout << "WINNER!" << endl;
                    cout << "DEALER OVER 21!" << endl;
                } else if(player.getValue() > dealer.getValue()){
                    winner = true;
                    cout << "WINNER!" << endl;
                    contPlayer++;
                } else {
                    contDealer++;
                    lost = true;
                    cout << "YOU LOSE!" << endl;
                }
                  cout << "DEALER"<< dealer.getValue() << endl;
                    dealer.str();
                    cout << endl;
                    cout << "PLAYER" << player.getValue() << endl;
                    player.str();
                    cout << endl;
                in_play = false;
            }
            break;
    }

*/


#include "fonts.h"

int monique_show = 0;

void show_my_feature(int x, int y)
{
    //draw a rectangle
    //show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x0000ff00, "Monique's Feature");

}
