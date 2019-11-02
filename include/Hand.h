#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Card.hpp"

using namespace std;

class Hand
{
public:
    Hand()
    {
        handValue = 0; // total value of cards
        count = 0;  // number of cards
    }

    void addToHand(Card C)
    {
        inHand.push_back(C);
        handValue+=C.getValue();


        count++;
    }

    int getTotalValue()
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

    void setBet(int b)
    {
        bet = b;
    }

    void doubleBet()
    {
        bet = bet*2;
    }



private:
    vector<Card>inHand;
    int handValue;
    int count;
    int bet;
};

