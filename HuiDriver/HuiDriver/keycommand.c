#include "keycommand.h"


void keyDown(struct command *trigger) {

    if(trigger->pressed == 0) {
        trigger->pressed = 1;

        if (trigger->key >= 0) {
            // press key
            src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
            keyButton = CGEventCreateKeyboardEvent(src, (CGKeyCode) trigger->key, true);

            printf("%d", (CGKeyCode) trigger->key);
        }

        if (trigger->cmd == 1) {
            cmdButton = CGEventCreateKeyboardEvent(src, 0x37, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskCommand);
            CGEventPost(kCGHIDEventTap, cmdButton);
        }
        if (trigger->alt == 1) {
            altButton = CGEventCreateKeyboardEvent(src, 0x3A, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskAlternate);
            CGEventPost(kCGHIDEventTap, altButton);
        }
        if (trigger->shift == 1) {
            shiftButton = CGEventCreateKeyboardEvent(src, 0x38, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskShift);
            CGEventPost(kCGHIDEventTap, shiftButton);
        }
        if (trigger->ctrl == 1) {
            ctrlButton = CGEventCreateKeyboardEvent(src, 0x3B, true);
            CGEventSetFlags(keyButton, kCGEventFlagMaskControl);
            CGEventPost(kCGHIDEventTap, ctrlButton);
        }

        printf("pressed: %d\n", trigger->input);
        CGEventPost(kCGHIDEventTap, keyButton);
    }
}


void keyUp(struct command *trigger) {

    if(trigger->pressed == 1) {
        trigger->pressed = 0;
        if(trigger->key >= 0) {
            // press key
            keyButton = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)trigger->key, false);
        }
        if(trigger->cmd == 1) {
            cmdButton = CGEventCreateKeyboardEvent(src, 0x37, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskCommand);
            CGEventPost(kCGHIDEventTap, cmdButton);
        }
        if(trigger->alt == 1) {
            altButton = CGEventCreateKeyboardEvent(src, 0x3A, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskAlternate);
            CGEventPost(kCGHIDEventTap, altButton);
        }
        if(trigger->shift == 1) {
            shiftButton = CGEventCreateKeyboardEvent(src, 0x38, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskShift);
            CGEventPost(kCGHIDEventTap, shiftButton);
        }
        if(trigger->ctrl == 1) {
            ctrlButton = CGEventCreateKeyboardEvent(src, 0x3B, false);
            CGEventSetFlags(keyButton, kCGEventFlagMaskControl);
            CGEventPost(kCGHIDEventTap, ctrlButton);
        }
        CGEventPost(kCGHIDEventTap, keyButton);
        printf("released\n");
    }
}
