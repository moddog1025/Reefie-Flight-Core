#ifndef USB_COMMS_H
#define USB_COMMS_H

#include "config.h"

void executeCommand(int CC, int SS, String data);
void monitorUSB();
float getSimAlt();

#endif