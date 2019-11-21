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
    grid->attach(*hit_button,6,3,1,1);
    grid->attach(*stand_button,7,3,1,1);
    grid->attach(*split_button,7,4,1,1);
    grid->attach(*leave_button,8,3,1,1);

    grid->set_row_spacing(10);
    grid->set_column_spacing(10); 

    vbox->add(*grid);

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
        }
    }
    else if(turn == UI_Interface::pid)
    {
        for(auto button : UI_Interface::buttons)
        {
            std::cout << "Called buttons: "<< button << std::endl; 
            button->set_sensitive(true);
        }
    }
    std::cout << "Got here" << std::endl;
    //only destroy and refresh if there
    //was any update  in the message
    std::cout << "LEN: " << data.length() << std::endl;
    if(data.length() >= 5) //used 5 just in case of some escaped \0 or \n or ' '
    {
        std::cout << "CALLED: :" << std::endl;
        for (auto image : images)
        {
            std::cout << image.second << std::endl;
            grid->remove(*(image.second));
            delete (image.second); //destroy those images; they can overlap
            //if left alone
        }
        images.clear(); //clear up vector
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
                std::cout << s << std::endl;
                images.insert(pair<int, Gtk::Image*>(id, Gtk::manage(new Gtk::Image(s))));
		//set_from_resource
            }
        }
        draw();
    }
}

void UI_Interface::draw()
{
    int prev = 0;
    int i = 1;
    labels.clear();

    for (const auto& image : images)
    {
        //image.first is player id
        if(prev != image.first)
        {
            if(image.first != 0) //if not dealer
            {
                Gtk::Label *p_info = Gtk::manage(new Gtk::Label());
                std::string info = "Player: ";
                info += std::to_string(image.first);
                if(image.first == UI_Interface::pid)
                {
                    info = "Yours: ";
                }
                p_info->set_label(info);
                grid->attach(*p_info,image.first,1,1,1);
                prev = image.first;
                i = 1;
            }
            else
            {
                i = 0;
            }
        }
        if(image.first == 0) //dealer
        {
            //attach horizontally
            grid->attach(*(image.second), i++, image.first, 1, 1);
        }
        else
        {
            grid->attach(*(image.second), image.first, ++i, 1, 1);
        }
    }
    std::cout << "Almost there" << std::endl;
    vbox->show_all();
    std::cout << "Reached end" << std::endl;
}
