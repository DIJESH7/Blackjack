#pragma once

#include "Hand.hpp"
#include "controller.h"
#include <gtkmm.h>

using namespace std;

class UI_Interface : public Gtk::Window
{
    public:
        UI_Interface(Controller* controller);
        ~UI_Interface();
        void hit_button_pressed();
        void stand_button_pressed();
        void doubleDown_button_pressed();
        void split_button_pressed();
        void exit_button_pressed();
        void on_button_clicked();
        void on_new_clicked();
        void set_id(int id);

    private:
    int id;
    Gtk::Statusbar* statusbar;
    Gtk::Label* label;
    std::vector<Gtk::Button*> buttons;
    Gtk::VBox* vbox;
    Gtk::Grid *grid;
    Gtk::MenuBar *menubar;
    Controller* controller;

};
