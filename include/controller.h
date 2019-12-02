#pragma once
#include <iostream>

class Controller
{
    public:
        Controller();
        void hit();
        void stand();
        void doubledown(int bet);
        void split();
        void leave();
        void new_game();
        ~Controller();

    private:

};
