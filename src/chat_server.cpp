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
            playerHand.addCard(t);
        }

        std::string printHand()
        {
            std::string result;
            result = playerHand.printAllHand(id);
            return result;
        }


        int id;
        Hand playerHand;
        bool play = false;
    private:


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
            std::cout << "reveal: " << reveal << std::endl;
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

        void deal()
        {
            while(playerHand.getTotal() < 17)
            {
                Card temp;
                temp = d.getCard();
                pHand(temp);
            }
        }

        void deliver(const chat_message& msg) {}
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
                //strcpy(handshake.ca.g, "");
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



        std::string stringOfCards() //string of every player's cards
        {
            std::string result = "";
            for(auto participant: participants_)
            {
                //TODO need to add dealer's cards as well
                result += participant->printHand();
            }
            result += dealer->printHand();
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
            turn = pturn;
            std::cout << "Got here" << std::endl;
            handshake.encode_header();
            std::cout << "Got far" << std::endl;
            for (auto participant : participants_)
            {
                participant->deliver(handshake);
            }
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
                          //if not dealt before and play from player first
                          //std::cout << read_msg_.ca.id << std::endl;
                          //if(!deal && (read_msg_.ca.id == 1))
                          //{
                          //  //thread starts
                          //  std::thread t([&](){
                          //  while(true)
                          //  {
                          //    std::cout << "H" << std::endl;
                          //    //when everyone is ready and 
                          //    //timer expires and didn't deal before
                          //    if(room_.checkAllPlay(read_msg_.ca.id) && inplay)
                          //    {
                          //        deal = true;
                          //        room_.giveCard(-1);
                          //        room_.giveCard(-1);
                          //        std::string gui = room_.stringOfCards();
                          //        char g[gui.size() +1 ];
                          //        std::copy(gui.begin(), gui.end(), g);
                          //        g[gui.size()] = '\0';
                          //        strcpy(read_msg_.ca.g, g);
                          //        std::cout << "DONE" << std::endl;
                          //        break;
                          //    }
                          //  }
                          //  
                          //  });
                          //  //thread ends 
                          //  //t.join();
                          //} didnot work out well with threads
                      }

                      if(read_msg_.ca.hit == true)
                      {
                          room_.giveCard(read_msg_.ca.id); 
                          std::string gui = room_.stringOfCards();

                          char g[gui.size() +1 ];
                          std::copy(gui.begin(), gui.end(), g);
                          g[gui.size()] = '\0';
                          strcpy(read_msg_.ca.g, g);
                      }
                      else if(read_msg_.ca.stand == true)
                      {
                          if(turn < room_.sizeOfParticipants())
                          {
                            turn++;
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
                    read_msg_.encode_header(); // save info in msg to be sent to client
                    std::cout << "Here\n" << read_msg_.ca.turn << std::endl;
                    room_.deliver(read_msg_); // deliver msg to all clients


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
        double seconds_passed;
        double seconds_expire = 10;
        while(true)
        {
            if(servers.front().numPlayers() >= 1)
            {
                //start the clock when one player joins
                clock_t start = clock();
                while(true) 
                {
                    seconds_passed = (clock() - start)/CLOCKS_PER_SEC;
                    if(seconds_passed > seconds_expire)
                    {
                        std::cout << "Expired" << std::endl;
                        inplay = true;
                        servers.front().start_play();
                        break;
                    }
                }
                break;
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
