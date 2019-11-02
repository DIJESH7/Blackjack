#include "main_window.h"
#include <gtkmm.h>
#include <string>

Main_window::Main_window()
{
    set_default_size(700, 700);
    set_title("BJ");

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

    /*Gtk::Toolbar *toolbar = Gtk::manage(new Gtk::Toolbar);
    vbox->add(*toolbar);

    Gtk::ToolButton *new_game_button = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::NEW));
    new_game_button->set_tooltip_markup("Create a new name, discarding any in progress");
    new_game_button->signal_clicked().connect(sigc::mem_fun(*this, &Main_window::on_new_clicked));
    toolbar->append(*new_game_button);*/


    

    grid = Gtk::manage(new Gtk::Grid);
    grid->set_hexpand(true);
    grid->set_vexpand(true); 

    // reverse for j because grid is added top down

    
        	//Gtk::Button *button = Gtk::manage(new Gtk::Button());

    		std::string tile_image = "images/cards/2_of_spades.png";

    		Gtk::Button *hit_button = Gtk::manage(new Gtk::Button("HIT"));
    		Gtk::Button *stand_button = Gtk::manage(new Gtk::Button("STAND"));
    		Gtk::Button *double_button = Gtk::manage(new Gtk::Button("DOUBLE"));
    		Gtk::Button *split_button = Gtk::manage(new Gtk::Button("SPLIT"));
    		Gtk::Button *leave_button = Gtk::manage(new Gtk::Button("LEAVE"));

    		Gtk::Label *d_info = Gtk::manage(new Gtk::Label());
    		d_info->set_label("Dealer :");

    		int i;
    		std::string l;

        	        	
                      
            Gtk::Image *d_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *d_second_card = Gtk::manage(new Gtk::Image(tile_image));

            Gtk::Image *d_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *d_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

//-----------------------------------------------------------------------------------------

            Gtk::Image *p1_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p1_second_card = Gtk::manage(new Gtk::Image(tile_image));

            Gtk::Image *p1_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p1_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

//-----------------------------------------------------------------------------------------

            Gtk::Image *p2_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p2_second_card = Gtk::manage(new Gtk::Image(tile_image));

            Gtk::Image *p2_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p2_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

//-----------------------------------------------------------------------------------------

            Gtk::Image *p3_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p3_second_card = Gtk::manage(new Gtk::Image(tile_image));

            Gtk::Image *p3_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p3_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

//-----------------------------------------------------------------------------------------

            Gtk::Image *p4_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p4_second_card = Gtk::manage(new Gtk::Image(tile_image));

            Gtk::Image *p4_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p4_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

//-----------------------------------------------------------------------------------------

            Gtk::Image *p5_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p5_second_card = Gtk::manage(new Gtk::Image(tile_image));

            Gtk::Image *p5_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p5_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

//-----------------------------------------------------------------------------------------

            Gtk::Image *p6_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p6_second_card = Gtk::manage(new Gtk::Image(tile_image));

            Gtk::Image *p6_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
            Gtk::Image *p6_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

//-----------------------------------------------------------------------------------------


            //button->set_image(*image);            
            //button->signal_clicked().connect([this, i, j, button] { this->on_button_clicked(i, j, button); });
           

            grid->attach(*d_info,1,1,1,1);
            grid->attach(*d_first_card,2,1,1,1);
            grid->attach(*d_second_card,3,1,1,1);    

            

            for(i=1;i<6;i++){
            label = Gtk::manage(new Gtk::Label());
            l = std::to_string(i);
            label->set_label("player" + l);
           	grid->attach(*label,i,2,1,1);
            }

            grid->attach(*hit_button,6,3,1,1);

            grid->attach(*p1_first_card,1,3,1,1);
            grid->attach(*p2_first_card,2,3,1,1);
            grid->attach(*p3_first_card,3,3,1,1);
            grid->attach(*p4_first_card,4,3,1,1);
            grid->attach(*p5_first_card,5,3,1,1);

            grid->attach(*stand_button,7,3,1,1);

            grid->attach(*p1_second_card,1,4,1,1);
            grid->attach(*p2_second_card,2,4,1,1);
            grid->attach(*p3_second_card,3,4,1,1);
            grid->attach(*p4_second_card,4,4,1,1);
            grid->attach(*p5_second_card,5,4,1,1);            

            grid->attach(*leave_button,8,3,1,1);

            grid->set_row_spacing(10);
            grid->set_column_spacing(10); 

    vbox->add(*grid);
    

    /*statusbar = Gtk::manage(new Gtk::Statusbar);    
    label->set_label("whats up");
    statusbar->set_center_widget(*label);
    vbox->add(*statusbar);*/
    vbox->show_all();

    		grid->attach(*d_sp_first_card,4,1,1,1);
            grid->attach(*d_sp_second_card,5,1,1,1);

    		grid->attach(*double_button,6,4,1,1);

            grid->attach(*p1_sp_first_card,1,5,1,1);
            grid->attach(*p2_sp_first_card,2,5,1,1);
            grid->attach(*p3_sp_first_card,3,5,1,1);
            grid->attach(*p4_sp_first_card,4,5,1,1);
            grid->attach(*p5_sp_first_card,5,5,1,1);

            grid->attach(*split_button,7,4,1,1);

            grid->attach(*p1_sp_second_card,1,6,1,1);
            grid->attach(*p2_sp_second_card,2,6,1,1);
            grid->attach(*p3_sp_second_card,3,6,1,1);
            grid->attach(*p4_sp_second_card,4,6,1,1);
            grid->attach(*p5_sp_second_card,5,6,1,1);

            //p1_sp_first_card->show();
            //p1_sp_second_card->show();
            
            split_button->show();

        


}

Main_window::~Main_window() {}


void Main_window::on_button_clicked(int i, int j, Gtk::Button* button){

}
/*void Main_window::on_button_clicked(int i, int j, Gtk::Button* button)
{
    game.onButtonClicked(i, j, button);
    label->set_label(game.getMessage());
    int w = game.checkStatus();
    if( w == 1)
    {
        Gtk::MessageDialog* dialog = Gtk::manage(new Gtk::MessageDialog(*this, "Winner", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true));
        dialog->set_message("PLAYER 1 WON", false);
        dialog->run();
        // label->set_label("PLAYER 1 WON");
    }
    if( w == 2)
    {
        Gtk::MessageDialog* dialog = Gtk::manage(new Gtk::MessageDialog(*this, "Winner", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true));
        dialog->set_message("PLAYER 2 WON", false);
        dialog->run();
        // label->set_label("PLAYER 2 WON");
    }
    // rotateBoard();
}*/
void Main_window::on_new_clicked(){

}
/*void Main_window::on_new_clicked()
{
    game = *(new Game());
    board = game.getBoard();
    vbox->remove(*grid);
    vbox->remove(*statusbar);
    grid = board.getGrid();
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Gtk::Button *button = Gtk::manage(new Gtk::Button());
            std::string tile_image = "images/black_square.png";
            Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
            css_provider->load_from_data("button {background-image: image(#4cc875);}\
                                            button:active {background-image: image(#30b5ec)}");
            button->get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
            //if i and j are not both even or both odd
            if ((i + j) % 2 != 0)
            {
                css_provider->load_from_data("button {background-image: image(#f3f48c);}");
                button->get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
                tile_image = "images/white_square.png";   
            }

            for (auto piece: board.getPieces())
            {
                if(piece->isLocated(i, j))
                {
                    tile_image = piece->getImageName();
                }
            }
            Gtk::Image *image = Gtk::manage(new Gtk::Image{tile_image});

            button->set_image(*image);
            button->signal_clicked().connect([this, i, j, button] { this->on_button_clicked(i, j, button); });
            grid->attach(*button, i, 7-j, 1, 1);
            buttons.push_back(button);
        }
    }
    vbox->add(*grid);
    label->set_label(game.getMessage());
    vbox->add(*statusbar);
    vbox->show_all();
}*/

// void Main_window::rotateBoard()
// {
//     vbox->remove(*grid);
//     vbox->remove(*statusbar);
//     grid = Gtk::manage(new Gtk::Grid());
//     for(int i = 0; i < 8; i++)
//     {
//         for(int j = 0; j < 8; j++)
//         {
//             Gtk::Button *button = Gtk::manage(new Gtk::Button());
//             std::string tile_image = "images/black_square.png";
//             Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
//             css_provider->load_from_data("button {background-image: image(#4cc875);}\
//                                             button:active {background-image: image(#30b5ec)}");
//             button->get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
//             //if i and j are not both even or both odd
//             if ((i + j) % 2 != 0)
//             {
//                 css_provider->load_from_data("button {background-image: image(#f3f48c);}");
//                 button->get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
//                 tile_image = "images/white_square.png";   
//             }

//             for (auto piece: board.getPieces())
//             {
//                 if(piece->isLocated(i, j))
//                 {
//                     tile_image = piece->getImageName();
//                 }
//             }
//             Gtk::Image *image = Gtk::manage(new Gtk::Image{tile_image});

//             button->set_image(*image);
//             button->signal_clicked().connect([this, i, j, button] { this->on_button_clicked(i, j, button); });
//             grid->attach(*button, 7- i, j, 1, 1);
//             buttons.push_back(button);
//         }
//     }
//     vbox->add(*grid);
//     label->set_label(game.getMessage());
//     vbox->add(*statusbar);
//     vbox->show_all();
// }  Not so sure about this approach, too much switching grids,, prolly have two grids, show diff each time

// void Main_window::onLoadClicked()
// {
//     try() //
// }
