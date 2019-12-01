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
        void doubledown_button_pressed(std::string msg);
        void split_button_pressed();
        void leave_button_pressed();
        void on_button_clicked();
        void on_new_clicked();
        void set_id(int id);
        void draw();
        void redraw(std::string data, int turn, bool split, bool doubledown, int * results, int size, int credits);
        void set_bet(std::string bet, int turn);
        void add_bet(int bet);
        void set_name(std::string name);
        void show_credit(int);

    private:
    std::vector<struct storage> _container;
    std::vector<int> ids;
    int pid;
    Gtk::Statusbar* statusbar;
    std::vector<Gtk::Label*> labels;
    std::vector<Gtk::Button*> buttons;
    Gtk::VBox* vbox;
    Gtk::Grid *grid;
    Gtk::MenuBar *menubar;
    Controller* controller;
    Gtk::Label * status_label;
    Gtk::Label * bet_label;
    Gtk::Label * name_label;
    Gtk::Label * credit_label;
    Gtk::Grid *F_grid;

};

struct storage
{
    int id;
    int hid;
    Gtk::Image* image;

    storage(int id_, int hid_, Gtk::Image* image_)
    {
        id = id_;
        hid = hid_;
        image = image_;
    }
};
