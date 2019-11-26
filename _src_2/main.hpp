
#include <vector>
#include <string>
#include <iostream>
#include <gtkmm.h>

#include "BlackjackView.hpp"
#include "BlackjackModel.hpp"
#include "BlackjackController.hpp"


using namespace std;

int main(int argc, char *argv[]) 
{

 	Gtk::Main kit(argc, argv);
	chat_client client;
	BlackjackView view (client);
    BlackjackController controller (client, view);
    controller.cli();


	Glib::RefPtr<Gtk::Application> app = 
    Gtk::Application::create(argc, argv, "3310Project");
    Mainwin win;

    vector<string>buttons;
	buttons.push_back("Hit");
	buttons.push_back("Stand");
	buttons.push_back("Surrender");

	win.set_title("Casino Royale");
	return app->run(win);
    return 0;
	
}
