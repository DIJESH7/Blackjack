CXX = g++
CXXFLAGS = --std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

blackjack: main.o ui_interface.o controller.o BJD.o BJP.o
	$(CXX) $(CXXFLAGS) -o blackjack main.o ui_interface.o controller.o BJD.o BJP.o $(GTKFLAGS) -g

main.o : src/main.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp $(GTKFLAGS) -g -Wall

ui_interface.o : src/UI_Interface.cpp include*.h
	$(CXX) $(CXXFLAGS) -c src/UI_Interface.cpp $(GTKFLAGS) -g -Wall

controller.o : src/controller.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/controller.cpp $(GTKFLAGS) -g -Wall

BJD.o : src/BJD.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/BJD.cpp $(GTKFLAGS) -g -Wall

BJP.o : src/BJP.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/BJP.cpp $(GTKFLAGS) -g -Wall

clean:
	-rm -f *.o *.gch *~ blackjack
