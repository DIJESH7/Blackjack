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
    private:


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
                handshake.encode_header();
                participant->deliver(handshake);
            }
            else
            {
                participants_.insert(participant);
                handshake.ca.id = participant->id;
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
            return result;
        }


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

                    if(read_msg_.ca.play == true) // fix so that one client doesn't start the game
                    {
                    room_.giveCard(-1);
                    room_.giveCard(-1);

                    std::string gui = room_.stringOfCards();

                    char g[gui.size() +1 ];
                    std::copy(gui.begin(), gui.end(), g);
                    g[gui.size()] = '\0';
                    strcpy(read_msg_.ca.g, g);
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

                    read_msg_.encode_header(); // save info in msg to be sent to client
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
        clock_t start = clock();
        double seconds_passed;
        double seconds_expire = 10;
        while(true)
        {
            seconds_passed = (clock() - start)/CLOCKS_PER_SEC;
            if(seconds_passed > seconds_expire)
            {
                std::cout << "Expired" << std::endl;
                inplay = true;
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
