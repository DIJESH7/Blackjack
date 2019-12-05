#pragma once
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
            handValue = 0; // total value of cards
            count = 0;  // number of cards
            playing = true;
        }
        virtual ~Hand() { clear();}
        void addCard(Card C)
        {
            inHand.push_back(C);
            count++;
            aceInHand();
        }
    
        /**
        * Clears the inHand vector.
        */
        void clear()
        {
            inHand.clear();
        }
    
        /**
        * Returns true if initial hand is blackjack
        */
        bool isBlackJack()
        {
            if(inHand.size() == 2)
            {
                bool jack = false;
                for(auto c : inHand)
                {
                    if(c.getRank() == 'J')
                    {
                        jack = true;
                        break;
                    }
                }
                if(jack && getTotal() == 21)
                {
                    return true;
                }
            }
            return false;
        }
    
        /**
        * Returns total value of cards in hand
        */
        int getTotal()
        {
            int total = 0;
            for(auto card : inHand)
            {
                total += card.getValue();
            }
            return total;
        }
    
        /**
        * Returns true if player's hand value is more than 21
        */
        bool isBust()
        {
            aceInHand();
            if(getTotal() > 21)
            {
                return true;
            }
            //Returns true if all possible scores are greater than 21
            return false;
        }
    
        /**
        * Checks if there is an ace in hand
        */
        void checkHand()
        {
            aceInHand();
        }
    
        /**
        * Changes Ace's value from 11 to 1 
        * if the Ace's 11 value causes hand to
        * go over 21.
        */
        void aceInHand()
        {
            int size = inHand.size();
            if(getTotal() <= 21)
                return;
            for(int i=0; i < size; i++)
            {
                if(inHand[i].getValue() == 11)
                {
                    inHand[i].setValue(1);
                }
            }
        }
    
        /**
        * Calls the display function of each card in the hand.
        */
        void printHand()
        {
            for(auto card: inHand)
            {
                card.display();
            }
        }

        /**
        * Returns the player's cards in hand
        * in the form of a string.
        */
        std::string printAllHand(int id)
        {
            std::string result;
            result += "<-- Player ";
            result += to_string(id);
            result += " hand:\n";

            for(auto card: inHand)
            {   
                result += card.printCard();
            }
            return result;
        }

        /**
        * Returns true if the player's hand is able to split.
        */
        bool canSplit()
        {
            //return true; //this line for testing purposes
            if(inHand.size() != 2)
                return false;

            if(inHand[0].getRank() != inHand[1].getRank())
                return false;

            return true;
        }

        /**
        * Removes a card from the hand
        * and returns it
        * when the player wants to split.
        */
        Card split()
        {
            Card c = inHand[1];
            inHand.pop_back();
            return c;
        }

        /**
        * Returns the hand's bet.
        */
        int get_bet()
        {
            return bet;
        }

        /**
        * Sets the bet of the hand.
        */
        void set_bet(int bet_)
        {
            bet = bet_;
        }

        /**
        * Returns true if the player's hand
        * is able to double.
        */
        bool can_double()
        {
            if(inHand.size() == 2)
                return true;
            return false;
        }

        std::vector<Card>inHand;
        int handValue;
        int count;
        bool playing;
        int bet;
};

