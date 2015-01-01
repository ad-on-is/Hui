#ifndef BUTTONS_H
#define BUTTONS_H

#define TABLETBUTTON1 118
#define TABLETBUTTON2 1
#define TABLETBUTTON3 78
#define TABLETBUTTON4 69
#define TABLETBUTTON5 5
#define TABLETBUTTON6 9
#define TABLETBUTTON7 7
#define TABLETBUTTON8 8

#include <ApplicationServices/ApplicationServices.h>

CFMachPortRef      eventTap;
CGEventMask        eventMask;
CFRunLoopSourceRef runLoopSource;

int keyboard;
CGKeyCode keycode;

CGEventRef keyboardKey;

void handleButtons();

#endif