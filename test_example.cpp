#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ExampleTestModule
#include <boost/test/unit_test.hpp>

// header defining all the code to be tested
#include "include/Hand.hpp"
#include "include/Deck.hpp"
//#include "include/Card.hpp"

using namespace boost;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(ExampleTestSuite)



struct TestFixtureHand
{
    Hand hand;
    TestFixtureHand()
    {
        Card card;
        card.setInfo(10, 'K', 'S');
        hand.addCard(card);
    }
};

struct TestFixtureDeck
{
    Deck deck;
    TestFixtureDeck()
    { 
        deck.build();
    }
};



BOOST_FIXTURE_TEST_CASE(lessthan21, TestFixtureHand)
{
    // just contains 1 card
    BOOST_CHECK( ! hand.is21 ());
   
    Card CC;
    // add another one  
    CC.setInfo(10, 'Q', 'C');
    Hand hand; 
    hand.addCard (CC);
    BOOST_CHECK( ! hand.is21 ()); 
}

BOOST_FIXTURE_TEST_CASE(over21, TestFixtureHand)
{
    Card CC;
    CC.setInfo(10, 'J', 'D');
    hand.addCard (CC);
    hand.addCard (CC);
    BOOST_CHECK( ! hand.is21 ());
}

BOOST_FIXTURE_TEST_CASE(equal21, TestFixtureHand)
{
    Card CC;
    CC.setInfo(11, 'A', 'H');
    hand.addCard (CC);
    BOOST_CHECK(  hand.is21 ());
}

BOOST_FIXTURE_TEST_CASE(numOfCards, TestFixtureDeck)
{
    BOOST_CHECK( 312 == deck.cardsLeft() );
    Hand hand;
    hand.addCard( deck.getCard() );
    BOOST_CHECK( 311 == deck.cardsLeft() );
}



BOOST_FIXTURE_TEST_CASE(notBust, TestFixtureHand)
{
    // just contains 1 card
    BOOST_CHECK(! hand.isBust() );
    Card CC;
    // add another one  
    CC.setInfo(10, 'Q', 'C');
    Hand hand; 
    hand.addCard (CC);
    BOOST_CHECK(! hand.isBust() ); 
}

BOOST_FIXTURE_TEST_CASE(Bust, TestFixtureHand)
{
    BOOST_CHECK( ! hand.isBust() );
    Card CC;
    CC.setInfo(10, 'J', 'D');
    hand.addCard (CC);
    hand.addCard (CC);
    BOOST_CHECK( hand.isBust() );
}









BOOST_AUTO_TEST_SUITE_END() 