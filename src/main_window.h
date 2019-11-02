#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <iostream>


class Main_window : public Gtk::Window
{
    public:
    Main_window();
    virtual ~Main_window();
    void on_button_clicked(int x, int y, Gtk::Button* button);
    void on_new_clicked();
    // void rotateBoard();
    // void onLoadClicked();
    // void onSaveClicked();

    private:
    Gtk::Statusbar* statusbar;
    Gtk::Label* label;
    std::vector<Gtk::Button*> buttons;
    Gtk::VBox* vbox;
    Gtk::Grid *grid;
    Gtk::MenuBar *menubar;
};

#endif