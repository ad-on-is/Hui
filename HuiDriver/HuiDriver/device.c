#include "device.h"


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

    printf("DEVICE READY!\n");
    tabletInit = 1;
    return 1;
}

void readDeviceData() {

    int i;

    res = hid_read(handle, buf, sizeof(buf));
    usleep(SLEEPTIME);
    // Print out the returned buffer.
//    printf("%d\n", res);
    if(res > 0) {
        for (i = 0; i < res; i++) {
//
            xPen = calcPenValue((int) buf[2], (int) buf[3], leftMode);
            yPen = calcPenValue((int) buf[4], (int) buf[5], leftMode);
            pPen = calcPenValue((int) buf[6], (int) buf[7], 0);
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