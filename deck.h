#ifndef DECK_H_INCLUDED
#define DECK_H_INCLUDED

#include <iostream>
#include <string>
#include <cstdlib>  // For rand() function
#include "cards.h"

using namespace std;

class Deck
{
    public:
        // Constructor
        Deck();

        // Other Methods
        char getSuit(int k);
        char getValue(int k);
        void shuffle();
        Card dealCard();
        string str();

    private:
        // Attributes
        Card d[52] = {
            Card('C','A'), Card('C','2'), Card('C','3'), Card('C','4'), 
            Card('C','5'), Card('C','6'), Card('C','7'), Card('C','8'), 
            Card('C','9'), Card('C','T'), Card('C','J'), Card('C','Q'), 
            Card('C','K'), Card('S','A'), Card('S','2'), Card('S','3'), 
            Card('S','4'), Card('S','5'), Card('S','6'), Card('S','7'),
            Card('S','8'), Card('S','9'), Card('S','T'), Card('S','J'), 
            Card('S','Q'), Card('S','K'), Card('H','A'), Card('H','2'), 
            Card('H','3'), Card('H','4'), Card('H','5'), Card('H','6'), 
            Card('H','7'), Card('H','8'), Card('H','9'), Card('H','T'), 
            Card('H','J'), Card('H','Q'), Card('H','K'), Card('D','A'),
            Card('D','2'), Card('D','3'), Card('D','4'), Card('D','5'), 
            Card('D','6'), Card('D','7'), Card('D','8'), Card('D','9'), 
            Card('D','T'), Card('D','J'), Card('D','Q'), Card('D','K')
        };

        int position;
};

// Constructor initializes position to 0
Deck::Deck() : position(0) {}

// Get suit of card at index k
char Deck::getSuit(int k) {
    return d[k].getSuit();
}

// Get value of card at index k
char Deck::getValue(int k) {
    return d[k].getValue();
}

// Shuffle the deck by swapping random pairs of cards
void Deck::shuffle() {
    for(int i = 0; i < 52; i++) {
        int sub1 = rand() % 52;
        int sub2 = rand() % 52;
        Card temp = d[sub1];
        d[sub1] = d[sub2];
        d[sub2] = temp;
    }
    position = 0;  // Reset position after shuffling
}

// Deal the next card in the deck
Card Deck::dealCard() {
    if (position >= 52) {  // If all cards are dealt, reshuffle
        shuffle();
        position = 0;
    }
    return d[position++];
}

// Return a string representation of the deck
string Deck::str() {
    string deckStr;
    for(int i = 0; i < 52; i++) {
        deckStr += d[i].getSuit();
        deckStr += d[i].getValue();
        deckStr += " ";
    }
    return deckStr;
}

#endif // DECK_H_INCLUDED

