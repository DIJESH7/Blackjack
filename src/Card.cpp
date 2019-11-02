#include <iostream>
//#include <string>

#include "../include/Card.h"

using namespace std;

Card::Card()
{

}

Card::Card(Rank rank, Suit suit)
{
    rank_ = rank;
    suit_ = suit;
}


void Card::display()
{
    const char * rankText[] = { " ", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

    const char * suitText[] = { "Spades", "Hearts", "Clubs", "Diamonds" };

    cout << rankText[rank_] << " of " << suitText[suit_] << endl;
}

void Card::setValue(int val)
{
    value = val;
}

void Card::setRank(Rank rank)
{
    rank_ = rank;
}

void Card::setSuit(Suit suit)
{
    suit_ = suit;
}

int Card::getValue()
{
    return value;
}

Rank Card::getRank()
{
    return rank_;
}

Suit Card::getSuit()
{
    return suit_;
}





