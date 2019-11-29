#ifndef HAND_H
#define HAND_H

#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
//#include <deque>
#include "Card.hpp"

using namespace std;


class Hand
{
public:
    Hand()
    {

    }

    /*
        Adds a card to the hand.
    */
    void addCard(Card newCard)
    {
        inHand.push_back(newCard);
        // might delete following
            handValue+=newCard.getValue();
            count++;
    }

    /*
        Returns the value of the hand.
    */
    int value()
    {
        int value = 0; 
        for (auto card : inHand)
        {
            value += card.value;
        }
        return value;
    }

    /*
        Returns the number of cards in the hand.
    */
    int size()
    {
        return inHand.size();
    }

    /*
        Returns the card at the given index.
     */
    Card getCard(int index)
    {
        return inHand[index];
    }

    /*
        Removes all of the cards from the hand.
    */
    void clear()
    {
        inHand.clear();
    }













    bool is21()
    {
        int total = 0;
        for (auto it = inHand.begin(); it != inHand.end(); ++it) 
        {
            total = total + it->value;
        }
        return total==21;
    }
    int getTotal()
    {
        return handValue;
    }
    bool isBust()
    {
        //Returns true if all possible scores are greater than 21
        return false;
    }
    Card removeCard(Card C)
    {
        // TODO
        return C;
    }
    void checkHand()
    {
        if ( handValue > 21 && aceInHand() ) 
            handValue -= 10; // whole hand value
    }
    // if there is an Ace in the Hand, value will change from 11 to 1 for Ace
    bool aceInHand()
    {
        bool result = false;
        Card C;
        for(int i=0; i < inHand.size(); i++)
        {
            C = inHand[i];
            if(C.getValue() == 11)
            {
                C.setValue(1); // card 1
                result = true;
            }
        }
        return result;
    }
    void printHand()
    {
      for(auto card: inHand)
      {
        card.display();
      }
    }

    std::string printAllHand()
    {
        std::string result;
        for(auto card: inHand)
        {   
            result += card.printCard(); // utsav caught this
            result += "\n";
        }
        return result;
    }

    std::vector<Card> inHand; //deque<Card>inHand;
    

    int handValue = 0;
    int count = 0;
    bool playing = false;
};


#endif
