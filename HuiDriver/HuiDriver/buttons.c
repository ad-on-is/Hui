#include "buttons.h"


int i;
int cb;

void handleKeyDown(CGKeyCode kc) {
    for(i = 0; i<8; i++) {
        if(tabletButtons[i].input == kc) {
            cb = i;
            break;
        }
    }

    keyDown(&tabletButtons[cb]);
}

void handleKeyUp(CGKeyCode kc) {
    keyUp(&tabletButtons[cb]);
}


CGEventRef ButtonsEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp))
        return event;

    keyboard = CGEventGetIntegerValueField(event, kCGKeyboardEventKeyboardType);
    keycode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (keyboard != 40) {
        return event;
    }


//    printf("%d\n", keycode);
    if(type == kCGEventKeyDown) {
        handleKeyDown(keycode);
    }

    if(type == kCGEventKeyUp) {
        handleKeyUp(keycode);
    }

    return false;
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