// deck.h
#ifndef DECK_H
#define DECK_H

#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "cards.h" 

class Deck {
public:
    Deck();                 
    void shuffle();         
    Card dealCard();    
    bool isEmpty() const;     

private:
    std::vector<Card> cards;
    std::vector<Card>::size_type currentCardIndex;  
    //int currentCardIndex;    
};

#endif // DECK_H

