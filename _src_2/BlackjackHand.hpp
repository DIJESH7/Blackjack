#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
//#include <deque>
#include "Hand.hpp"
#include "Card.hpp"

using namespace std;

class Card;

class BlackjackHand : public Hand
{
public:
    virtual ~BlackjackHand()
    {
        //delete doubleDownCard_;
    }

    /*
        Returns the hand value. 
        Aces count as 1 or 11 depending on 
        whether the hand is soft or not.
    */
    virtual int blackjackValue()
    {
        int value = 0;
        for (auto card : inHand)
        {
            value += card.value;
        }
        if (isSoft())
        {
            value += 10;
        }
        return value;
    }

    /*
        Returns whether or not the hand is soft.
    */
    virtual bool isSoft()
    {
        return hasAce() && value() < 12;
    }

    /*
        Places a bet on the hand.
    */
    virtual void placeBet(double bet)
    {
        this->bet_ = bet;
    }

    /*
        Returns the bet placed on the hand.
    */
    virtual double bet()
    {
        return bet_;
    }

    /*
        Sets the hand as split.
    */
    virtual void setSplitPairs()
    {
        splitPairs_ = true;
    }

    /*
        Returns whether or not the hand has been split.
    */
    virtual bool splitPairs()
    {
        return splitPairs_;
    }

    /*
        Sets the hand as doubled down.
    */
    virtual void setDoubleDown()
    {
        doubleDown_ = true;
    }

    /*
        Returns whether or not the hand has been doubled down.
    */
    virtual bool doubleDown()
    {
        return doubleDown_;
    }

    /*
        Adds a single card to the hand after doubling down.
    */
    virtual void addDoubleDownCard(Card card)
    {
        doubleDownCard_ = card;
        addCard(card);
    }

    /*
        Returns the card added to the hand after doubling down.
    */
    virtual Card doubleDownCard()
    {
        return doubleDownCard_;
    }

    std::string printAllHand2()
    {
        std::string result;
        for(auto card: inHand)
        {   
            result += card.printCard(); // utsav caught this
            card.display();
            result += "\n";
        }
        return result;
    }

    virtual Card getCard2(int index)
    {
        return inHand[index];
    }


private:
    /*
        Returns true if the hand has an Ace.
    */
    bool hasAce()
    {
        for (auto card : inHand)
        {
            if (card.value == 1)
            {
                return true;
            }
        }
        return false;
    }

    double bet_ = 0;
    bool splitPairs_ = false;
    bool doubleDown_ = false;
    Card doubleDownCard_;
};

