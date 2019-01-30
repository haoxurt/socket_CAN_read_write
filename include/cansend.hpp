#ifndef _CANSEND_H
#define _CANSEND_H

#include <iostream>
#include <vector>

//socket can.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

void cansend(int s, int* out_can);
void can_send_frame(int s,int i, char* buffer);
int can_init(int& );
void canread(int&);

#endif
