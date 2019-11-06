#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
//#include <deque>
#include "Card.hpp"

using namespace std;

class Deck
{
public:
    Deck()
    {

    };
    virtual ~Deck(){};
   
    void build3() // older way of building deck
    {
        for(int d = 0 ; d < 6 ; d++)
        {
            for (int suitCounter = 0; suitCounter < 4; suitCounter++)
            {
                for (int rankCounter = 1; rankCounter <= 13; rankCounter++)
                {   
                    Card result;
                    result.setRank(rankCounter);
                    result.setSuit(suitCounter);
                    result.setValue(rankCounter);

                    if(rankCounter <= 13 && rankCounter >= 11) result.setValue(10);
                    if(rankCounter == 1) result.setValue(11);
                    cards_.push_back(result);
                }
            }
        }
    }

    void build2() // testing if building with for loops is better or not
    {
        for(int d = 0 ; d < 6 ; d++)
        {
            for (int suitCounter = 0; suitCounter < 4; suitCounter++)
            {
                for (int rankCounter = 1; rankCounter <= 13; rankCounter++)
                {  
                    Card result;
                    int v = rankCounter;
                    char s;
                    char r;
                    
                    if(rankCounter == 1) r = 'A';
                    if(rankCounter == 2) r = '2';
                    if(rankCounter == 3) r = '3';
                    if(rankCounter == 4) r = '4';
                    if(rankCounter == 5) r = '5';
                    if(rankCounter == 6) r = '6';
                    if(rankCounter == 7) r = '7';
                    if(rankCounter == 8) r = '8';
                    if(rankCounter == 9) r = '9';
                    if(rankCounter == 10) r = 'T';
                    if(rankCounter == 11) r = 'J';
                    if(rankCounter == 12) r = 'Q';
                    if(rankCounter == 13) r = 'K';

                    if(suitCounter == 0) s = 'S';
                    if(suitCounter == 1) s = 'H'; 
                    if(suitCounter == 2) s = 'C';
                    if(suitCounter == 3) s = 'D';

                    if(rankCounter <= 13 && rankCounter >= 11) v = 10;
                    if(rankCounter == 1) v = 11;
                    result.setInfo(v,r,s);
                    cards_.push_back(result);


                    
                    result.setInfo(11, 'A', 'H');
                    cards_.push_back(result);
                    result.setInfo(2, '2', 'H');
                    cards_.push_back(result);
                    result.setInfo(3, '3', 'H');
                    cards_.push_back(result);
                    result.setInfo(4, '4', 'H');
                    cards_.push_back(result);
                    result.setInfo(5, '5', 'H');
                    cards_.push_back(result);
                    result.setInfo(6, '6', 'H');
                    cards_.push_back(result);
                    result.setInfo(7, '7', 'H');
                    cards_.push_back(result);
                    result.setInfo(8, '8', 'H');
                    cards_.push_back(result);
                    result.setInfo(9, '9', 'H');
                    cards_.push_back(result);
                    result.setInfo(10, 'T', 'H');
                    cards_.push_back(result);
                    result.setInfo(10, 'J', 'H');
                    cards_.push_back(result);
                    result.setInfo(10, 'Q', 'H');
                    cards_.push_back(result);
                    result.setInfo(10, 'K', 'H');
                    cards_.push_back(result);
                }
            }
        }
    }

    void build() // direct approach to building deck
    {
        for(int d = 0 ; d < 6 ; d++)
        {
            Card result;
            
            result.setInfo(11, 'A','S');
            cards_.push_back(result);
            result.setInfo(2, '2', 'S');
            cards_.push_back(result);
            result.setInfo(3, '3', 'S');
            cards_.push_back(result);
            result.setInfo(4, '4', 'S');
            cards_.push_back(result);
            result.setInfo(5, '5', 'S');
            cards_.push_back(result);
            result.setInfo(6, '6', 'S');
            cards_.push_back(result);
            result.setInfo(7, '7', 'S');
            cards_.push_back(result);
            result.setInfo(8, '8', 'S');
            cards_.push_back(result);
            result.setInfo(9, '9', 'S');
            cards_.push_back(result);
            result.setInfo(10, 'T', 'S');
            cards_.push_back(result);
            result.setInfo(10, 'J', 'S');
            cards_.push_back(result);
            result.setInfo(10, 'Q', 'S');
            cards_.push_back(result);
            result.setInfo(10, 'K', 'S');
            cards_.push_back(result);

            result.setInfo(11, 'A', 'H');
            cards_.push_back(result);
            result.setInfo(2, '2', 'H');
            cards_.push_back(result);
            result.setInfo(3, '3', 'H');
            cards_.push_back(result);
            result.setInfo(4, '4', 'H');
            cards_.push_back(result);
            result.setInfo(5, '5', 'H');
            cards_.push_back(result);
            result.setInfo(6, '6', 'H');
            cards_.push_back(result);
            result.setInfo(7, '7', 'H');
            cards_.push_back(result);
            result.setInfo(8, '8', 'H');
            cards_.push_back(result);
            result.setInfo(9, '9', 'H');
            cards_.push_back(result);
            result.setInfo(10, 'T', 'H');
            cards_.push_back(result);
            result.setInfo(10, 'J', 'H');
            cards_.push_back(result);
            result.setInfo(10, 'Q', 'H');
            cards_.push_back(result);
            result.setInfo(10, 'K', 'H');
            cards_.push_back(result);

            result.setInfo(11, 'A','C');
            cards_.push_back(result);
            result.setInfo(2, '2', 'C');
            cards_.push_back(result);
            result.setInfo(3, '3', 'C');
            cards_.push_back(result);
            result.setInfo(4, '4', 'C');
            cards_.push_back(result);
            result.setInfo(5, '5', 'C');
            cards_.push_back(result);
            result.setInfo(6, '6', 'C');
            cards_.push_back(result);
            result.setInfo(7, '7', 'C');
            cards_.push_back(result);
            result.setInfo(8, '8', 'C');
            cards_.push_back(result);
            result.setInfo(9, '9', 'C');
            cards_.push_back(result);
            result.setInfo(10, 'T', 'C');
            cards_.push_back(result);
            result.setInfo(10, 'J', 'C');
            cards_.push_back(result);
            result.setInfo(10, 'Q', 'C');
            cards_.push_back(result);
            result.setInfo(10, 'K', 'C');
            cards_.push_back(result);

            result.setInfo(11, 'A','D');
            cards_.push_back(result);
            result.setInfo(2, '2', 'D');
            cards_.push_back(result);
            result.setInfo(3, '3', 'D');
            cards_.push_back(result);
            result.setInfo(4, '4', 'D');
            cards_.push_back(result);
            result.setInfo(5, '5', 'D');
            cards_.push_back(result);
            result.setInfo(6, '6', 'D');
            cards_.push_back(result);
            result.setInfo(7, '7', 'D');
            cards_.push_back(result);
            result.setInfo(8, '8', 'D');
            cards_.push_back(result);
            result.setInfo(9, '9', 'D');
            cards_.push_back(result);
            result.setInfo(10, 'T', 'D');
            cards_.push_back(result);
            result.setInfo(10, 'J', 'D');
            cards_.push_back(result);
            result.setInfo(10, 'Q', 'D');
            cards_.push_back(result);
            result.setInfo(10, 'K', 'D');
            cards_.push_back(result);
        }
    }

    void displayDeck()
    {
      for (auto it : cards_)
      {
        Card c = it;
        c.display();
      }
    }
    void shuffle()
    {
        srand((unsigned) time(0));
        for(int i = 0 ; i < 312 ; i++)
        {
            std::swap( cards_[i] , cards_[ rand()%311] );
        }
    }
    int cardsLeft()
    {
      return cards_.size();
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
        if(deck_is_empty())
        {
            build();
        }
    }
    
    //get card and return it
    Card getCard()
    {
        Card temp = cards_.front();
        cards_.pop_front();
        return temp;
    }
    std::deque <Card> cards_;
private:
    
};

