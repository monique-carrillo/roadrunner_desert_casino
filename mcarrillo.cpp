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

using namespace std;
int monique_show = 0;

// Card structure
struct Card {
    int value;  // Card value (1-11, Ace can be 1 or 11)
    string suit;  // Card suit (e.g., Hearts, Spades, etc.)
    string name;  // Card name (e.g., 2, King, Ace)
};

// Deck of 52 cards
Card deck[52];

// Initialize deck
void initializeDeck() {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string names[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};  // Ace initially 11

    int index = 0;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            deck[index++] = {values[rank], suits[suit], names[rank]};
        }
    }
}

// Shuffle deck
void shuffleDeck() {
    for (int i = 0; i < 52; ++i) {
        int randomIndex = rand() % 52;
        swap(deck[i], deck[randomIndex]);
    }
}

// Deal cards to a player
void dealCards(Card *hand, int &handSize, int cardsToDeal, int &deckIndex) {
    for (int i = 0; i < cardsToDeal; ++i) {
        hand[handSize++] = deck[deckIndex++];
    }
}

// Calculate hand total
int calculateHandValue(Card *hand, int handSize) {
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
void displayHand(Card *hand, int handSize) {
    for (int i = 0; i < handSize; ++i) {
        cout << hand[i].name << " of " << hand[i].suit << ", ";
    }
    cout << endl;
}

void mcarrilloFeature() {
    srand(static_cast<unsigned int>(time(0)));

    // Initialize deck
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
    cout << "Dealer's hand: " << dealerHand[0].name << " of " << dealerHand[0].suit << " and [Hidden]" << endl;

    // Player's turn
    char choice;
    while (true) {
        int playerTotal = calculateHandValue(playerHand, playerHandSize);
        cout << "Player's total: " << playerTotal << endl;

        if (playerTotal > 21) {
            cout << "Player busts! Dealer wins." << endl;
            exit(0);
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

