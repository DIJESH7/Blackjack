#include <iostream>


class Controller
{
	public:
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
					p.deal_card(player);
				case 2:
					pl.stand(player);
				case 3: // double down
					player.set_bet();// asking hand to double bet 






			}
		}




	private:

		BJD p;
}