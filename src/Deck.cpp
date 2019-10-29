#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include "Deck.h"

using namespace std;

Deck::Deck()
{
   build();
}


void Deck::build()
{   
    // create 312 cards
    for(int d = 0 ; d < 6 ; d++)
    {
        // make 52 cards with a Rank and Suit
        for (int suitCounter = Spades; suitCounter < 4; suitCounter++)
        {
            for (int rankCounter = Ace; rankCounter <= 13; rankCounter++)
            {   
                Card result;
                result.setRank(static_cast<Rank>(rankCounter));
                result.setSuit(static_cast<Suit>(suitCounter));
                result.setValue(rankCounter);
                if(rankCounter <= 13 && rankCounter >= 11)
                    result.setValue(10);
                if(rankCounter == 1)
                    result.setValue(11);
                cards_.push_back(result);
            }
        }
    }
}

void Deck::shuffle()
{
    random_shuffle(cards_.begin(), cards_.end());
}


Card Deck::deal()
{
    Card result;
    result = cards_.front(); // targets first card
    cards_.erase(cards_.begin());   // removes card from deck
    return result;  // returns card to hand
}









/*

void Deck::build2()
{
    // helps iterate 312 indexes
    currCard = 0; 
    
    // create 312 cards
    for(int d = 0 ; d < 6 ; d++)
    {
        // make 52 cards with a Rank and Suit
        for (int suitCounter = Spades; suitCounter < 4; suitCounter++)
        {
            for (int rankCounter = Ace; rankCounter <= 13; rankCounter++)
            {   
                cards[currCard].setRank(static_cast<Rank>(rankCounter));
                cards[currCard].setSuit(static_cast<Suit>(suitCounter));

                    //cards[currCard].display(); // for deck checking

                currCard++;
            }
        }
    }
}

void Deck::shuffle2()
{
    random_shuffle(std::begin(cards), std::end(cards));
  
    //for (int currCard = 51; 0 <= currCard; currCard--)
    for (int currCard = 311; 0 <= currCard; currCard--)
    {
            //cards[currCard].display(); // for deck checking
    }
}

Card Deck::deal2()
{
    // TODO throw error if deck is empty
    
	currCard--;
	return cards[currCard];
}


*/






























