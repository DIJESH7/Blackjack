#ifndef UI_INTERFACE
#define UI_INTERFACE

#include <gtkmm.h>
#include <iostream>


class UI_Interface : public Gtk::Window
{
    public:
    UI_Interface();
    virtual ~UI_Interface();
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
