#include "mouse.h"
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

double calcPenValue(int a, int b, int leftMode) {
    if(leftMode == 1) {
//        printf("%f\n", fabs(2047.00-(a + (256*b))));
        return fabs(2047.00-(a + (256*b)));
    }
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
    return (pRatio * pPen) - 0.0003;
}





