#ifndef KEYCOMMAND_H
#define KEYCOMMAND_H

#include "settings.h"
#include <ApplicationServices/ApplicationServices.h>

CGEventRef keyButton, cmdButton, altButton, shiftButton, ctrlButton;
CGEventSourceRef src;

void keyDown(struct command *trigger);
void keyUp(struct command *trigger);

#endif