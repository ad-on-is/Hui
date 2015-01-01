#include "buttons.h"


CGEventRef ButtonsEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp))
        return event;

    keyboard = CGEventGetIntegerValueField(event, kCGKeyboardEventKeyboardType);

    if (keyboard != 40) {
        return event;
    }

    keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    printf("%d\n", keycode);


    // Swap 'a' (keycode=0) and 'z' (keycode=6).
//    if (keycode == (CGKeyCode)0)
//        keycode = (CGKeyCode)6;
//    else if (keycode == (CGKeyCode)6)
//        keycode = (CGKeyCode)0;


    if (keycode == TABLETBUTTON1 || keycode == TABLETBUTTON2 || keycode == TABLETBUTTON3 || keycode == TABLETBUTTON4 || keycode == TABLETBUTTON5 || keycode == TABLETBUTTON6 || keycode == TABLETBUTTON7 || keycode == TABLETBUTTON8) {
        return false;
    }
    // Set the modified keycode field in the event.
//    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t) keycode);

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
            eventMask, ButtonsEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }

    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);

    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    // Enable the event tap.
    CGEventTapEnable(eventTap, true);

    // Set it all running.
    CFRunLoopRun();
}