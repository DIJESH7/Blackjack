#ifndef CARD_H
#define CARD_H

#include <iostream>
//#include <string>
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

  Card()
  {

  }

  Card(Rank rank, Suit suit)
  {
      rank_ = rank;
      suit_ = suit;
  }

  // prints the card info in a string
  void display() 
  {
      const char * rankText[] = { " ", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

      const char * suitText[] = { "Spades", "Hearts", "Clubs", "Diamonds" };
      
      cout << rankText[rank_] << " of " << suitText[suit_] << endl;
  }

  void setValue(int val)
  {
     value = val;
  }

  void setRank(Rank rank)
  {
     rank_ = rank;
  }

  void setSuit(Suit suit)
  {
     suit_ = suit;
  }

  int getValue()
  {
     return value;
  }

  Rank getRank()
  {
     return rank_;
  }

  Suit getSuit()
  {
     return suit_;
  }

private:
  Rank rank_;
  Suit suit_;
  int value;
  
};

#endif

