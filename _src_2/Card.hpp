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

  void display() 
  {
     std::cout << rank_ << " of " << suit_ << std::endl;
  }

  std::string printCard()
  {
    std::string result;
    result += rank_;
    result += " of ";
    result += suit_;
    return result;
  }

  void flip()
  {
    isFaceUp =!isFaceUp;
  }
  void setValue(int val)
  {
     value = val;
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
  
  char rank_;
  char suit_;
  int value;
  bool isFaceUp;

private:

};


#endif

