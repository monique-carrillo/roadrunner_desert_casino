// hand.h
#ifndef HAND_H
#define HAND_H

#include <vector>
#include "cards.h"

class Hand {
public:
    Hand();                     
    void addCard(const Card& card); 
    int getValue() const;       
    bool isBust() const;        

private:
    std::vector<Card> cards;    
};

#endif // HAND_H

