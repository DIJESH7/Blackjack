#pragma once
#include <iostream>
#include "controller.h"
#include "BJD.h"


class Controller
{
	public:
        Controller();
		void execute_cmd(int cmd, BJP player)
		{
			switch(cmd)
			{
				//hit
				//stand
				//split
				//double down
				//surrender/quit
				//insurance (if dealer has faceup Ace)
				case 1:
					p.deal_cards(player);
				// case 2:
				// 	// p.stand(player);
				// case 3: // double down
					// player.set_bet();// asking hand to double bet 
			}
		}
        void hit();
        void stand();
        void doubleDown();
        void split();
        ~Controller();


	private:

		BJD p;
};