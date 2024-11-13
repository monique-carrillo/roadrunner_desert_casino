// cards.h
#ifndef CARDS_H
#define CARDS_H

#include <string>

class Card {
public:
    Card(int suit, int rank);  
    int getRank() const;       
    int getSuit() const;       
    std::string toString() const; 

private:
    int suit; 
    int rank; 
};

#endif // CARDS_H

