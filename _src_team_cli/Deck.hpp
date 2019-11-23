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
    /*
        Constructor for Deck object.
    */
    Deck()
    {
    };
    /*
        Building six decks
    */
    void build() 
    {
        deck.clear();
        for(int d = 0 ; d < 6 ; d++)
        {
            Card result;
            
            result.setInfo(1, 'A','S');
            deck.push_back(result);
            result.setInfo(2, '2', 'S');
            deck.push_back(result);
            result.setInfo(3, '3', 'S');
            deck.push_back(result);
            result.setInfo(4, '4', 'S');
            deck.push_back(result);
            result.setInfo(5, '5', 'S');
            deck.push_back(result);
            result.setInfo(6, '6', 'S');
            deck.push_back(result);
            result.setInfo(7, '7', 'S');
            deck.push_back(result);
            result.setInfo(8, '8', 'S');
            deck.push_back(result);
            result.setInfo(9, '9', 'S');
            deck.push_back(result);
            result.setInfo(10, 'T', 'S');
            deck.push_back(result);
            result.setInfo(10, 'J', 'S');
            deck.push_back(result);
            result.setInfo(10, 'Q', 'S');
            deck.push_back(result);
            result.setInfo(10, 'K', 'S');
            deck.push_back(result);

            result.setInfo(1, 'A', 'H');
            deck.push_back(result);
            result.setInfo(2, '2', 'H');
            deck.push_back(result);
            result.setInfo(3, '3', 'H');
            deck.push_back(result);
            result.setInfo(4, '4', 'H');
            deck.push_back(result);
            result.setInfo(5, '5', 'H');
            deck.push_back(result);
            result.setInfo(6, '6', 'H');
            deck.push_back(result);
            result.setInfo(7, '7', 'H');
            deck.push_back(result);
            result.setInfo(8, '8', 'H');
            deck.push_back(result);
            result.setInfo(9, '9', 'H');
            deck.push_back(result);
            result.setInfo(10, 'T', 'H');
            deck.push_back(result);
            result.setInfo(10, 'J', 'H');
            deck.push_back(result);
            result.setInfo(10, 'Q', 'H');
            deck.push_back(result);
            result.setInfo(10, 'K', 'H');
            deck.push_back(result);

            result.setInfo(1, 'A','C');
            deck.push_back(result);
            result.setInfo(2, '2', 'C');
            deck.push_back(result);
            result.setInfo(3, '3', 'C');
            deck.push_back(result);
            result.setInfo(4, '4', 'C');
            deck.push_back(result);
            result.setInfo(5, '5', 'C');
            deck.push_back(result);
            result.setInfo(6, '6', 'C');
            deck.push_back(result);
            result.setInfo(7, '7', 'C');
            deck.push_back(result);
            result.setInfo(8, '8', 'C');
            deck.push_back(result);
            result.setInfo(9, '9', 'C');
            deck.push_back(result);
            result.setInfo(10, 'T', 'C');
            deck.push_back(result);
            result.setInfo(10, 'J', 'C');
            deck.push_back(result);
            result.setInfo(10, 'Q', 'C');
            deck.push_back(result);
            result.setInfo(10, 'K', 'C');
            deck.push_back(result);

            result.setInfo(1, 'A','D');
            deck.push_back(result);
            result.setInfo(2, '2', 'D');
            deck.push_back(result);
            result.setInfo(3, '3', 'D');
            deck.push_back(result);
            result.setInfo(4, '4', 'D');
            deck.push_back(result);
            result.setInfo(5, '5', 'D');
            deck.push_back(result);
            result.setInfo(6, '6', 'D');
            deck.push_back(result);
            result.setInfo(7, '7', 'D');
            deck.push_back(result);
            result.setInfo(8, '8', 'D');
            deck.push_back(result);
            result.setInfo(9, '9', 'D');
            deck.push_back(result);
            result.setInfo(10, 'T', 'D');
            deck.push_back(result);
            result.setInfo(10, 'J', 'D');
            deck.push_back(result);
            result.setInfo(10, 'Q', 'D');
            deck.push_back(result);
            result.setInfo(10, 'K', 'D');
            deck.push_back(result);
        }
    }


    /*
        Returns the last card in the shoe. (which here is actually the first card)
    */
    Card dealCard() //could name getCard()
    {
        Card temp = deck.front();
        deck.pop_front();
        //temp.display();
        return temp;
    }

    /*
        Returns the number of cards in the deck.
    */
    int size() // or cards left
    {
        return deck.size();
    }

    /*
        Shuffles the shoe.
    */
    void shuffle()
    {
        srand((unsigned) time(0));
        for(int i = 0 ; i < 312 ; i++)
        {
            std::swap( deck[i] , deck[ rand()%311] );
        }
    }








    void displayDeck()
    {
      for (auto it : deck)
      {
        Card c = it;
        c.display();
      }
    }
    
    
    bool deck_is_empty()
    {
        return deck.empty();
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
    
    
private:
    std::deque <Card> deck;

};

