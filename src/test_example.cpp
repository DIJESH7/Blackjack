#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ExampleTestModule
#include <boost/test/unit_test.hpp>

// header defining all the code to be tested
#include "../include/Hand.hpp"
#include "../include/Card.hpp"

using namespace boost;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(ExampleTestSuite)



struct TestFixtureDeck
{
    Hand hand;
    TestFixtureAllCard()
    {
        Card card;
        card.setInfo(10, 'K', 'S');
        hand.addCard(card);
    }
};


BOOST_FIXTURE_TEST_CASE(lessthan21, TestFixtureDeck)
{
    // just contains 1 card
    BOOST_CHECK( ! hand.is21 ());
   
    Card CC;
    // add another one
    CC.setInfo(10, 'Q', 'C'); 
    hand.addCard (CC);
    BOOST_CHECK( ! hand.is21 ());
    
}

BOOST_FIXTURE_TEST_CASE(over21, TestFixtureDeck)
{
    Card CC;
    CC.setInfo(10, 'J', 'D');
    hand.addCard (CC);
    hand.addCard (CC);
    BOOST_CHECK( ! hand.is21 ());
}

BOOST_FIXTURE_TEST_CASE(equal21, TestFixtureDeck)
{
    Card CC;
    CC.setInfo(11, 'A', 'H');
    hand.addCard (CC);
    BOOST_CHECK(  hand.is21 ());
}

BOOST_AUTO_TEST_SUITE_END() 
