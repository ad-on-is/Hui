// hui.h

#ifndef HUI_H
#define HUI_H

#define SLEEPTIME 1200
#define SENSITIVITY_LEVELS 2048
#define CLEANAREA_PADDING_TOP 100
#define CLEANAREA_PADDING_BOTTOM 48
#define PENBUTTON_NONE 192
#define PENBUTTON_TIP 193
#define PENBUTTON_BUTTON1 194
#define PENBUTTON_TIPBUTTON1 195
#define PENBUTTON_BUTTON2 196
#define PENBUTTON_TIPBUTTON2 197

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <ApplicationServices/ApplicationServices.h>
#include "hidapi.h"
#include "loadsettings.h"
#include "buttons.h"




int res;
unsigned char buf[256];
hid_device *handle;

int i;

//
double xMouse; // x-position for mouse
double yMouse; // y-position for mouse
double pMouse = 1; // pressure for mouse

int xPen; // x-position of pen
int yPen; // y-position of pen
int pPen; // pressure of pen

double xRatio;
double pRatio;


double screenWidth;
double screenHeight;

int buttonLeftDown = 0;
int buttonRightDown = 0;

CGEventRef moveMouse;
CGEventRef leftMouse;
CGEventRef rightMouse;



struct hid_device_info;
double tabletSpan;
double screenSpan;

double valueScaled;



int tabletDeviceOpen = 0;
int tabletDataReceived = 0;
int tabletInit = 0;

int tbCount = 8; // get amount of tabletButtons
int pbCount = 2; // get amount of tabletButtons


int initDevice();
void doMouseMove(double x, double y);
void handleMouse(double x, double y, double pressure, int penButton);


#endif