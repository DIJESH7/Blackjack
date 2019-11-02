#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Card.hpp"

using namespace std;

class Deck
{

public:
    void build()
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
                    result.setValue(rankCounter); //1 2 3 4... 12, 13

                    if(rankCounter <= 13 && rankCounter >= 11)
                        result.setValue(10);
                    if(rankCounter == 1)
                        result.setValue(11);
                    //11, 2, 3, 4, ..., 10, 10
                    cards_.push_back(result);
                }
            }
        }
    }

    void shuffle()
    {
        srand((unsigned) time(0));
        for(int i = 0 ; i < 52 ; i++)
        {
            swap( cards_[i] , cards_[ rand()%52] );
        }
    }

    bool deck_is_empty()
    {
        return cards_.empty();
    }


    //for when deck runs out of cards
    // for later
    void reset()
    {
        //clear the vector
        if(deck_is_empty)
        {
            build();
        }
    }

    //get card and return it
    Card getCard()
    {
        Card result;
        result = cards_.front(); // targets first card
        cards_.erase(cards_.begin());   // removes card from deck
        return result;  // returns card
    }

private:
    vector <Card> cards_;
};

