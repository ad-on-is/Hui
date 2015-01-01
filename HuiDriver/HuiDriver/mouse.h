#ifndef MOUSE_H
#define MOUSE_H


#define SENSITIVITY_LEVELS 2048
#define CLEANAREA_PADDING_TOP 100
#define CLEANAREA_PADDING_BOTTOM 48
#define PENBUTTON_NONE 192
#define PENBUTTON_TIP 193
#define PENBUTTON_BUTTON1 194
#define PENBUTTON_TIPBUTTON1 195
#define PENBUTTON_BUTTON2 196
#define PENBUTTON_TIPBUTTON2 197

#include <ApplicationServices/ApplicationServices.h>

int buttonLeftDown;
int buttonRightDown;

CGEventRef moveMouse;
CGEventRef leftMouse;
CGEventRef rightMouse;

// calculated output values for mouse-events
double xMouse; // x-position for mouse
double yMouse; // y-position for mouse
double pMouse; // pressure for mouse

// calculated values from device
int xPen; // x-position of pen
int yPen; // y-position of pen
int pPen; // pressure of pen

double xRatio;
double pRatio;

double screenWidth;
double screenHeight;

// used for acitve drawing area
double tabletSpan;
double screenSpan;
double valueScaled;


double calcPenValue(int a, int b, int leftMode);
double calculateYMouse(int xPen);
double calculateXMouse(int xPen);
double calculatePMouse(int pPen);
void handleMouse(double x, double y, double pressure, int penButton);


#endif