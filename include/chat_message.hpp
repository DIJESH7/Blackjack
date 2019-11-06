//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "Card.hpp"

// these two classes are examples of sending an
// entire structure as part of the header
class game_state
{
    public:
        bool valid;
        int dealer_points;
        int something[3];
        // note you can't use std::string 
        // or pointers
};

class client_action
{
    public:
        bool valid;
        bool hit;   
        bool stand;
        bool doubledown;
        bool split;
        bool surrender;
        bool play;
        char C[5];
};



class chat_message
{
    public:
        // header_length will help make space for typed terminal string, client_action data, and Card data
        enum { header_length = 4 + sizeof(client_action) + sizeof(Card) };
        enum { max_body_length = 512 };

        chat_message()
            : body_length_(0)
        {
        }

        const char* data() const
        {
            return data_;
        }

        char* data()
        {
            return data_;
        }

        std::size_t length() const
        {
            return header_length + body_length_;
        }

        const char* body() const
        {
            return data_ + header_length;
        }

        char* body()
        {
            return data_ + header_length;
        }

        std::size_t body_length() const
        {
            return body_length_;
        }

        void body_length(std::size_t new_length)
        {
            body_length_ = new_length;
            if (body_length_ > max_body_length)
                body_length_ = max_body_length;
        }


        bool decode_header()
        {
            char header[header_length + 1] = "";
            std::strncat(header, data_, 4); // Appends the first 4 characters of data_ to header
            body_length_ = std::atoi(header); // length of header

            char *p = data_ + 4; // skip the integer || p now has [client_action data, Card data]

            // (dest, source, sizeof dest)
            std::memcpy(&ca,p,sizeof(client_action)); //copy client_action data from p to ca
            std::memcpy(&card,p+sizeof(client_action),sizeof(Card)); // copy Card data from p to card

            if (body_length_ > max_body_length)
            {
                body_length_ = 0;
                return false;
            }
            return true;
        }

        void encode_header()
        {
            char header[4 + 1] = "";
            std::sprintf(header, "%4d", static_cast<int>(body_length_));
            std::memcpy(data_, header, header_length);

            char *p = data_+4; // skip over the int we just extracted
            std::memcpy(p,&ca,sizeof(client_action)); // copy client_action data from ca to p
            std::memcpy(p+sizeof(client_action),&card,sizeof(Card)); //skip above and copy Card data from card to p
        }

    private:
        char data_[header_length + max_body_length];
        std::size_t body_length_;
    public:
        client_action ca;
        game_state gs;
        Card card;
};
