#ifndef CARD_H
#define CARD_H

//#include <iostream>
#include <string>

#include "enums.h"

using namespace std;

class Card
{
public:
    Card();
    Card(Rank rank, Suit suit);
    //Card Card(Integer, Integer);
    
    void display();
    void setValue(int val);
    void setRank(Rank rank);
    void setSuit(Suit suit);
    int getValue();
    Rank getRank(); // return value of card "2", "J", "A"
    Suit getSuit(); // return suit of card "hearts" "spades"
    
private:
    Rank rank_;
    Suit suit_;
    int value;
};


#endif



