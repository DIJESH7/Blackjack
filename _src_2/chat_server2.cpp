//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <string>
#include <vector>
#include "asio.hpp"
#include "chat_message2.hpp"
#include "Deck.hpp"
//#include "Hand.hpp"
#include "BlackjackHand.hpp"

using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//------------------------------------------------------------------------------------------------

vector<int> plrs;
int playercount = 0;
bool inplay = false;


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


class chat_participant // A PLAYER
{
public:
    chat_participant()
    {
        std::cout << "Creating a new BLACKJACK player." << std::endl;
        money = 100;
    }
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;


    void playBJ()
    {
        //setupPlayer();
        //getBet();
    }


    /*
        Set up player for another round of BJ
    */
    void setupPlayer()
    {
        playerHands.clear();
        originalPlayerHand_.clear();
        playerHands.push_back(originalPlayerHand_);
        hasBlackjack = false;
        placedInsuranceBet = false;
        continuePlaying = false;
    }

    /*
        Receives player's bet
    */
    void getBet()
    {
        std::cout << "bet decreases total credits." << std::endl;
        money -= originalPlayerHand_.bet();
    }

    /*
        Returns the player's original hand.
    */
    BlackjackHand originalPlayerHand()
    {
        return originalPlayerHand_;
    }

    void addCardToPlayerHand(Card card)
    {
        originalPlayerHand_.addCard(card);
    }

    void takeTurn(BlackjackHand hand)
    {
        
    }

    void setblackjack(bool b)
    {
        hasBlackjack = b;
    }

        std::string displayHand()
        {
            std::string result;
            result += "<--Player ";
            result += name;
            result += ":\n";
            result += originalPlayerHand_.printAllHand();
            return result;
        }


    std::string name;
    int id;
    int bet;
    Hand phand;

private:
    int maximumScore = 21; // max score before bust
    int minimumDoubleDownScore = 9; // min score player can have to double down
    int maximumDoubleDownScore = 11; // max score player can have to double down
    int minimumDoubleDownSoftScore = 19; // min score player can double down if soft hand
    int maximumDoubleDownSoftScore = 21; // max score player can double down if soft hand
    double blackjackPayoutMultiplier = 3.0/2.0; // payout when player has BJ
    BlackjackHand originalPlayerHand_; // for player's original hand
    std::vector<BlackjackHand> playerHands; // for all of one player's hands
    double money; // available credits
    bool hasBlackjack = false; // true if player has BJ
    std::string choice;
    bool receivedChoice = false;
    double insuranceBet = false; // amount of bet for insurance
    bool placedInsuranceBet = false; // true if player made insurance bet
    bool continuePlaying = false; // true if player wants to play another round
    
};

//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------


class chat_room  // the TABLE with players vector
{
public:
    chat_room()
    {
        run();
    }
	// stores client in the participants vector and sends past msg logs
    void join(chat_participant_ptr participant) 
    {
        std::cout << "a player is joining the table." << std::endl;
    	participant->id = ++playercount;

    	participants_.insert(participant);
    	for (auto msg: recent_msgs_)
      		participant->deliver(msg);
    	
    }

    void leave(chat_participant_ptr participant)
    {
        std::cout << "a player is leaving the table." << std::endl;
        participants_.erase(participant);
    }

    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
          recent_msgs_.pop_front();

        for (auto participant: participants_)
            participant->deliver(msg);
    }

    void playBJ()
    {
        setup();
        // wait for players to join
        // wait for players to bet
        dealInitalCards();
        // iterate through players for dealing options
        // any insurance bets
        for(auto player : participants_)
        {
            //player.takeTurn(player.originalPlayerHand());
        }
    }

    void setup()
    {
        if (deck.size() <= minimumCardsBeforeShuffling)
        {
            deck.build();
            deck.shuffle();
        }
        dealerHand_.clear();
        dealer_has_blackjack_ = false;
        //join countdown
        //bet countdown
        //turn countdown
        //continue countdown

    }

    /*
        Deals the first two cards to each player and the dealer.
    */
    void dealInitalCards()
    {
        for (int i = 0; i < 2; i++)
        {
            dealerHand_.addCard(dealCard());
            for (auto player : participants_)
            { 
                Card temp = dealCard();
                //player->originalPlayerHand().addCard(dealCard());
                player->addCardToPlayerHand(temp);
            }
        }
        if (dealerHand_.blackjackValue() == maximum_score)
        {
            dealer_has_blackjack_ = true;
        }
    }

    /*
        Actions for the dealer's turn.
    */
    void dealerTurn()
    {
        while( dealerHand_.isSoft() && dealerHand_.blackjackValue() == dealer_hit_threshold || dealerHand_.blackjackValue() < dealer_hit_threshold)
        {
            dealerHand_.addCard( dealCard() );
        }
    }

    /*
        Returns the number of players in the room_.
    */
    int numPlayers()
    {
        return participants_.size();
    }

    /*
        Returns the minimum bet allowed.
    */
    double minBet()
    {
        return minimumBet;
    }
    /*
        Returns true if the dealer has Blackjack.
    */
    bool dealerHasBJ()
    {
        return dealer_has_blackjack_;
    }

    /*
        Returns the dealer's face up card.
    */
    Card dealerVisibleCard()
    {
        return dealerHand_.getCard(0);
    }

    /*
        Returns the dealer's hand.
    */
    BlackjackHand dealersHand()
    {
        return dealerHand_;
    }

    /*
        Check if any cards left. Returns a card from the deck.
    */
    Card dealCard()
    {
        if (deck.size() == 0)
        {
            deck.build();
            deck.shuffle();
        }
        return deck.dealCard();
    }

    /*
        Return true if all players have a name stored
    */
    bool all_players_have_a_name()
    {
        bool retval = true;
        for (auto player_: participants_)
        {
            retval = retval &&  (player_->name > "");
        }
        return retval;
    }


    std::string displayCards()
    {
        std::string result;
        
        result += "<--Dealer's Hand:\n";
        for(auto card : dealerHand_.inHand)
        {
            result += card.printCard();
            result += "\n";
        }
        
        for(auto player: participants_)
        {
            result += player->displayHand();
            //result += player->originalPlayerHand().printAllHand2();
        }
        
        return result;
    }

    void run()
    {
        deck.build();
        deck.shuffle();
        setup();
    }

    Card dealerShownCard() 
    {
        return dealerHand_.getCard(0);
    }


   


private:
    int dealer_hit_threshold = 17;
    int maximum_score = 21;
    int minimumBet = 0;
    int minimumCardsBeforeShuffling = 0;
    bool dealer_has_blackjack_ = false;
    Deck deck;
    BlackjackHand dealerHand_;


    std::set<chat_participant_ptr> participants_; // <ptr> players
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};




//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------




class chat_session  // A BLACKJACK PLAYER with socket_ and room_
  : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
      room_(room)
  {
    std::cout << "Creating a BLACKJACK player." << std::endl;
  }

  void start() // client joins the chat room
  {
    room_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const chat_message& msg) // send saved past msg log
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }


private:
  void do_read_header() 
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_msg_.decode_header()) 
            { 
                do_read_body(); 
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                read_msg_.body_length(0);
                read_msg_.gs.dealer_cards_valid = false;
                read_msg_.gs.player_cards_valid = false;


                if(read_msg_.ca.join) // fix so that one client doesn't start the game
                {
                    std::cout << "the name is " << read_msg_.ca.name << std::endl;
                    std::string m = std::string(read_msg_.ca.name) + " has joined.";
                    strcpy(read_msg_.body(),m.c_str());
                    read_msg_.body_length(strlen(read_msg_.body()));
                    self->name = std::string(read_msg_.ca.name);
                    self->bet = read_msg_.ca.bet;
                    read_msg_.gs.one = 1;
                    self->setupPlayer();
                }

                if(room_.all_players_have_a_name() && read_msg_.ca.bet)
                {
                    self->originalPlayerHand().placeBet(read_msg_.ca.bet);
                    std::cout << "All players are ready." << std::endl;
                    room_.dealInitalCards();
                    if(self->originalPlayerHand().blackjackValue() == 21)
                    {
                        std::cout << "Player has blackjack!" << std::endl;
                        self->setblackjack(true);
                    }
                    
                }
                if(table.dealerShownCard().getRank() == 'A' && read_msg_.ca.insurance = false) 
                {
                    std::cout << "Asking for insurance bet." << std::endl;
                    read_msg_.ga.askinsurance = true;
                }

                if(read_msg_.ca.hit)
                {


                }

                
                //std::string gui = room_.stringOfCards();
                std::string gui = room_.displayCards();
                std::cout << gui << std::endl;
                char g[gui.size() +1 ];
                std::copy(gui.begin(), gui.end(), g);
                g[gui.size()] = '\0';
                strcpy(read_msg_.ca.g, g);
                



                  
                //room_.deliver(read_msg_, id); // can send to specific client


                read_msg_.encode_header(); // save info in msg to be sent to client
                room_.deliver(read_msg_); // deliver msg to all clients
                do_read_header();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
  }

  void do_write()
    {
        auto self(shared_from_this());
        asio::async_write(socket_,
            asio::buffer(write_msgs_.front().data(),
              write_msgs_.front().length()),
            [this, self](std::error_code ec, std::size_t /*length*/)
            {
              if (!ec)
              {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
              }
              else
              {
                room_.leave(shared_from_this());
              }
            });
    }

    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;

};

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

class chat_server // THE BLACKJACK GAME
{
public:
  chat_server(asio::io_context& io_context,
      const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
    {
        std::cout << "Creating a BLACKJACK game." << std::endl;
        do_accept(); 
    }

private:
  void do_accept()
  {
    acceptor_.async_accept( 
        [this](std::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<chat_session>(std::move(socket), room_)->start(); 
            }
            do_accept(); 
        });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{ 
    try
    {
        if (argc < 2)
        {
          std::cerr << "Usage: chat_server <port> [<port> ...]\n";
          return 1;
        }
        asio::io_context io_context;

        std::list<chat_server> servers; 
        for (int i = 1; i < argc; ++i) 
        { 
          tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
          servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

  return 0;
}
