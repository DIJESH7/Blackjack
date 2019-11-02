#include "../include/UI_Interface.h"

UI_Interface::UI_Interface(Controller controller)
{
    set_default_size(500, 500);
    set_title("Black Jack");

    vbox = Gtk::manage(new Gtk::Vbox);
    add(*vbox);

    menubar = Gtk::manage(new Gtk::MenuBar());
    vbox->pack_start(*menubar, Gtk::PackOptions::PACK_SHRINK, 0);
    Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
    menubar->append(*menuitem_file);

    Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
    menuitem_file->set_submenu(*filemenu);

    Gtk::MenuItem *menuitem_new = Gtk::manage(new Gtk::MenuItem("_New Game", true));
    menuitem_new->signal_activate().connect([this] { this->on_new_clicked(); });
    filemenu->append(*menuitem_new);

    Gtk::Toolbar *toolbar = Gtk::manage(new Gtk::Toolbar);
    vbox->add(*toolbar);

    Gtk::ToolButton *new_game_button = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::NEW));
    new_game_button->set_tooltip_markup("Create a new name, discarding any in progress");
    new_game_button->signal_clicked().connect(sigc::mem_fun(*this, &Main_window::on_new_clicked));

    toolbar->append(*new_game_button);

    Statusbar* statusbar = Gtk::manage(new Gtk::Statusbar);
    Gtk::Label* label = Gtk::manage(new Gtk::Label());
    label->set_label("something");
    statusbar->set_center_widget(*label);
    vbox->add(*statusbar);
    vbox->show_all();

}

void UI_Interface::hit_button_pressed()
{

}
void UI_Interface::stand_button_pressed()
{

}
void UI_Interface::doubleDown_button_pressed()
{

}
void UI_Interface::split_button_pressed()
{

}
void UI_Interface::exit_button_pressed()
{

}
