# cse3310 project sample makefile


CXX=g++
# note the below asio package may be in a 
# different place
CPPFLAGS=-I../asio-1.13.0/include
CXXFLAGS=-Wall -g -std=c++11
#GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

GTKLINKFLAGS=$(shell pkg-config --libs gtk+-2.0)
GTKCOMPILEFLAGS=$(shell pkg-config --cflags gtk+-2.0)

TARGETS=chat_client chat_server

all:${TARGETS}


# these compile and link in the same command.
# no intermediate object code files

chat_client: chat_client.cpp chat_message.hpp Card.hpp Hand.hpp #Dialogs.hpp
	${CXX} ${CXXFLAGS} ${GTKCOMPILEFLAGS} ${CPPFLAGS} -o $@ $< \
	${GTKLINKFLAGS} -lpthread -lncurses

chat_server: chat_server.cpp chat_message.hpp Card.hpp Deck.hpp Hand.hpp BlackjackHand.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< -lpthread

clean: #call by command "make clean"
	-rm -f ${TARGETS}
