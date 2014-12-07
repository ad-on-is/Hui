/*******************************************************
HUI!
Tablet Driver for HUION tablets

Adis Durakovic

OUTPUT (example)

0  1  2  3  4  5  6  7
0a c5 74 03 95 05 b1 02

0: nothing
1: B (c1, c2, c3 = c1+c2, c4, c5 = c1+c4)
2: X (min: 00, max: ff)
3: X (min: 00, max: 07)
4: Y (min: 00, max: ff)
5: Y (min: 00, max: 07)
6: P (min: 00, max: ff)
7: P (min: 00, max: 07)

Button: [1]
X-pos = [3][2]
Y-pos = [5][4]
Pres. = [7][6]


@TODO:
- get tablet buttons to work
- reclean code

********************************************************/

#define SENSITIVITY_LEVELS 2048
#define CLEANAREA_PADDING_TOP 100
#define CLEANAREA_PADDING_BOTTOM 48
#define PENBUTTON_NONE 192
#define PENBUTTON_TIP 193
#define PENBUTTON_BUTTON1 194
#define PENBUTTON_TIPBUTTON1 195
#define PENBUTTON_BUTTON2 196
#define PENBUTTON_TIPBUTTON2 197

#define SLEEPTIME 1200

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"
#include <unistd.h>

#include <ApplicationServices/ApplicationServices.h>


int printMouse = 0;

int res;
unsigned char buf[256];
hid_device *handle;

int i;
int ready = 0;
//
double xMouse; // x-position for mouse
double yMouse; // y-position for mouse
double pMouse; // pressure for mouse

int xPen; // x-position of pen
int yPen; // y-position of pen
int pPen; // pressure of pen

double xRatio;
double pRatio;


double screenWidth;
double screenHeight;

int buttonLeftDown = 0;

CGEventRef moveMouse, dragMouse, clickLeftMouse, releaseLeftMouse, mouseEvent;


struct hid_device_info;
double tabletSpan;
double screenSpan;

double valueScaled;


/**
* move mouse without touching the tablet
*/
void doMouseMove(double x, double y) {

    CGEventSetLocation(moveMouse, CGPointMake(x, y));
    CGEventSetLocation(releaseLeftMouse, CGPointMake(x, y));

    if(buttonLeftDown == 1) {
        buttonLeftDown = 0;
        CGEventSetLocation(releaseLeftMouse, CGPointMake(x, y));
        CGEventPost(kCGHIDEventTap, releaseLeftMouse);
//        CFRelease(releaseLeftMouse);
//        CFRelease(dragMouse);
    }

    CGEventPost(kCGHIDEventTap, moveMouse);
}

void doMouseMoveDrag(double x, double y, double pressure) {

    CGEventSetLocation(dragMouse, CGPointMake(x, y));


    CGEventSetDoubleValueField(dragMouse, kCGMouseEventPressure, pressure);
    CGEventSetIntegerValueField(dragMouse, kCGMouseEventSubtype, kCGEventMouseSubtypeTabletPoint);
//    CGEventSetIntegerValueField(dragMouse, kCGTabletEventPointPressure, pressure);


    CGEventSetDoubleValueField(clickLeftMouse, kCGMouseEventPressure, pressure);
    CGEventSetIntegerValueField(clickLeftMouse, kCGMouseEventSubtype, kCGEventMouseSubtypeTabletPoint);
//    CGEventSetIntegerValueField(clickLeftMouse, kCGTabletEventPointPressure, pressure);
    if(buttonLeftDown == 0) {
//        printf("here\n");
        buttonLeftDown = 1;
        CGEventSetLocation(clickLeftMouse, CGPointMake(x, y));
        CGEventPost(kCGHIDEventTap, clickLeftMouse);
    }
//
    CGEventPost(kCGHIDEventTap, dragMouse);
}

void handleMouse(double x, double y, double pressure, int penButton) {

    if (penButton == PENBUTTON_TIP) {
        doMouseMoveDrag(x, y, pressure);
    } else if (penButton == PENBUTTON_BUTTON1) {

    } else if (penButton == PENBUTTON_TIPBUTTON1) {

    } else if (penButton == PENBUTTON_BUTTON2) {

    } else if (penButton == PENBUTTON_TIPBUTTON2) {

    } else {
        doMouseMove(x, y);
    }


}



/**
* calcPenValue (a, b)
*
* calculates the value coming from the pen for x, y and pressure
* if a exceeds 255 it's set back to 0 and b gets increased by 1 resulting in a range of 2048 values
* return min: 0, max: 2048
*
* a = 0-255
* b = 0-7
*
*/

double calcPenValue(int a, int b) {
    return a + (256*b);
}

double calculateYMouse(int xPen) {
    if(xPen < 100) {
        return -1;
    }
    if(xPen > (SENSITIVITY_LEVELS - CLEANAREA_PADDING_BOTTOM)) {
        return screenHeight-0.999999;
    }
    tabletSpan = (SENSITIVITY_LEVELS-(CLEANAREA_PADDING_TOP + CLEANAREA_PADDING_BOTTOM));
    screenSpan = screenHeight-1; // aproximate mouse height

    valueScaled = (xPen - 100) / tabletSpan;

    // handle left hand mode

    return valueScaled * screenSpan;
}

double calculateXMouse(int xPen) {
    // handle left-hand mode
    return xRatio * xPen;
}

double calculatePMouse(int pPen) {
    // handle sensitivity
    return pRatio * pPen;
}


void handleTablet() {

    res = hid_read(handle, buf, sizeof(buf));
    usleep(SLEEPTIME);
    // Print out the returned buffer.
//    printf("%d\n", res);
    if(res > 0) {
        for (i = 0; i < res; i++) {
//
            xPen = calcPenValue((int) buf[2], (int) buf[3]);
            yPen = calcPenValue((int) buf[4], (int) buf[5]);
            pPen = calcPenValue((int) buf[6], (int) buf[7]);
//
////          printf("%d %d\n", yPen);
            if(calculateYMouse(yPen) > 0) {
                xMouse = calculateXMouse(xPen);
                yMouse = calculateYMouse(yPen);
                pMouse = calculatePMouse(pPen);

                handleMouse(xMouse, yMouse, pMouse, (int) buf[1]);
            }
////            printf("mousepos: %d, %f, %f, %d, %f, %f \n",xPen, xRatio, xMouse, yPen, yRatio, yMouse);
//            printf("%02hhx ", buf[i]);
        }
////        printf("\n");
    }



//    hid_close(handle);

    /* Free static HIDAPI objects. */
//    hid_exit();
}



int main(int argc, char* argv[]) {


    screenWidth = CGDisplayPixelsWide(CGMainDisplayID());
    screenHeight = CGDisplayPixelsHigh(CGMainDisplayID());

    xRatio = screenWidth/SENSITIVITY_LEVELS;
    pRatio = 0.00048;


    if (hid_init()) {
        return -1;
    }


    // Set up the command buffer.

    memset(buf,0x00,sizeof(buf));
//    buf[0] = 0x01;
//    buf[1] = 0x81;


    // and optionally the Serial number.
    ////handle = hid_open(0x4d8, 0x3f, L"12345");
    handle = hid_open(0x256c, 0x006e, NULL);
    if (!handle) {
        printf("unable to open device\n");
        return 1;
    } else {
        printf("device open\n");
    }

    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1);


    // Read requested state. hid_read() has been set to be
    // non-blocking by the call to hid_set_nonblocking() above.
    // This loop demonstrates the non-blocking nature of hid_read().
    res = 0;



    moveMouse = CGEventCreateMouseEvent(
            NULL, kCGEventMouseMoved,
            CGPointMake(0, 0),
            kCGMouseButtonLeft // ignored
    );

    dragMouse = CGEventCreateMouseEvent(
            NULL, kCGEventLeftMouseDragged,
            CGPointMake(0, 0),
            kCGMouseButtonLeft // ignored
    );

    clickLeftMouse = CGEventCreateMouseEvent(
            NULL, kCGEventLeftMouseDown,
            CGPointMake(0,0),
            kCGMouseButtonLeft
    );

    releaseLeftMouse = CGEventCreateMouseEvent(
            NULL, kCGEventLeftMouseUp,
            CGPointMake(0, 0),
            kCGMouseButtonLeft
    );

    while(1) {
        handleTablet();
    }

    return 0;
}