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
#include "chat_message2.hpp"
#include "Hand.hpp"

#include <cstdlib>

using asio::ip::tcp;


//------------------------------------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;



//------------------------------------------------------------------------------------------------


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
    srand((unsigned) time(0));
    id = rand() % 50;
    return id;
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

            //std::cout << "New Player connected with Player ID "<< register_client() << std::endl;
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
            { //std::cout << "first !ec statement" << std::endl;
                switch(read_msg_.gs.one)
                { //std::cout << "first switching statement" << std::endl;
                    case 1: // show welcoming
                        //view showWelcomepanel
                        break;
                    case 2: // get bet
                        //view set waiting false
                        //view set continueplaying false
                        //view show bet panel
                        //view set bet money label ( read_msg_.gs.two )
                        //view set bet min bet label ( read_msg_.gs.three )
                        
                        break;
                    case 3: // bet response
                        switch(read_msg_.gs.two)
                        {
                            case 1: // invalid
                                
                                break;
                            case 2: // too much
                            
                                break;
                            case 3: // min
                                
                                break;
                            case 4: // success
                                // view bet success()
                                // view set bet money label ( read_msg_.gs.three )
                                break;
                        }
                        break;
                    case 4: // new round
                        //view betwaiting (false)
                        //view show turn panel ()
                        // view show turn money label (read_msg_.gs.two)
                        break;
                    case 5: // blackjack
                        switch(read_msg_.gs.two)
                        {
                            case 1: // player and dealer
                                //view setBlackjacklabel("both have BJ");
                                break;
                            case 2: // player
                                //view setBlackjacklabel("you have BJ")
                                break;
                            case 3: // dealer
                                //view setBlackjacklabel(dealer has BJ)
                                break;
                            case 4:
                                //view setBlackjacklabel(deal no bJ)
                                break;
                        }
                        break;
                    case 6: // new dealer card
                        //view addDealerCard(model->getCardimageLabel(read_msg_.gs.two))
                        break;
                    case 7: // get insurace bet
                        //view enableInsuranceBet()
                        break;
                    case 8: // insurace bet response
                        switch (read_msg_.gs.two)
                        {
                            case 1: // error
                                //view insuracebetError
                                break;
                            case 2: // placed
                                //view insuracebetSuccess()
                                //view setmessagelabel("insurace bet: $" + read_msg_.gs.three)
                                //view setturnmoneylabel(read_msg_.gs.four)
                                break;
                            case 3: // not places
                                //view insuracebetSuccess()
                                //view removeInsuranceBetInfo()
                                break;
                        }
                        break;
                    case 9: // cannot insurace bet
                        //view setmessagelabel("you have not enough money to bet insure")
                        break;
                    case 10: // insurace bet won
                        //view setmessagelabel("you won $" + read_msg_.gs.two +"from insurace bet")
                        //view setturnmoneylabel(read_msg_.gs.three)
                    case 11: // insurace bet lost
                        //view setmessagelabel("you lost insurace bet")
                        break;
                    case 12: // insurace bet done
                        //view setInsuraceBetWaiting(false)
                        break;
                    case 13: // take turn
                        //view setturnwaiting(false)
                        //view removeInsuranceBetInfo()
                        break;
                    case 14: // new hand
                        //BlackjackPanel tempVar(this)
                        //model addPlayerHandPanel(stoi(read_msg_.gs.two), &tempVar)
                        //view addPlayerHandPanel(model getPlayerHandPanel(stoi(read_msg_.gs.two)), stoi(read_msg_.gs.two))
                        break;
                    case 15: // remove hand
                        //view removePlayerHandPanel(model getPlayerHandPanel(read_msg_.gs.two) )
                        //model removePlayerHandPanel(stoi(read_msg_.gs.two))
                        break;
                    case 16: // hand bet
                        //model getPlayerHandPanel(stoi(read_msg_.gs.two)) -> setHandBet(read_msg_.gs.three)
                        break;
                    case 17: // hand value
                        //model getPlayerHandPanel(stoi(read_msg_.gs.two)) -> sethandValueLabel(read_msg_.gs.three)
                        break;
                    case 18: // turn blackjack
                        switch(read_msg_.gs.two)
                        {
                            case 1: // player and dealer
                                //view setBlackjacklabel("both have BJ")
                                break;
                            case 2: // player
                                //view setBlackjacklabel("you have blackjack")
                                break;
                            case 3: // dealer
                                //view setBlackjacklabel("dealer has BJ")
                                break;
                        }
                        break;
                    case 19: // new player card
                        //model getPlayerHandPanel(stoi(read_msg_.gs.two)) -> addCard(model getCardimageLabel(read_msg_.gs.three))
                        break;
                    case 20: // turn option
                        switch(read_msg_.gs.two)
                        {
                            case 1: // both
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableSplitPairs()
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableDoubleDown()
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableHitStand()
                                break;
                            case 2: // split pairs
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableSplitPairs()
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableHitStand()
                                break;
                            case 3: // double down
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableDoubleDown()
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableHitStand()
                                break;
                            case 4: // neither
                                //model getPlayerHandPanel( stoi(read_msg_.gs.three)) -> enableHitStand()
                                break;
                        }
                        break;
                    case 21: // turn option error
                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.two))->turnError();
                        //getServerMessage();
                        break;
                    case 22: // bust
                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.two))->bust();
                        //getServerMessage();
                        break;
                    case 23: // split pair response
                        switch (read_msg_.gs.two)
                        {
                            case 1: // success
                                //view->setTurnMoneyLabel(read_msg_.gs.three);
                                //getServerMessage();
                                break;
                        }
                        break;
                    case 24: // double down response
                        switch (read_msg_.gs.two)
                        {
                            case 1: // success
                                //model->getPlayerHandPanel(std::stoi(read_msg_.gs.three))->doubleDownSuccess();
                                //view->setTurnMoneyLabel(read_msg_.gs.four);
                                //getServerMessage();
                                break;
                        }
                        break;
                    case 25: // send result
                        //view->setTurnWaiting(false);
                        //getServerMessage();
                        break;
                    case 26: // remove dealer face down card
                        //view->removeDealerFaceDownCard();
                        //getServerMessage();
                        break;
                    case 27: // dealer hand value
                        //view->setDealerHandValueLabel(read_msg_.gs.two);
                        //getServerMessage();
                        break;
                    case 28: // remove dealer doubledown facedowncard
                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.two))->removeDoubleDownFaceDownCard();
                        //getServerMessage();
                        break;
                    case 29: // round result
                        switch (read_msg_.gs.two)
                        {
                            case 1: // bust
                                switch (read_msg_.gs.three)
                                {
                                    case 1: // tie
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"You and the dealer both busted. It's a tie!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                    case 2: // dealer
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"You busted. The dealer wins!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                    case 3: // player
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"The dealer busted. You win!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                }
                                break;
                            case 2: // normal
                                switch (read_msg_.gs.three)
                                {
                                    case 1: // tie
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"It's a tie!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                    case 2: // dealer
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"The dealer wins!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                    case 3: // player
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"You win!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                }
                                break;
                            case 3: // blackjack
                                switch (read_msg_.gs.three)
                                {
                                    case 1: // tie
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"You and the dealer both have Blackjack. It's a tie!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                    case 2: // dealer
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"The dealer has Blackjack. The dealer wins!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                    case 3: // player
                                        //model->getPlayerHandPanel(std::stoi(read_msg_.gs.four))->setHandMessageLabel(L"You have Blackjack. You win!");
                                        //view->setTurnMoneyLabel(read_msg_.gs.five);
                                        //getServerMessage();
                                        break;
                                }
                                break;
                        }
                        break;
                    case 30: // get continue playing
                        //view->enableContinuePlaying();
                        //getServerMessage();
                        break;
                    case 31: // continue playing response
                        switch (read_msg_.gs.two)
                        {
                            case 1: // error
                                //view->continuePlayingError();
                                //getServerMessage();
                                break;
                            case 2: // continue
                                //view->reset();
                                //model->reset();
                                //view->showContinuePlayingPanel();
                                //getServerMessage();
                                break;
                        }
                        break;
                    case 32: // gsmeover
                        //view->showContinuePlayingPanel();
                        //iew->setContinuePlayingMoneyLabel(read_msg_.gs.two);
                        //view->gsmeOver();
                        //getServerMessage();
                        break;
                    case 33: // waiting
                        switch (read_msg_.gs.two)
                        {
                            case 1: // welcome
                                //view->setWelcomeWaiting(true);
                                //view->setContinuePlayingWaiting(true);
                                //getServerMessage();
                                break;
                            case 2: // bet
                                //view->setBetWaiting(true);
                                //getServerMessage();
                                break;
                            case 3: // insurance bet
                                //view->setInsuranceBetWaiting(true);
                                //getServerMessage();
                                break;
                            case 4: // turn
                                //view->setTurnWaiting(true);
                                //getServerMessage();
                                break;
                        }
                        break;
                    default:
                        //std::cout << "Unknown message received from server" << std::endl;
                        break;


                }
                
                //system("clear");
                std::cout << read_msg_.ca.g << std::endl;
                std::cout << std::endl;
                std::cout << "Options ";
            if(read_msg_.ca.checkHit == true)
            {
                std::cout << "(1)Hit ";
            }
            if(read_msg_.ca.checkStand == true)
            {
                std::cout << "(2)Stand ";
            }
            if(read_msg_.ca.checkDouble == true)
            {
                std::cout << "(3)Double Down ";
            }
            if(read_msg_.ca.checkSplit == true)
            {
                std::cout << "(4)Split ";
            }
            if(read_msg_.ca.checkInsure == true)
            {
                std::cout << "(6)Insure Bet ";
            }
            if(read_msg_.ca.checkLeave == true)
            {
                std::cout << "(7)Leave ";
            }
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
    int id;
    bool initdeal = false;
    //BlackjackModel model;
    //BlackjackView view;
};



chat_client * c;



int main(int argc, char* argv[])
{/*
    try
    { */


        if (argc != 3)
        {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);

        //chat_client c(io_context, endpoints);
        c = new chat_client(io_context, endpoints);
        assert(c);

        std::thread t([&io_context](){ io_context.run(); });


        ///////////////////////////////////////////////////////////////////////////////

    



        std::cout << "Welcome to Casino Royale!\n" << std::endl;
        std::cout << "Enter your name: ";
        std::string pname;
        std::cin >> pname;

        std::cout << "\nPlayer joining..." << std::endl;
       

        
            chat_message msg;
            msg.body_length(0);
            msg.ca.join = true;
            if(pname.length() < 25)
            {
                char temp[pname.size() +1];
                std::copy(pname.begin(), pname.end(), temp);
                temp[pname.size()] = '\0';
                strcpy(msg.ca.name, temp);             
            }
            msg.encode_header();
            assert(c);
            c->write(msg); // sends name

            msg.ca.join = false;
            usleep(100000);

            std::cout << "Enter bet amount:";
            double b;
            std::cin >> b;
            msg.ca.bet = true;
            msg.encode_header();
            assert(c);
            c->write(msg); // sends bet

            msg.ca.bet = false;
      
        
/*
        bool checkHit = true;
        bool checkStand = true;
        bool checkDouble = true;
        bool checkSplit = true;
        bool checkInsure = true;
*/

        int choice = -1;
        while(choice != 0)
        {
            msg.ca.hit = false;
            msg.ca.stand = false;
            msg.ca.doubledown = false;
            msg.ca.split = false;
            msg.ca.surrender = false;
            msg.ca.insurance = false;

            

            //std::cout << "Options (1)hit (2)stand (3)double (4)split (5)surrender (6)insurance (0)leave" << std::endl;
            std::cin >> choice;
            std::cerr << "---> button pressed: " << choice << std::endl;
            msg.body_length (0);
            switch(choice)
            {
                case 1:
                    msg.ca.hit = true;
                    msg.encode_header();
                    c->write(msg);
                    break;
                case 2:
                    msg.ca.stand = true;
                    msg.encode_header();
                    c->write(msg);
                    break;
                case 3:
                    msg.ca.doubledown = true;
                    msg.encode_header();
                    c->write(msg);
                    break;
                case 4:
                    msg.ca.split = true;
                    msg.encode_header();
                    c->write(msg);
                case 5:
                    msg.ca.surrender = true;
                    msg.encode_header();
                    c->write(msg);
                case 6:
                    msg.ca.insurance = true;
                    msg.encode_header();
                    c->write(msg);
                case 7:
                    msg.ca.leave = true;
                    msg.encode_header();
                    c->write(msg);
            }
        }
                

    
        
        
        /////////////////////////////////////////////////////////////////////////////


        c->close();
        t.join();
        /*
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    */

    return 0;
}
