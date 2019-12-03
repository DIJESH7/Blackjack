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

  Card(char rank, char suit)
  {
      rank_ = rank;
      suit_ = suit;
      isFaceUp = false;
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
    result += "  \n";
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
  
  char rank_;
  char suit_;
  int value;
  bool isFaceUp;

private:

};


#endif

