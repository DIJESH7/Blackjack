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
#include <thread>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <string>
#include <ctime>
#include "asio.hpp"
#include "../include/chat_message.hpp"
#include "../include/Deck.hpp"
#include "../include/Hand.hpp"



using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;



//------------------------------------------------------------------------------------------------

// deck and hand are global variables so all classes can access
Deck d;
Hand h;
int turn = 0;

bool reveal              = false;
bool deal                = false;
bool dealer_initialized  = false;

//------------------------------------------------------------------------------------------------




class chat_participant
{
    public:
        virtual ~chat_participant() {}
        virtual void deliver(const chat_message& msg) = 0;

        void pHand(Card t)
        {
            getCurrentHand().addCard(t);
        }

        std::string printHand()
        {
            std::string result = "";
            int i = 0;
            for(auto hand : playerHand)
            {
                result += "hand: " + std::to_string(i++) + "\n";
                result += hand.printAllHand(id);
            }
            return result;
        }

        Hand& getCurrentHand()
        {
            if (playerHand.size() == 0)
            {
                Hand h;
                playerHand.push_back(h);
            }
            return playerHand[currentHand];
        }

        //TODO check why it works only sometimes
        //FIXME hand.hpp only checks if ace in hand, 
        //if yes -10, and sets ace.value = 1,
        //but this cannot work for reference...
        bool checkBust()
        {
            return getCurrentHand().isBust();
        }

        bool setNextHand()
        {
            int temp = (playerHand.size()-1);
            if(currentHand < temp)
            {
                currentHand++;
                return true;
            }
            else
                return false;
        }

        //TODO test if its working
        void split()
        {
            if(getCurrentHand().canSplit())
            {
                Hand h;
                h.set_bet(getCurrentHand().get_bet());
                playerHand.insert(playerHand.begin()+currentHand+1, h);
                Card temp;
                temp = d.getCard();
                Card c = getCurrentHand().split();
                pHand(temp);
                temp = d.getCard();
                playerHand[currentHand+1].addCard(c);
                playerHand[currentHand+1].addCard(temp);
            }
        }

	bool checkSplit()
	{
	    return getCurrentHand().canSplit();	
	}

        int getHandTotal(int idx)
        {   int size = playerHand.size();
            if(idx >=size )
            {
                return -1;
            }
            return playerHand[idx].getTotal();

        }

        std::string getAllBets()
        {
            std::string s = "";
            for(auto hand : playerHand)
            {
                s += std::to_string(hand.get_bet()) + "   ";
            }
            return s;
        }


        int id;
        bool play = false;
    private:
        int currentHand = 0;
        std::vector<Hand> playerHand;


};

class Dealer : public chat_participant
{
    public:
        Dealer() {}
        ~Dealer() {}
        std::string printHand()
        {
            std::string result;
            result = playerHand.printAllHand(id);
            if(reveal == false)
            {
                std::stringstream ss(result);
                std::string s = "";
                std::string token = "";
                std::getline(ss, s); //line with <-- Player 0...
                s += "\n";
                std::getline(ss, token); //line with first card
                s += token;
                s += "\n";
                s += "B ACK2\n"; //give back of card
                result = s;
            }
            return result;
        }

        void pHand(Card t)
        {
            playerHand.addCard(t);
        }

        void deal()
        {
            while(playerHand.getTotal() < 17)
            {
                Card temp;
                temp = d.getCard();
                pHand(temp);
            }
        }

        int getTotal()
        {
            return playerHand.getTotal();
        }

        void deliver(const chat_message& msg) {}
    private:
        Hand playerHand;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

int playercount = 0;
bool inplay = false;

class chat_room
{
    public:
        // puts client in participants vector and sends past msg logs
        void join(chat_participant_ptr participant) 
        {
            participant->id = ++playercount;

            if(inplay || playercount > 6)
            {
                //tell them to wait
                strcpy(handshake.ca.g, "Please wait for the game to finish\n");
                handshake.ca.turn = 0;
                handshake.encode_header();
                participant->deliver(handshake);
            }
            else
            {
                participants_.insert(participant);
                handshake.ca.id = participant->id;
                handshake.ca.turn = 0;
                handshake.encode_header();
                participant->deliver(handshake);
                for (auto msg: recent_msgs_)
                {
                    msg.ca.id = participant->id;
                    participant->deliver(msg);
                }
            }



        }

        void leave(chat_participant_ptr participant)
        {
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

        // TODO deliver msg to specific client
        void deliver2(const chat_message& msg, int recipient_id)
        {
            recent_msgs_.push_back(msg);
            while (recent_msgs_.size() > max_recent_msgs)
                recent_msgs_.pop_front();

            for (auto participant: participants_)
            {
                if(participant->id == recipient_id)
                    participant->deliver(msg);
            }
        }


        int sizeOfParticipants()
        {
            int size = participants_.size();
            return size;
        }

        void giveCard(int pid) // for initial dealing
        {
            if(pid == -1)
            {
                for(auto participant: participants_)
                {	
                    Card temp;
                    temp = d.getCard();
                    participant->pHand(temp);
                }
                Card temp;
                temp = d.getCard();
                dealer->pHand(temp);
                return;
            }
            if(pid == 0)
            {
                Card temp;
                temp = d.getCard();
                dealer->pHand(temp);
                return;
            }
            else
            {
                for(auto participant: participants_)
                {
                    if(participant->id == pid)
                    {
                        Card temp;
                        temp = d.getCard();
                        participant->pHand(temp);
                    }
                }
            }
        }

        bool check_points(int id)
        {
            for (auto participant : participants_)
            {
                if(participant->id == id)
                {
                    if(participant->checkBust())
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        std::string stringOfCards() //string of every player's cards
        {
            std::string result = "";
            for(auto participant: participants_)
            {
                //TODO need to add dealer's cards as well
                result += participant->printHand();
            }
            result += dealer->printHand();
            std::cout << result << std::endl;
            return result;
        }

        bool checkAllPlay(int id)
        {
            for (auto participant : participants_)
            {
                if(participant->id == id)
                {
                    participant->play = true;
                }
            }
            for (auto participant : participants_)
            {
                if(!participant->play)
                {
                    return false;
                }
            }
            return true;
        }

        void changeActivePlayer(int pturn)
        {
            chat_message handshake;
            handshake.ca.turn = pturn;
            handshake.ca.split_button = canBeSplit(pturn);
            handshake.ca.double_button = checkDouble(pturn);
            turn = pturn;
            handshake.encode_header();
            for (auto participant : participants_)
            {
                participant->deliver(handshake);
            }
        }

        bool idInVector(int id)
        {
            for (auto participant : participants_)
            {
                if(participant->id == id)
                {
                    return true;
                }
            }
            return false;
        }

        int getLastId()
        {
            int i = 0;
            for (auto participant : participants_)
            {
                if(i == sizeOfParticipants() - 1)
                {
                    return participant->id;
                }
                i++;
            }
            return -1;
        }

        bool setNextHand(int id)
        {
            for(auto participant : participants_)
            {
                if(participant->id == id)
                {
                    return participant->setNextHand();
                }
            }
            return false;
        }

        void splitHand(int id)
        {
            for (auto participant : participants_)
            {
                if(participant->id == id)
                {
                    participant->split();
                }
            }
        }

	bool canBeSplit(int id)
	{
            for (auto participant : participants_)
            {
                if(participant->id == id)
                {
                    return participant->checkSplit();
                }
            }
	    return false;
	}

        bool checkDouble(int id)
        {
            for(auto participant : participants_)
            {
                if(participant->id == id)
                {
                    return participant->getCurrentHand().can_double();
                }
            }
            return false;
        }

        void announceResults(int id)
        {
            int target = dealer->getTotal();
            if(id == -1) //announce all
            {
                for (auto participant : participants_)
                {
                    handshake.ca.turn = turn;
                    std::vector<int> v;
                    int total = participant->getHandTotal(0);
                    //in vector v / handWins
                    // 0  = game in progress
                    //-1  = lost
                    // 1  = win
                    int i = 1;

                    while(total != -1)
                    {

                        //player Hand busts
                        if(total > 21) 
                            v.push_back(-1);
                        //dealer busts
                        else if(target > 21) 
                            v.push_back(1);
                        //dealer is closer to 21 than player
                        else if(target > total) 
                            v.push_back(-1);
                        //tied
                        else if(target == total)
                            v.push_back(2);
                        //player is closer to 21 than dealer
                        else 
                            v.push_back(1);

                        total = participant->getHandTotal(i++);
                    }
                    std::copy(v.begin(), v.end(), handshake.ca.handWins);
                    handshake.ca.size = v.size();
                    handshake.encode_header();
                    participant->deliver(handshake);
                }
                return;
            }
        
        }

        void set_bet(int id, int bet)
        {
            for(auto participant : participants_)
            {
                if(participant->id == id)
                {
                    participant->getCurrentHand().set_bet(bet);
                }
            }
        }

        int get_bet(int id)
        {
            for(auto participant : participants_)
            {
                if(participant->id == id)
                {
                    return participant->getCurrentHand().get_bet();
                }
            }
            return -1;
        }

        void deliverBets(int id)
        {
            for(auto participant : participants_)
            {
                if(participant->id == id)
                {
                    chat_message msg;
                    std::string s = participant->getAllBets();
                    char bet[s.size() +1 ];
                    std::copy(s.begin(), s.end(), bet);
                    bet[s.size()] = '\0';
                    strcpy(msg.ca.updateBet, bet);
                    msg.ca.id = id;
                    std::cout << msg.ca.updateBet << " sbkhrb" << std::endl;
                    msg.ca.update = true;
                    msg.encode_header();
                    deliver2(msg, id);
                    return;
                }
            }
            return;
        }

        Dealer* dealer;

    private:
        std::set<chat_participant_ptr> participants_;
        enum { max_recent_msgs = 100 };
        chat_message_queue recent_msgs_;
        chat_message handshake;
};

//----------------------------------------------------------------------

class chat_session
: public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
    public:
        chat_session(tcp::socket socket, chat_room& room)
            : socket_(std::move(socket)),
            room_(room)
    {
    }

        void start() // client joins the chat room
        {
            //initializes a dealer only one time
            if(!dealer_initialized)
            {
                dealer_initialized = true;
                room_.dealer = new Dealer();
                room_.dealer->id = 0;
            }
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
        // calls data() and waits for an async_write from client's do_write then calls decode
        void do_read_header() 
        {
            auto self(shared_from_this());
            asio::async_read(socket_,
                    asio::buffer(read_msg_.data(), chat_message::header_length),
                    [this, self](std::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec && read_msg_.decode_header()) 
                    {

                      if(read_msg_.ca.play == true)
                      {

                          if(!deal) //deal cards to dealer first time only
                          {
                            room_.giveCard(0);
                            room_.giveCard(0);
                            deal = true;
                          }
                          room_.giveCard(read_msg_.ca.id);
                          room_.giveCard(read_msg_.ca.id);
                          std::string gui = room_.stringOfCards();
                          char g[gui.size() +1 ];
                          std::copy(gui.begin(), gui.end(), g);
                          g[gui.size()] = '\0';
                          strcpy(read_msg_.ca.g, g);
                          room_.set_bet(read_msg_.ca.id, read_msg_.ca.bet);
                      }

                      if(read_msg_.ca.hit == true)
                      {
                          room_.giveCard(read_msg_.ca.id); 
                          std::string gui = room_.stringOfCards();
                          bool busted = room_.check_points(read_msg_.ca.id);

                          char g[gui.size() +1 ];
                          std::copy(gui.begin(), gui.end(), g);
                          g[gui.size()] = '\0';
                          strcpy(read_msg_.ca.g, g);
                          if(busted)
                            read_msg_.ca.stand = true;
                      }
                      else if(read_msg_.ca.split == true)
                      {
                          room_.splitHand(read_msg_.ca.id);
                          std::string gui = room_.stringOfCards();
                          bool busted = room_.check_points(read_msg_.ca.id);

                          char g[gui.size() +1 ];
                          std::copy(gui.begin(), gui.end(), g);
                          g[gui.size()] = '\0';
                          strcpy(read_msg_.ca.g, g);
                          if(busted)
                            read_msg_.ca.stand = true;
                          
                          room_.deliverBets(read_msg_.ca.id);
                      }
                      else if(read_msg_.ca.doubledown)
                      {
                          room_.set_bet(read_msg_.ca.id, room_.get_bet(read_msg_.ca.id) + read_msg_.ca.bet);  
                          room_.giveCard(read_msg_.ca.id);
                          std::string gui = room_.stringOfCards();
                          char g[gui.size() +1 ];
                          std::copy(gui.begin(), gui.end(), g);
                          g[gui.size()] = '\0';
                          strcpy(read_msg_.ca.g, g);
                          read_msg_.ca.stand = true;
                          room_.deliverBets(read_msg_.ca.id);
                          
                      }
                      else if(read_msg_.ca.leave == true)
                      {
                          if(turn == read_msg_.ca.id) //left when it was his turn
                          {
                            read_msg_.ca.stand = true;
                          }
                          else if(turn == 0) //left when people were still joining
                          {
                            
                          }
                          else if(turn == -1) //left when dealer was taking his turn
                          {
                            
                          }
                          else if(read_msg_.ca.id > turn) //left before taking his turn
                          {
                            
                          }
                          else if(read_msg_.ca.id < turn) //left after taking his turn
                          {

                          }
                          room_.leave(shared_from_this());
                      }

                      if(read_msg_.ca.stand == true)
                      {
                          //if setNextHand true, it sets player hand to next hand
                          if(!room_.setNextHand(read_msg_.ca.id))
                          {
                            if(turn < room_.getLastId())
                            {
                              turn++;
                              //keep incrementing turn while id is not in vector
                              while(!room_.idInVector(turn))
                              {
                                turn++;
                              }
                            }
                            else
                            {
                              turn = -1; //everyone is finished so dealer's turn
                              reveal = true;
                              room_.dealer->deal();
                              std::string gui = room_.stringOfCards();

                              char g[gui.size() +1 ];
                              std::copy(gui.begin(), gui.end(), g);
                              g[gui.size()] = '\0';
                              strcpy(read_msg_.ca.g, g);
                            }
                          }
                      }

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

                    read_msg_.ca.turn = turn;
		    read_msg_.ca.split_button = room_.canBeSplit(turn);
                    read_msg_.ca.double_button = room_.checkDouble(turn);
                    read_msg_.ca.bet = room_.get_bet(read_msg_.ca.id);
                    std::cout << "dOUBLE :: " << read_msg_.ca.double_button << std::endl;
                    read_msg_.encode_header(); // save info in msg to be sent to client
                    room_.deliver(read_msg_); // deliver msg to all clients
                    if(reveal)
                    {
                        usleep(500000); //sleep for half second
                                         //give user chance to see the cards
                        room_.announceResults(-1);
                    }

                    //room_.deliver(read_msg_, id); // can send to specific client

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

//----------------------------------------------------------------------

class chat_server
{
    public:
        chat_server(asio::io_context& io_context,
                const tcp::endpoint& endpoint)
            : acceptor_(io_context, endpoint)
        {
            do_accept(); 
        }

        void start_play()
        {
            room_.changeActivePlayer(1);
        }

        int numPlayers()
        {
            return room_.sizeOfParticipants();
        }

    private:
        void do_accept() // accepts client's do_connect() call
        {
            acceptor_.async_accept( 
                    [this](std::error_code ec, tcp::socket socket)
                    {
                    if (!ec)
                    {
                    // start the chat_session and calls start()
                    std::make_shared<chat_session>(std::move(socket), room_)->start(); 
                    }
                    // waiting for more clients
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

        //making deck and shuffling
        d.build();
        d.shuffle();

        std::list<chat_server> servers; 

        // starting a server calls the do_accept() function
        for (int i = 1; i < argc; ++i) 
        { 
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }
        std::thread t([&io_context](){ io_context.run(); });
        //double seconds_passed;
        //double seconds_expire = 20;
        while(true)
        {
            usleep(1000000);//sleep for 1 second to reduce memory consumption
            if(servers.front().numPlayers() >= 1)
            {
                //start the clock when one player joins
                //clock_t start = clock();
                //while(true) 
                //{

                usleep(10000000);//sleep for 10 seconds
                //seconds_passed = (clock() - start)/CLOCKS_PER_SEC;
                //if(seconds_passed > seconds_expire)
                //{

                std::cout << "Expired" << std::endl;
                inplay = true;
                servers.front().start_play();
                break;

                //}
                //}
                //break;
            }
        }
        //io_context.run();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
