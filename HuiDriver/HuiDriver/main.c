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
- actions for tablet buttons (cmd, alt, keycode, ...)
- write enums
- leftmode
- config file

********************************************************/





#include "device.h"
#include "settings.h"
#include "buttons.h"
#include "mouse.h"






int tbCount = 8; // get amount of tabletButtons
int pbCount = 2; // get amount of tabletButtons


int main(int argc, char* argv[]) {

    loadSettings();

    int pid = fork();

    if(pid == 0) {
        handleButtons();
    } else if (pid > 0) {

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
            readDeviceData();
        }

    } else {
        exit(1);
    }


    return 0;
}