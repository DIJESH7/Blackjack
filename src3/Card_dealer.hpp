#ifndef CARD_H
#define CARD_H

#include <iostream>

using namespace std;


class Card
{
public:
  Card()
  {

  }
  /*
  Card(Rank rank, Suit suit)
  {
      rank_ = rank;
      suit_ = suit;
      isFaceUp = false;
  }


  void display() 
  {
      const char * rankText[] = { " ", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
      const char * suitText[] = { "Spades", "Hearts", "Clubs", "Diamonds" };
      std::cout << rankText[rank_] << " of " << suitText[suit_] << std::endl;
  }
  */

  void display() 
  {
     std::cout << rank_ << " of " << suit_ << std::endl;
  }

  void flip()
  {
    isFaceUp =!isFaceUp;
  }
  void setValue(int val)
  {
     value = val;
  }
  /*
  void setRank(Rank rank)
  {
     rank_ = rank;
  }
  void setSuit(Suit suit)
  {
     suit_ = suit;
  }
  int getValue() const
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
  */

  void setRank(int r)
  {
    switch(r)
    {
      case 1: rank_ = 'A'; break;
      case 2: rank_ = '2'; break;
      case 3: rank_ = '3'; break;
      case 4: rank_ = '4'; break;
      case 5: rank_ = '5'; break;
      case 6: rank_ = '6'; break;
      case 7: rank_ = '7'; break;
      case 8: rank_ = '8'; break;
      case 9: rank_ = '9'; break;
      case 10: rank_ = 'T'; break;
      case 11: rank_ = 'J'; break;
      case 12: rank_ = 'Q'; break;
      case 13: rank_ = 'K'; break;
    }
  }
  void setSuit(int s)
  {
    switch(s)
    {
      case 0: suit_ = 'S'; break;
      case 1: suit_ = 'H'; break;
      case 2: suit_ = 'C'; break;
      case 3: suit_ = 'D'; break;
    }
  }
  int getValue()
  {
    return value;
  }
  char getRank()
  {
    return rank_;
  }
  char getSuit()
  {
    return suit_;
  }

  void setInfo(int v, char r, char s)
  {
    value = v;
    rank_ = r;
    suit_ = s;
    isFaceUp = false;
  }

  //Rank rank_;
  //Suit suit_;
  char rank_;
  char suit_;
  int value;
  bool isFaceUp;

private:
};


#endif

