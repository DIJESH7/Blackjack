//
// chat_client.cpp
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
#include <thread>
#include "asio.hpp"
#include "../include/chat_message.hpp"
#include "../include/Hand.hpp"
#include "../include/UI_Interface.h"
#include "../include/controller.h"

using asio::ip::tcp;


//------------------------------------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

Hand h;

//------------------------------------------------------------------------------------------------

class chat_client
{
    public:
        chat_client(asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints)
            : io_context_(io_context),
            socket_(io_context)
    {
        do_connect(endpoints);
    }

        // clients call do_write
        void write(const chat_message& msg)
        {
            asio::post(io_context_,
                    [this, msg]()
                    {
                    bool write_in_progress = !write_msgs_.empty();
                    write_msgs_.push_back(msg);
                    if (!write_in_progress)
                    {
                    do_write();
                    }
                    });
        }

        void close()
        {
            asio::post(io_context_, [this]() { socket_.close(); });
        }

        int register_client()
        {
            //  ask name
            // all clients have 7
            std::cout << "\n\nWELCOME TO CASINO ROYALE!" << std::endl;
            srand((unsigned) time(0));
            id = rand() % 50;
            return id;
            std::cout << "given player id:" << id << std::endl;
        }

        int get_id()
        {
            return id;
        }

    private:
        void do_connect(const tcp::resolver::results_type& endpoints)
        {
            asio::async_connect(socket_, endpoints,
                    [this](std::error_code ec, tcp::endpoint)
                    {
                    if (!ec)
                    {
                    std::cout << "New Player connected with Player ID "<< register_client() << std::endl;
                    do_read_header();
                    }
                    });
        }

        void do_read_header()
        {
            asio::async_read(socket_,
                    asio::buffer(read_msg_.data(), chat_message::header_length),
                    [this](std::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec && read_msg_.decode_header())
                    {
                    //system("clear");
                    std::cout << "Your Player id:"<< id << std::endl;
                    std::cout << std::endl;
                    std::cout << read_msg_.ca.g << std::endl;
                    do_read_body();
                    }
                    else
                    {
                    socket_.close();
                    }
                    });
        }

        void do_read_body()
        {
            asio::async_read(socket_,
                    asio::buffer(read_msg_.body(), read_msg_.body_length()),
                    [this](std::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec)
                    {
                    //std::cout << " getting card:" << read_msg_.card.value << std::endl;
                    //h.addCard(read_msg_.card);
                    //std::cout.write(read_msg_.body(), read_msg_.body_length());
                    //std::cout << "\n";
                    do_read_header();
                    }
                    else
                    {
                    socket_.close();
                    }
                    });
        }

        void do_write()
        {
            cout << "got called\n";
            asio::async_write(socket_,
                    asio::buffer(write_msgs_.front().data(),
                        write_msgs_.front().length()),
                    [this](std::error_code ec, std::size_t /*length*/)
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
                    socket_.close();
                    }
                    });
        }

    private:
        asio::io_context& io_context_;
        tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;

        std::string name;
        int id;
};

chat_client* c;


Controller::Controller()
{

}

void Controller::hit()
{
    char line[chat_message::max_body_length + 1];
    //std::strcpy(line, "garbage");
    chat_message msg;

    msg.body_length(std::strlen(line));
    std::memcpy(msg.body(), line, msg.body_length());

    // testing
    std::cout << "Starting round." << std::endl;
    // hitting 1 card
    msg.ca.hit = true;
    msg.encode_header(); // write hit
    c->write(msg);       // send hit
    // hitting 1 card
    msg.encode_header(); // write hit
    c->write(msg);       // send hit
    std::cout << "controller c:" << c << std::endl;
}

int main(int argc, char* argv[])
{

    auto app = Gtk::Application::create("");
    //gtk_init( &argc, &argv);
    if (argc != 3)
    {
        std::cerr << "Usage: chat_client <host> <port>\n";
        return 1;
    }

    asio::io_context io_context; 
    std::cout << "Play" << std::endl;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    c = new chat_client(io_context, endpoints);
    std::cout << "c:" << c << std::endl;
    Controller* controller = new Controller();
    UI_Interface win(controller); 

    std::thread t([&io_context](){ io_context.run(); });
    chat_message msg;
    msg.ca.client_credits = 100;

    msg.body_length(std::strlen(line));
    std::memcpy(msg.body(), line, msg.body_length());

    // testing
    std::cout << "Play? y/n" << std::endl;
    char ans;
    std::cin >> ans;

    if (ans == 'y')
    {
        std::cout << "Bet amount? [max 3, min 1]" << std::endl;
        int amount;
        //TODO check for min and max input
        std::cin >> amount;
        msg.ca.bet = amount;
        Card temp;

        //start dealing
        msg.ca.play = true;
        msg.encode_header();
        c.write(msg);
    }

    std::thread t2([&](){ app->run(win); });

    char line[chat_message::max_body_length + 1];

    //while(std::cin.getline(line, chat_message::max_body_length + 1))
    {
        //cout << "Called\n";
        //chat_message msg;

        //msg.body_length(std::strlen(line));
        //std::memcpy(msg.body(), line, msg.body_length());

        //// testing
        //std::cout << "Starting round." << std::endl;
        //// hitting 1 card
        //msg.ca.hit = true;
        //msg.encode_header(); // write hit
        //c->write(msg);       // send hit
        //// hitting 1 card
        //msg.encode_header(); // write hit
        //c->write(msg);       // send hit
    }

    //main keeps waiting for gui to exit...
    //only after gui closes, we close client
    t2.join();
    c->close();
    t.join();

    //catch (std::exception& e)
    //{
    //  std::cerr << "Exception: " << e.what() << "\n";
    //}

    return 0;
}
