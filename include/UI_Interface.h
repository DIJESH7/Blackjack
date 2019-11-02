#pragma once

#include "Hand.h"
#include "controller.h"
#include <gtkmm.h>

using namespace std;

class UI_Interface : public Gtk::Window
{
    public:
        UI_Interface(Controller c);
        void hit_button_pressed();
        void stand_button_pressed();
        void doubleDown_button_pressed();
        void split_button_pressed();
        void exit_button_pressed();

    private:
        Controller _controller;

};
