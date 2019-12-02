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
        void clear()
        {
            //TODO clears hand of cards
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
            int total = 0;
            for(auto card : inHand)
            {
                total += card.getValue();
            }
            return total;
        }
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
        Card removeCard(Card C)
        {
            // TODO
            return C;
        }
        void checkHand()
        {
            aceInHand();
            //if ( handValue > 21 && aceInHand() ) 
                //handValue -= 10; // whole hand value
        }
        // if there is an Ace in the Hand, value will change from 11 to 1 for Ace
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
        void printHand()
        {
            for(auto card: inHand)
            {
                card.display();
            }
        }

        std::string printAllHand(int id)
        {
            std::string result;
            result += "<-- Player ";
            result += to_string(id);
            result += " hand:\n";

            for(auto card: inHand)
            {   
                result += card.printCard(); // utsav caught this
            }
            return result;
        }

        bool canSplit()
        {
            return true; //this line for testing purposes
            if(inHand.size() != 2)
                return false;

            if(inHand[0].getRank() != inHand[1].getRank())
                return false;

            return true;
        }

        Card split()
        {
            Card c = inHand[1];
            inHand.pop_back();
            return c;
        }

        int get_bet()
        {
            return bet;
        }

        void set_bet(int bet_)
        {
            bet = bet_;
        }

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

