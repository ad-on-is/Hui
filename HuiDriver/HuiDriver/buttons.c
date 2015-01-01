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
    if(tabletButtons[cb].pressed == 0) {
        tabletButtons[cb].pressed = 1;

        if(tabletButtons[cb].key >= 0) {
            // press key
            src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
            keyButton = CGEventCreateKeyboardEvent(src, (CGKeyCode)tabletButtons[cb].key, true);

            printf("%d", (CGKeyCode)tabletButtons[cb].key);
        }

        if(tabletButtons[cb].cmd == 1) {
            cmdButton = CGEventCreateKeyboardEvent(src, 0x37, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskCommand);
            CGEventPost(kCGHIDEventTap, cmdButton);
        }
        if(tabletButtons[cb].alt == 1) {
            altButton = CGEventCreateKeyboardEvent(src, 0x3A, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskAlternate);
            CGEventPost(kCGHIDEventTap, altButton);
        }
        if(tabletButtons[cb].shift == 1) {
            shiftButton = CGEventCreateKeyboardEvent(src, 0x38, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskShift);
            CGEventPost(kCGHIDEventTap, shiftButton);
        }
        if(tabletButtons[cb].ctrl == 1) {
            ctrlButton = CGEventCreateKeyboardEvent(src, 0x3B, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskControl);
            CGEventPost(kCGHIDEventTap, ctrlButton);
        }


        CGEventPost(kCGHIDEventTap, keyButton);
        printf("pressed: %d\n", kc);
    }
}

void handleKeyUp(CGKeyCode kc) {
    if(tabletButtons[cb].pressed == 1) {
        tabletButtons[cb].pressed = 0;
        if(tabletButtons[cb].key >= 0) {
            // press key
            keyButton = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)tabletButtons[cb].key, false);
        }
        if(tabletButtons[cb].cmd == 1) {
            cmdButton = CGEventCreateKeyboardEvent(src, 0x37, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskCommand);
            CGEventPost(kCGHIDEventTap, cmdButton);
        }
        if(tabletButtons[cb].alt == 1) {
            altButton = CGEventCreateKeyboardEvent(src, 0x3A, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskAlternate);
            CGEventPost(kCGHIDEventTap, altButton);
        }
        if(tabletButtons[cb].shift == 1) {
            shiftButton = CGEventCreateKeyboardEvent(src, 0x38, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskShift);
            CGEventPost(kCGHIDEventTap, shiftButton);
        }
        if(tabletButtons[cb].ctrl == 1) {
            ctrlButton = CGEventCreateKeyboardEvent(src, 0x3B, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskControl);
            CGEventPost(kCGHIDEventTap, ctrlButton);
        }
        CGEventPost(kCGHIDEventTap, keyButton);
        printf("released\n");
    }
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