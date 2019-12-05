CXXFLAGS = --std=c++11
GTKFLAGS = $(shell pkg-config gtkmm-3.0 --cflags --libs)
CPPFLAGS = -I./asio-1.13.0/include

TARGETS = server client test_example

all:$(TARGETS) 

server: src/chat_server.cpp include/chat_message.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $< -lpthread -g -Wall

client: UI_Interface.o chat_client.o
	$(CXX) $(CXXFLAGS) -o client UI_Interface.o chat_client.o $(GTKFLAGS) -g -Wall

chat_client.o : src/chat_client.cpp include/chat_message.hpp include/controller.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -lpthread $(GTKFLAGS) -g -Wall 

UI_Interface.o : src/UI_Interface.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/UI_Interface.cpp $(GTKFLAGS) -g -Wall
	
	
test_example : test_example.cpp include/Hand.hpp include/Card.hpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -o $@ $< \
            -lboost_unit_test_framework

clean:
	-rm -f *.o *.gch *~ $(TARGETS)
