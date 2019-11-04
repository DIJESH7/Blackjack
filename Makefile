CXXFLAGS = --std=c++11
GTKFLAGS = $(shell pkg-config gtkmm-3.0 --cflags --libs)
CPPFLAGS = -I./asio-1.13.0/include

TARGETS = server client 

all:$(TARGETS) 

server: src/chat_server.cpp include/chat_message.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $< -lpthread -g -Wall

client: UI_Interface.o controller.o BJD.o BJP.o chat_client.o
	$(CXX) $(CXXFLAGS) -o client UI_Interface.o controller.o BJD.o BJP.o chat_client.o $(GTKFLAGS) -g -Wall

chat_client.o : src/chat_client.cpp include/chat_message.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -lpthread $(GTKFLAGS) -g -Wall 

UI_Interface.o : src/UI_Interface.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/UI_Interface.cpp $(GTKFLAGS) -g -Wall

controller.o : src/controller.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/controller.cpp $(GTKFLAGS) -g -Wall

BJD.o : src/BJD.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/BJD.cpp $(GTKFLAGS) -g -Wall

BJP.o : src/BJP.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/BJP.cpp $(GTKFLAGS) -g -Wall

clean:
	-rm -f *.o *.gch *~ $(TARGETS)
