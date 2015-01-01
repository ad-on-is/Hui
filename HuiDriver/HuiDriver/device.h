#ifndef DEVICE_H
#define DEVICE_H

#define SLEEPTIME 1200
#include "hidapi.h"
#include <unistd.h>
#include <string.h>

int res;
unsigned char buf[256];
hid_device *handle;
struct hid_device_info;


int tabletDeviceOpen;
int tabletDataReceived;
int tabletInit;


void readDeviceData();
int initDevice();

#endif