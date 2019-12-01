//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
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
UI_Interface* win;

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
                    std::cout << "\n\nWELCOME TO CASINO ROYALE!" << std::endl;
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
			gdk_threads_enter();
                        if(!gotId) //runs only first time
                        {
                          id = read_msg_.ca.id;
                          gotId = true;
                          win->set_id(id);
                          std::cout << "New Player connected with Player ID "<< id << std::endl;
                          std::cout << "Your Player id: "<< id << std::endl;
                        }
                        std::cout << std::endl;
                        //std::cout << read_msg_.ca.g << std::endl;
                        do_read_body();
                        storeData();
			gdk_threads_leave();
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

        void storeData()
        {
            std::string data(read_msg_.ca.g);
            std::cout << read_msg_.ca.size << " HEHHB" << std::endl;
            win->redraw(data, read_msg_.ca.turn, read_msg_.ca.split_button, read_msg_.ca.handWins, read_msg_.ca.size, read_msg_.ca.bet);
        }
    private:
        asio::io_context& io_context_;
        tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;

        std::string name;
        int id;
        bool gotId = false;
};

chat_client* c;


Controller::Controller()
{

}

Controller::~Controller()
{
    
}

void Controller::hit()
{
    chat_message msg;

    msg.body_length(0);

    // hitting 1 card
    msg.ca.hit = true;
    msg.ca.id = c->get_id();
    msg.ca.play = false;
    msg.ca.split = false;
    msg.encode_header(); // write hit
    c->write(msg);       // send hit
}

void Controller::split()
{
    //char line[chat_message::max_body_length + 1];
    //std::strcpy(line, "garbage");
    chat_message msg;

    msg.body_length(0);
    //std::memcpy(msg.body(), line, msg.body_length());

    msg.ca.hit = false;
    msg.ca.split = true;
    msg.ca.stand = false;
    msg.ca.id = c->get_id();
    msg.ca.play = false;
    msg.encode_header();
    c->write(msg);
}

void Controller::stand()
{
    //char line[chat_message::max_body_length + 1];
    //std::strcpy(line, "garbage");
    chat_message msg;

    msg.body_length(0);
    //std::memcpy(msg.body(), line, msg.body_length());

    msg.ca.hit = false;
    msg.ca.stand = true;
    msg.ca.id = c->get_id();
    msg.ca.play = false;
    msg.encode_header();
    c->write(msg);
}

void Controller::doubledown()
{
    chat_message msg;
    msg.body_length(0);
    msg.ca.id = c->get_id();
    msg.ca.doubledown = true;
    msg.encode_header();
    c->write(msg);
}

void Controller::leave()
{
    chat_message msg;

    msg.body_length(0);

    msg.ca.hit = false;
    msg.ca.stand = false;
    msg.ca.split = false;
    msg.ca.id = c->get_id();
    msg.ca.play = false;
    msg.ca.leave = true;
    msg.encode_header();
    c->write(msg);
    exit(0);
}

int main(int argc, char* argv[])
{

    gdk_threads_init();
    auto app = Gtk::Application::create("");
    gtk_init( &argc, &argv);
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
    Controller* controller = new Controller();
    win = new UI_Interface(controller); 

    std::thread t([&io_context](){ io_context.run(); });
    chat_message msg;
    msg.ca.client_credits = 100;

    // testing
    char ans;
    do
    {
        std::cout << "Play? y/n" << std::endl;
        std::cin >> ans;
    }
    while(ans != 'y');

    std::cout << "Bet amount? [max 3, min 1]" << std::endl;
    int amount;
    //TODO check for min and max input
    std::cin >> amount;
    msg.ca.bet = amount;
    Card temp;

    //start dealing
    msg.ca.play = true;
    msg.ca.id = c->get_id();
    msg.encode_header();
    c->write(msg);

    std::thread t2([&](){ app->run(*win); });

    //main keeps waiting for gui to exit...
    //only after gui closes, we close client
    t2.join();
    c->close();
    t.join();

    //catch (std::exception& e)
    //{
    //  std::cerr << "Exception: " << e.what() << "\n";
    //}
    delete c;
    delete controller;
    delete win;
    return 0;
}
