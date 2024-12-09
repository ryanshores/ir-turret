//
// Created by Ryan Shores on 12/8/24.
//
#ifndef REMOTE_H
#define REMOTE_H
class Remote {
    private:
        int pin;
        bool received();
    public:
        Remote(int pin);
        void setup();
        uint16_t get_command();
};
#endif //REMOTE_H