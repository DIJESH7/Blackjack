#pragma once

#include "controller.h"
#include "BJD.h"

class Controller
{
    private:
        BJD _dealer;

    public:
        Controller(BJD dealer) : _dealer(dealer) {}
        virtual ~Controller();
        void hit();
        void stand();
        void doubleDown();
        void split();
};
