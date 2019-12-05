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

   /**
   * Displays the card's rank and suit
   */
  void display() 
  {
     std::cout << rank_ << " of " << suit_ << std::endl;
  }

   /**
   * Returns a card's info in a string format
   */
  std::string printCard()
  {
    std::string result;
    result += rank_;
    result += " of ";
    result += suit_;
    result += "  \n";
    return result;
  }

   /**
   * Changes the boolean isFaceUp from false to true
   */
  void flip()
  {
    isFaceUp =!isFaceUp;
  }
  
   /**
   * Sets the value of the card
   */
  void setValue(int val)
  {
     value = val;
  }
  
   /**
   * Returns the value of the card
   */
  int getValue()
  {
    return value;
  }

  /**
   * Returns the rank of the card.
   */
  char getRank()
  {
    return rank_;
  }
  
   /**
   * Returns the suit of the card.
   */
  char getSuit()
  {
    return suit_;
  }
  
  /**
   * Sets the value, rank, and suit of card.
   */
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

