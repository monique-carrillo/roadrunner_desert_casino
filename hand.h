#ifndef HAND_H_INCLUDED
#define HAND_H_INCLUDED

#include <iostream>
#include "Card.h"

class Hand
{
    public:
        Hand();
        // Returns the suit of card at index k
        char getSuit(int k);
        // Returns the value of card at index k
        char getValue(int k); 
        // Prints all cards in the hand
        void str(); 
        // Adds a card to the hand
        void addCard(Card card);
        // Returns the total points value of the hand
        int getValue();
        // Displays all cards starting from position (x, y)
        void draw(int x, int y, bool should); 

    private:
        // Holds up to 10 cards in the hand
        Card hand[10] = {}; 
        // Number of cards in the hand
        int count;  
};

// Constructor initializes count to 0
Hand::Hand() : count(0) {}

// Get the suit of the card at index k
char Hand::getSuit(int k) {
    return hand[k].getSuit();
}

// Get the value of the card at index k
char Hand::getValue(int k) {
    return hand[k].getValue();
}

// Print all cards in the hand
void Hand::str() {
    for (int i = 0; i < count; i++) {
        std::cout << hand[i].getSuit() << hand[i].getValue() << " ";
    }
    std::cout << std::endl;
}

// Add a card to the hand
void Hand::addCard(Card card) {
    if (count < 10) { // Ensure we don't exceed hand capacity
        hand[count++] = card;
    }
}

// Get the total point value of the hand
int Hand::getValue() {
    int handValue = 0;
    int aces = 0;

    for (int i = 0; i < count; i++) {
        char val = hand[i].getValue();

        if (val == 'T' || val == 'J' || val == 'Q' || val == 'K') {
            handValue += 10;
        } else if (val == 'A') {
            aces++;
        } else if (val != 'z') {
            handValue += (val - '0'); // Convert char '2' to '9' to int 2 to 9
        }
    }

    // Calculate the value of Aces
    while (aces > 0) {
        if (handValue + 11 > 21) {
            handValue += 1;
        } else {
            handValue += 11;
        }
        aces--;
    }

    return handValue;
}

void Hand::draw(int x, int y, bool shouldShow) {
    for (int i = 0; i < count; i++) {
        char suit = hand[i].getSuit();
        char value = hand[i].getValue();

        // Draw the card's rectangle
        glColor3ub(0, 0, 255);  // Set color to blue for card background
        hand[i].drawPolygon(x, x + 60, y, y + 85);

        if (shouldShow || i == 0) {  // Show suit and value if requested
            glColor3ub(255, 255, 255);  // Set text color to white
           // renderText(x + 5, y + 75, std::string(1, suit) + std::string(1, value));
        }

        x += 75;  // Move the x-position for the next card
    }
}

/*
// Display all cards in the hand
void Hand::draw(int x, int y, bool shouldShow) {
    std::cout << "Drawing hand at position (" << x << ", " << y << "):" << std::endl;

    for (int i = 0; i < count; i++) {
        char suit = hand[i].getSuit();
        char value = hand[i].getValue();

        if (shouldShow || i == 0) {
            std::cout << "Card " << i + 1 << ": " << suit << value << std::endl;
        } else {
            std::cout << "Card " << i + 1 << ": Hidden" << std::endl;
        }
    }
}
*/
#endif // HAND_H_INCLUDED

