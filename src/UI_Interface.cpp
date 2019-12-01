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

    bet_label = Gtk::manage(new Gtk::Label());
    bet_label->set_label("Bet: ");
    vbox->add(*bet_label);

    grid = Gtk::manage(new Gtk::Grid);
    grid->set_hexpand(true);
    grid->set_vexpand(true); 

    // reverse for j because grid is added top down

    std::string tile_image = "images/cards/2 of S.jpg";

    Gtk::Button *hit_button = Gtk::manage(new Gtk::Button("HIT"));
    hit_button->signal_clicked().connect([this] { this->hit_button_pressed(); });
    //hit_button->set_device_enabled(Gdk::Device(*hit_button), false);
    Gtk::Button *stand_button = Gtk::manage(new Gtk::Button("STAND"));
    stand_button->signal_clicked().connect([this] { this->stand_button_pressed(); });
    Gtk::Button *doubledown_button = Gtk::manage(new Gtk::Button("DOUBLE\n  DOWN"));
    doubledown_button->signal_clicked().connect([this] {this->doubledown_button_pressed(); });
    Gtk::Button *split_button = Gtk::manage(new Gtk::Button("SPLIT"));
    split_button->signal_clicked().connect([this] { this->split_button_pressed(); });
    Gtk::Button *leave_button = Gtk::manage(new Gtk::Button("LEAVE"));
    leave_button->signal_clicked().connect([this] { this->leave_button_pressed(); });
    buttons.push_back(hit_button);
    buttons.push_back(stand_button);
    buttons.push_back(doubledown_button);
    buttons.push_back(split_button);

    Gtk::Label *d_info = Gtk::manage(new Gtk::Label());
    d_info->set_label("Dealer :");
    grid->attach(*d_info,0,0,1,1);

    grid->set_row_spacing(10);
    grid->set_column_spacing(10); 

    vbox->add(*grid);
    Gtk::HBox * hbox = Gtk::manage(new Gtk::HBox());
    hbox->add(*hit_button);
    hbox->add(*stand_button);
    hbox->add(*split_button);
    hbox->add(*doubledown_button);
    hbox->add(*leave_button);
    vbox->add(*hbox);
    Glib::RefPtr<Gtk::Adjustment> adjustment = Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0);

    //Scrollbar cannot make to be at right of window
    //Gtk::Scrollbar * scrollbar = Gtk::manage(new Gtk::Scrollbar(adjustment, Gtk::ORIENTATION_VERTICAL));
    //vbox->add(*scrollbar);

    statusbar = Gtk::manage(new Gtk::Statusbar);
    status_label = Gtk::manage(new Gtk::Label());
    status_label->set_label("Waiting for players to join");
    statusbar->set_center_widget(*status_label);
    vbox->add(*statusbar);

    vbox->show_all();
}

UI_Interface::~UI_Interface() {}


void UI_Interface::set_bet(std::string bet, int id)
{
    if(id == UI_Interface::pid)
        bet_label->set_label("Bet: " + bet);

}
void UI_Interface::add_bet(int bet)
{
    bet_label->set_label(bet_label->get_text() + " " + std::to_string(bet));
}


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
    UI_Interface::controller->stand();
}

void UI_Interface::split_button_pressed()
{
    UI_Interface::controller->split();
}

void UI_Interface::doubledown_button_pressed()
{
    int bet;
    //make a dialog and ask for bet
    Gtk::Dialog * dialog = new Gtk::Dialog();
    dialog->set_title("Bet Amount");
    dialog->add_button("OK", -1);

    Gtk::Label * label = new Gtk::Label("How much more would you like to bet?");
    dialog->get_content_area()->pack_start(*label);
    label->show();

    Gtk::Entry * entry = new Gtk::Entry{};
    entry->set_text("1");
    entry->set_max_length(1);
    entry->show();
    dialog->get_vbox()->pack_start(*entry);
    dialog->run();
    std::string s = entry->get_text();
    std::cout << s << std::endl;
    dialog->close();
    delete dialog;
    delete entry;
    delete label;

    bet = std::stoi(s);
    UI_Interface::controller->doubledown(bet);
}

void UI_Interface::leave_button_pressed()
{
    UI_Interface::controller->leave();
}

void UI_Interface::redraw(std::string data, int turn, bool split, bool doubledown, int * results, int size)
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
            button->set_sensitive(true);
            status_label->set_label("Your Turn");
        }
        if(!split)
            buttons[3]->set_sensitive(false);
        if(!doubledown)
            buttons[2]->set_sensitive(false);
    }

    //only destroy and refresh if there
    //was any update  in the message
    std::cout << "LEN: " << data.length() << std::endl;
    if(data.length() >= 5) //used 5 just in case of some escaped \0 or \n or ' '
    {
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
                token = token.substr(0,1); //substr to remove trailing spaces
                s += token;
                std::getline(ss,token);
                token = token.substr(0,5); //substr to remove trailing spaces
                s += token;
                s += ".jpg";
                //images.insert(pair<int, Gtk::Image*>(id, Gtk::manage(new Gtk::Image(s))));
                _container.push_back(storage(id, hid, Gtk::manage(new Gtk::Image(s))));
            }
        }
        draw();
    }
    if(size > 0)
    {
        std::string wins = "";
        int i;
        for(i = 0; i < size; i++)
        {
            wins += "Hand#" + std::to_string(i) + " - ";
            if(results[i] == -1)
                wins += "Lost\n";
            else if(results[i] == 1)
                wins += "Won\n";
            else if(results[i] == 2)
                wins += "Draw, No bet deducted\n";
            else
                wins += "Game is not finished yet\n";
        }
        std::cout << wins << std::endl;
        Gtk::MessageDialog* dialog = new Gtk::MessageDialog(*this, "Results", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dialog->set_secondary_text(wins, false);
        dialog->run();
        delete dialog;
        status_label->set_text(wins);
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
                //TODO: maybe put a background color or something to know
                //which hand is active
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
}
