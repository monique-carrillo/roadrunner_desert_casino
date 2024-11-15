// Monique Carrillo
// 10/8/24
// Updated: 11/08/2024

#include <stdlib.h>
#include <sstream>
#include <string>
#include <string>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include "cards.h"
#include "hand.h"
#include "deck.h"
#include "mcarrillo.h"
#include <iostream>

Deck deck;
Hand playerHand;
Hand dealerHand;
bool playerTurn = true;
bool gameOver = false;
int monique_show = 0;

void mcarrillo_init() {
    deck.shuffle();
    playerHand.addCard(deck.dealCard());
    playerHand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());
    gameOver = false;
    playerTurn = true;
}

void mcarrillo_update() {
    if (playerTurn) {
        if (playerHand.isBust()) {
            std::cout << "Player busts!\n";
            gameOver = true;
            playerTurn = false;
        }
    } else {
        if (dealerHand.getValue() < 17) {
            std::cout << "Draw Card\n";
            dealerHand.addCard(deck.dealCard());
        } else {
            gameOver = true;
            std::cout << "Dealer stands.\n";
        }
    }
}

void mcarrillo_render() 
{
    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << "Player's Hand: " << playerHand.getValue() << std::endl;
    std::cout << "Dealer's Hand: " << dealerHand.getValue() << std::endl;
    
    if (gameOver) {
        if (playerHand.isBust()) {
            std::cout << "Game Over: Player Busts!\n";
        } else if (dealerHand.isBust() || 
                playerHand.getValue() > dealerHand.getValue()) {
            std::cout << "Game Over: Player Wins!\n";
        } else {
            std::cout << "Game Over: Dealer Wins!\n";
        }
    }

    glFlush();
}

void mcarrillo_input(int key) 
{
    if (playerTurn && !gameOver) {
        if (key == XK_h) { 
            // Hit
            playerHand.addCard(deck.dealCard());
        } else if (key == XK_s) { 
            // Stand
            playerTurn = false;
        }
    }
}

void mcarrilloFeature() 
{
    mcarrillo_init();   
    mcarrillo_update();  
    mcarrillo_render();  
}

Hand::Hand() {
}

void Hand::addCard(const Card& card) 
{
    cards.push_back(card);
}

int Hand::getValue() const 
{
    int value = 0;
    int aceCount = 0;
    for (const Card& card : cards) {
        int rank = card.getRank(); 
        if (rank > 10) {
            value += 10;  
        } else if (rank == 1) {
            value += 11; 
            aceCount++;
        } else {
            value += rank; 
        }
    }
    while (value > 21 && aceCount > 0) {
        value -= 10;  
        aceCount--;
    }
    return value;
}

bool Hand::isBust() const 
{
    return getValue() > 21;
}

Deck::Deck() : currentCardIndex(0) {
    // Initialize the deck with 52 cards (standard deck with ranks/suits)
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            cards.push_back(Card(suit, rank));
        }
    }
    shuffle();
}

void Deck::shuffle() {
    currentCardIndex = 0;
    std::srand(static_cast<unsigned int>(std::time(0)));
    std::random_shuffle(cards.begin(), cards.end());
    currentCardIndex = 0;
}

Card Deck::dealCard() {
    if (currentCardIndex < cards.size()) {
        return cards[currentCardIndex++];
    } else {
        throw std::out_of_range("No cards left in the deck");
    }
}

bool Deck::isEmpty() const {
    return currentCardIndex >= cards.size();
}

Card::Card(int suit, int rank) : suit(suit), rank(rank) {}

int Card::getRank() const {
    return rank;
}

int Card::getSuit() const {
    return suit;
}

std::string Card::toString() const {
    std::string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::string ranks[] = {"", "Ace", "2", "3", "4", "5", "6", "7", "8", "9", 
        "10", "Jack", "Queen", "King"};
    return ranks[rank] + " of " + suits[suit];
}



