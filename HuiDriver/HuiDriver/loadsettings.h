// loadsettings.h

#ifndef LOADSETTINGS_H
#define LOADSETTINGS_H


#include "libconfig/lib/libconfig.h"

struct command {
    int input;
    int mode;
    int cmd;
    int ctrl;
    int alt;
    int shift;
    int key;
    int pressed;
};


struct command penButtons[2];
struct command tabletButtons[8];
struct command hotCells[16];

int leftMode;

void loadSettings();


#endif