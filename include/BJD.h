
#pragma once
#include "Hand.h"
#include "BJP.h"

#define cardsTotalLimit 21

using namespace std;

class BJD
{

public:
	BJD();
	bool check_win(BJP);
	void deal_cards(BJP);
	Hand current_hand_dealer();
	void reveal();


private:
	map<int, vector<BJP>> players;
	Hand& hand;

};
