
#pragma once
//#include <csdtio>
//#include <stdlib>
#include <string>
#include <vector>
#include "Hand.h"

using namespace std;

class BJP
{

    public:
        BJP();
        BJP(string n, int id, int c) : name(n), player_id(id), chips(c) {};
        string get_player_name();
        void set_player_name(string);
        int get_chips();
        void set_chips(int);
        Hand currentHand();
        Hand getHand();


    private:
        string name;
        int chips, player_id;
        vector<Hand> hands;
        Hand hand;

};
