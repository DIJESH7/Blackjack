#include <iostream>
//#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include "Card.hpp"

using namespace std;

class Deck
{
public:
    Deck()
    {

    };
    virtual ~Deck(){};
    
   /**
   * Populates the vector of cards with six decks.
   */
    void build() 
    {
        cards_.clear();
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

   /**
   * Calls the display function of each card
   * in the card vector.
   */
    void displayDeck()
    {
      for (auto it : cards_)
      {
        Card c = it;
        c.display();
      }
    }
    
   /**
   * Shuffles the deck's vector of cards randomly.
   */
    void shuffle()
    {
        srand((unsigned) time(0));
        for(int i = 0 ; i < 312 ; i++)
        {
            std::swap( cards_[i] , cards_[ rand()%311] );
        }
    }
    
   /**
   * Returns the number of cards left in deck.
   */
    int cardsLeft()
    {
      return cards_.size();
    }
    
   /**
   * Returns ture if the deck is empty.
   */
    bool deck_is_empty()
    {
        return cards_.empty();
    }
    
   /**
   * Calls the deck object's build function
   * if the vector of cards becomes empty
   */
    void reset()
    {
        //clear the vector
        if(deck_is_empty())
        {
            build();
        }
    }
    
   /**
   * Returns a card from the deck
   * and removes that card from the deck.
   */
    Card getCard()
    {
        Card temp = cards_.front();
        cards_.pop_front();
        return temp;
    }

    std::deque <Card> cards_;
private:
    
};

