//
// Created by Ryan Shores on 12/8/24.
//

#include <IRRemote.hpp>

#ifndef IRREMOTE_H
#define IRREMOTE_H



class IRRemote {
    private:
        int pin;


    public:
        IRRemote(int pin);
        void setup();
        bool decode();
        uint16_t get_command();
};



#endif //IRREMOTE_H
