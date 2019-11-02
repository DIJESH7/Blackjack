CXX = g++
CXXFLAGS = --std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

blackjack: main.o ui_interface.o controller.o dealer.o player.o
	$(CXX) $(CXXFLAGS) -o blackjack main.o ui_interface.o controller.o dealer.o player.o $(GTKFLAGS) -g

main.o : src/main.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp $(GTKFLAGS) -g -Wall

ui_interface.o : UI_Interface.cpp *.h
	$(CXX) $(CXXFLAGS) -c UI_Interface.cpp $(GTKFLAGS) -g -Wall

controller.o : controller.cpp *.h
	$(CXX) $(CXXFLAGS) -c controller.cpp $(GTKFLAGS) -g -Wall

dealer.o : dealer.cpp *.h
	$(CXX) $(CXXFLAGS) -c dealer.cpp $(GTKFLAGS) -g -Wall

player.o : player.cpp *.h
	$(CXX) $(CXXFLAGS) -c player.cpp $(GTKFLAGS) -g -Wall

clean:
	-rm -f *.o *.gch *~ blackjack
