#ifndef DECK_H
#define DECK_H

#include <iostream>
#include <string>
#include <vector>

#include "Card.h"


class Deck
{
public:
    Deck();
    void build(); //create all cards in deck
    void shuffle();	// shuffle algorithm
    Card deal(); // deal card and reduce deck
    
private:
    //Card cards[312]; //will hold 312 cards
    vector <Card> cards_;
    int currCard; // used as index interator
};

#endif