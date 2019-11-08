#pragma once
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "../include/chat_message.hpp"
#include "../include/Hand.hpp"
#include "../include/UI_Interface.h"
#include <cstdlib>

using asio::ip::tcp;


//------------------------------------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;



//------------------------------------------------------------------------------------------------


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

        void register_client()
        {
            //  ask name
            // all clients have 7
            std::cout << "\n\nWELCOME TO CASINO ROYALE!" << std::endl;

            id = rand() % 50;
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
                    register_client();
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
                    std::cout << " getting card:" << read_msg_.card.value << std::endl;
                    h.addCard(read_msg_.card);
                    std::cout.write(read_msg_.body(), read_msg_.body_length());
                    std::cout << "\n";
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



    private:
        asio::io_context& io_context_;
        tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;

        std::string name;
        int id = 0;
};

extern chat_client* c;

class Connector
{
    public:
        Connector() {}

        static void hit(int id)
        {
            char line[chat_message::max_body_length + 1];
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
        }
};
