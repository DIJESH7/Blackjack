#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include "../include/Hand.h"

using namespace std;

Hand::Hand()
{
   handValue = 0;
   count = 0;
}

Card Hand::getCard(Deck D)
{
    Card result;
    result = D.deal();
    inHand.push_back(result);
    handValue+=result.getValue();
    //reduceHand(result);
    count++;
    return result;  
}

int Hand::getTotalValue()
{
    return handValue;
    /*
       int hard = getHardValue();
       int soft = getSoftValue();
       if (hard > soft && hard <= 21)
       return hard;
       else if (soft > hard && soft <= 21)
       return soft;
       else
       return hard;
       */
}

bool Hand::isBust()
{
    //Returns true if all possible scores are greater than 21

    return false;
}

Card Hand::removeCard(Card C)
{
    // TODO
    return C;
}

void Hand::isSoftHand(Card C)
{
    if ( handValue > 21 && aceInHand() )
        handValue -= 10;
}


// if there is an Ace in the Hand, value will change from 11 to 1 for Ace
bool Hand::aceInHand()
{
    bool result = false;
    Card C;
    for(int i=0; i < inHand.size(); i++)
    {
        C = inHand[i];
        if(C.getValue() == 11)
        {
            C.setValue(1);
            result = true;
        }
    }
    return result;
}

int Hand::getHardValue()
{
    //TODO gets hand value with all aces==1

    return 0;
}

int Hand::getSoftValue()
{
    //TODO gets hand value with first ace==1
    // if no aces, return same value as getHardValue
    return 0;
}


