#include "../include/UI_Interface.h"
#include <gtkmm.h>
#include <string>
#include <sstream>

UI_Interface::UI_Interface(Controller* controller)
{
    UI_Interface::controller = controller;

    set_default_size(700, 700);
    set_title("Black Jack");

    vbox = Gtk::manage(new Gtk::VBox);
    add(*vbox);

    menubar = Gtk::manage(new Gtk::MenuBar());
    vbox->pack_start(*menubar, Gtk::PackOptions::PACK_SHRINK, 0); //pack_shrink packs it shrinked, 0 for padding

    Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
    menubar->append(*menuitem_file);

    Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
    menuitem_file->set_submenu(*filemenu);

    Gtk::MenuItem *menuitem_new = Gtk::manage(new Gtk::MenuItem("_New Game", true));
    menuitem_new->signal_activate().connect([this] { this->on_new_clicked(); });
    filemenu->append(*menuitem_new);

    grid = Gtk::manage(new Gtk::Grid);
    grid->set_hexpand(true);
    grid->set_vexpand(true); 

    // reverse for j because grid is added top down

    std::string tile_image = "images/cards/2 of S.jpg";

    Gtk::Button *hit_button = Gtk::manage(new Gtk::Button("HIT"));
    hit_button->signal_clicked().connect([this] { this->hit_button_pressed(); });
    //hit_button->set_device_enabled(Gdk::Device(*hit_button), false);
    buttons.push_back(hit_button);
    Gtk::Button *stand_button = Gtk::manage(new Gtk::Button("STAND"));
    stand_button->signal_clicked().connect([this] { this->stand_button_pressed(); });
    Gtk::Button *double_button = Gtk::manage(new Gtk::Button("DOUBLE"));
    Gtk::Button *split_button = Gtk::manage(new Gtk::Button("SPLIT"));
    split_button->signal_clicked().connect([this] { this->split_button_pressed(); });
    Gtk::Button *leave_button = Gtk::manage(new Gtk::Button("LEAVE"));
    buttons.push_back(stand_button);
    buttons.push_back(double_button);
    buttons.push_back(split_button);

    Gtk::Label *d_info = Gtk::manage(new Gtk::Label());
    d_info->set_label("Dealer :");
    grid->attach(*d_info,0,0,1,1);
    //grid->attach(*hit_button,6,3,1,1);
    //grid->attach(*stand_button,7,3,1,1);
    //grid->attach(*split_button,7,4,1,1);
    //grid->attach(*leave_button,8,3,1,1);

    grid->set_row_spacing(10);
    grid->set_column_spacing(10); 

    vbox->add(*grid);
    Gtk::HBox * hbox = Gtk::manage(new Gtk::HBox());
    hbox->add(*hit_button);
    hbox->add(*stand_button);
    hbox->add(*split_button);
    hbox->add(*leave_button);
    vbox->add(*hbox);

    Gtk::Scrollbar * scrollbar = Gtk::manage(new Gtk::Scrollbar());

    //vbox->add(*scrollbar);
    statusbar = Gtk::manage(new Gtk::Statusbar);
    status_label = Gtk::manage(new Gtk::Label());
    status_label->set_label("Waiting for players to join");
    statusbar->set_center_widget(*status_label);
    vbox->add(*statusbar);
    vbox->show_all();
    //hit_button->hide();
    //stand_button->hide();
    //double_button->hide();
    //split_button->hide();
}


UI_Interface::~UI_Interface() {}

void UI_Interface::on_button_clicked()
{

}

void UI_Interface::on_new_clicked()
{

}

void UI_Interface::set_id(int id)
{
    UI_Interface::pid = id;
}

void UI_Interface::hit_button_pressed()
{
    UI_Interface::controller->hit();
}

void UI_Interface::stand_button_pressed()
{
    std::cout << "Called" << std::endl;
    UI_Interface::controller->stand();
}

void UI_Interface::split_button_pressed()
{
    UI_Interface::controller->split();

}

void UI_Interface::redraw(std::string data, int turn, bool split)
{
    std::cout << turn << " Turn " << std::endl;
    if(turn != UI_Interface::pid)
    {
        for(auto button : UI_Interface::buttons)
        {
            button->set_sensitive(false);
            if(turn == -1)
                status_label->set_label("Dealer's Turn");
            else if(turn == 0)
                status_label->set_label("Waiting for players to join");
            else
                status_label->set_label("Player " + std::to_string(turn) + " Turn");
        }
    }
    else if(turn == UI_Interface::pid)
    {
        for(auto button : UI_Interface::buttons)
        {
            std::cout << "Called buttons: "<< button << std::endl; 
            button->set_sensitive(true);
            status_label->set_label("Your Turn");
        }
    }
    std::cout << "Got here" << std::endl;
    //only destroy and refresh if there
    //was any update  in the message
    std::cout << "LEN: " << data.length() << std::endl;
    if(data.length() >= 5) //used 5 just in case of some escaped \0 or \n or ' '
    {
        std::cout << "CALLED: :" << std::endl;
        for (auto c : _container)
        {
            grid->remove(*(c.image));
            delete (c.image); //destroy those images; they can overlap
            //if left alone
        }
        for ( auto l : labels)
        {
            grid->remove(*l);
            delete l;
        }
        _container.clear(); //clear up vector
        labels.clear();
        ids.clear();
        std::stringstream ss(data);//convert string to sstream
        int id = 0;
        int hid = 0;
        int i = 0;

        while(true)
        {
            std::string token;
            std::string s;
            if(!(ss>>token))
            {
                break;
            }
            if(token == "<--") //removes the line <-- Player x hand:
            {
                ss>>token;
                ss>>id;
                ids.push_back(id);
                ss>>token;
            }
            else if(token == "hand:")
            {
                ss>>hid;
            }
            else
            {
                s = "images/cards/";
                std::cout << token << std::endl;
                token = token.substr(0,1); //substr to remove trailing spaces
                s += token;
                std::getline(ss,token);
                token = token.substr(0,5); //substr to remove trailing spaces
                s += token;
                s += ".jpg";
                std::cout << s << " " << hid << std::endl;
                //images.insert(pair<int, Gtk::Image*>(id, Gtk::manage(new Gtk::Image(s))));
                _container.push_back(storage(id, hid, Gtk::manage(new Gtk::Image(s))));
            }
        }
        draw();
    }
}

void UI_Interface::draw()
{
    int column = 0;
    int prev = 0;
    int lastHand = 0;
    int i = 1;
    labels.clear();

    for (const auto& c : _container)
    {
        //c.id is player id
        std::cout << c.id << c.hid << prev << std::endl;
        if(prev != c.id)
        {
            if(c.id != 0) //if not dealer
            {
                column++;
                Gtk::Label *p_info = Gtk::manage(new Gtk::Label());
                std::string info = "Player: ";
                info += std::to_string(c.id);
                if(c.id == UI_Interface::pid)
                {
                    info = "Yours: ";
                }
                p_info->set_label(info);
                labels.push_back(p_info);
                grid->attach(*p_info,column,1,1,1);
                i = 1;
                lastHand = c.hid;
            }
            else
            {
                i = 0;
            }
            prev = c.id;
        }
        else if(lastHand != c.hid)
        {
            column++;
            lastHand = c.hid;
            i = 1;
        }
        if(c.id == 0) //dealer
        {
            //attach horizontally
            grid->attach(*(c.image), ++i, c.id, 1, 1);
        }
        else
        {
            grid->attach(*(c.image), column, ++i, 1, 1);
        }
    }
    vbox->show_all();
    std::cout << "Reached end" << std::endl;
}
