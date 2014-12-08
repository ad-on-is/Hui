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

Kecodes:

118
1
78

69
5

9
7
8


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

#define TABLETBUTTON1 118
#define TABLETBUTTON2 1
#define TABLETBUTTON3 78
#define TABLETBUTTON4 69
#define TABLETBUTTON5 5
#define TABLETBUTTON6 9
#define TABLETBUTTON7 7
#define TABLETBUTTON8 8

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


int initDevice();
void doMouseMove(double x, double y);
void handleMouse(double x, double y, double pressure, int penButton);




int initDevice() {



    // Set up the command buffer.

    memset(buf,0x00,sizeof(buf));

    res = 0;

    while(tabletDataReceived == 0) {
//        printf("inwhile\n");
        // and optionally the Serial number.
        ////handle = hid_open(0x4d8, 0x3f, L"12345");
        if(tabletDeviceOpen == 0) {
//            printf("init\n");
            handle = hid_open(0x256c, 0x006e, NULL);
            if(handle) {
//                printf("device open\n");
                tabletDeviceOpen = 1;
            }

        } else {
            hid_set_nonblocking(handle, 1);
            usleep(50000);
            res = hid_read(handle, buf, sizeof(buf));
            if(res > 0) {
                if(tabletDataReceived == 0) {
                    tabletDataReceived = 1;
                }
            }
            if(tabletDataReceived == 0) {
                tabletDeviceOpen = 0;
                hid_close(handle);
            }
        }

        // Set the hid_read() function to be non-blocking.
    }

    i = 0;
    while(i < 100) {
        doMouseMove(100.0, 100.0);
        i++;
    }
    printf("DEVICE READY!\n");
    printf("%d", tabletInit);
    tabletInit = 1;
    return 1;
}


/**
* move mouse without touching the tablet
*/
void doMouseMove(double x, double y) {

    CGEventSetType(moveMouse, kCGEventMouseMoved);
    CGEventSetLocation(moveMouse, CGPointMake(x, y));

    if(buttonLeftDown == 1) {
        buttonLeftDown = 0;
        CGEventSetLocation(leftMouse, CGPointMake(x, y));
        CGEventSetType(leftMouse, kCGEventLeftMouseUp);
        CGEventPost(kCGHIDEventTap, leftMouse);
    }

    if(buttonRightDown == 1) {
        buttonRightDown = 0;
        CGEventSetLocation(rightMouse, CGPointMake(x, y));
        CGEventSetType(rightMouse, kCGEventRightMouseUp);
        CGEventPost(kCGHIDEventTap, rightMouse);
    }
    CGEventPost(kCGHIDEventTap, moveMouse);
}

void doRightMouseClick(double x, double y) {
    if(buttonRightDown == 0) {
        CGEventSetLocation(rightMouse, CGPointMake(x, y));
        CGEventSetType(rightMouse, kCGEventRightMouseDown);
        CGEventPost(kCGHIDEventTap, rightMouse);
        buttonRightDown = 1;
    }
}

void doMouseMoveDrag(double x, double y, double pressure) {

    CGEventSetType(moveMouse, kCGEventLeftMouseDragged);

    CGEventSetLocation(moveMouse, CGPointMake(x, y));

    CGEventSetDoubleValueField(moveMouse, kCGMouseEventPressure, pressure);
    CGEventSetIntegerValueField(moveMouse, kCGMouseEventSubtype, kCGEventMouseSubtypeTabletPoint);

    CGEventSetDoubleValueField(leftMouse, kCGMouseEventPressure, pressure);
    CGEventSetIntegerValueField(leftMouse, kCGMouseEventSubtype, kCGEventMouseSubtypeTabletPoint);

    if(buttonLeftDown == 0) {
//        printf("here\n");
        buttonLeftDown = 1;
        CGEventSetLocation(leftMouse, CGPointMake(x, y));
        CGEventSetType(leftMouse, kCGEventLeftMouseDown);
        CGEventPost(kCGHIDEventTap, leftMouse);
    }
    CGEventPost(kCGHIDEventTap, moveMouse);
//
}

void handleMouse(double x, double y, double pressure, int penButton) {

    if (penButton == PENBUTTON_TIP) {
        doMouseMoveDrag(x, y, pressure);
    } else if (penButton == PENBUTTON_BUTTON1) {

    } else if (penButton == PENBUTTON_TIPBUTTON1) {

    } else if (penButton == PENBUTTON_BUTTON2) {
        doRightMouseClick(x, y);
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
//          printf("%d %d\n", yPen);
            if(calculateYMouse(yPen) > 0) {
                xMouse = calculateXMouse(xPen);
                yMouse = calculateYMouse(yPen);
                pMouse = calculatePMouse(pPen);

                handleMouse(xMouse, yMouse, pMouse, (int) buf[1]);
            }
//            printf("mousepos: %d, %f, %f, %d, %f, %f \n",xPen, xRatio, xMouse, yPen, yRatio, yMouse);
//            printf("%02hhx ", buf[i]);
        }
//        printf("\n");
    }

}



CFMachPortRef      eventTap;
CGEventMask        eventMask;
CFRunLoopSourceRef runLoopSource;


CGEventRef
myCGEventCallback(CGEventTapProxy proxy, CGEventType type,
        CGEventRef event, void *refcon)
{
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp))
        return event;

    // The incoming keycode.
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
            event, kCGKeyboardEventKeycode);

    // Swap 'a' (keycode=0) and 'z' (keycode=6).
//    if (keycode == (CGKeyCode)0)
//        keycode = (CGKeyCode)6;
//    else if (keycode == (CGKeyCode)6)
//        keycode = (CGKeyCode)0;

    printf("%d\n", CGEventGetFlags(event));

    if(keycode == TABLETBUTTON1 || keycode == TABLETBUTTON2 || keycode == TABLETBUTTON3 || keycode == TABLETBUTTON4 || keycode == TABLETBUTTON5 || keycode == TABLETBUTTON6 || keycode == TABLETBUTTON7 || keycode == TABLETBUTTON8) {
//        return false;
    }

    // Set the modified keycode field in the event.
    CGEventSetIntegerValueField(
            event, kCGKeyboardEventKeycode, (int64_t)keycode);

    // We must return the event for it to be useful.
    return event;
}


void handleButtons() {
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;

    // Create an event tap. We are interested in key presses.
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
            eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }

    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(
            kCFAllocatorDefault, eventTap, 0);

    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
            kCFRunLoopCommonModes);

    // Enable the event tap.
    CGEventTapEnable(eventTap, true);

    // Set it all running.
    CFRunLoopRun();
}



int main(int argc, char* argv[]) {


    int pid = fork();

    if(pid == 0) {
        handleButtons();
    } else {

        screenWidth = CGDisplayPixelsWide(CGMainDisplayID());
        screenHeight = CGDisplayPixelsHigh(CGMainDisplayID());

        xRatio = screenWidth/SENSITIVITY_LEVELS;
        pRatio = 0.00048;





        moveMouse = CGEventCreateMouseEvent(
                NULL, kCGEventMouseMoved,
                CGPointMake(0, 0),
                kCGMouseButtonLeft // ignored
        );

        leftMouse = CGEventCreateMouseEvent(
                NULL, kCGEventLeftMouseDown,
                CGPointMake(0,0),
                kCGMouseButtonLeft
        );

        rightMouse = CGEventCreateMouseEvent(
                NULL, kCGEventRightMouseDown,
                CGPointMake(0, 0),
                kCGMouseButtonRight
        );



        initDevice();

        while(1) {
            handleTablet();
        }

    }




    return 0;
}