#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <string>
#include <vector>

#include "Deck.h"
#include "Card.h"


class Hand
{
    public:
        Hand();
        Card getCard(Deck D);
        int getTotalValue();
        bool isBust();
        Card removeCard(Card C);
        void isSoftHand(Card C);
        bool aceInHand();

        //might not implement
        int getHardValue();
        int getSoftValue();
    private:
        //Card cards[52];
        vector<Card>inHand;
        int handValue;
        int count;
};

#endif
