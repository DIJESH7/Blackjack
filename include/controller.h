#pragma once

#include "controller.h"
#include "BJD.h"

class Controller
{
    private:
        Dealer _dealer;

    public:
        Controller();
        virtual ~Controller();
        void hit();
        void stand();
        void doubleDown();
        void split();
}
