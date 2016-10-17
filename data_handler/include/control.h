#ifndef __DRONE_CONTROL_H__
#define __DRONE_CONTROL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include <com.h>

#define MAX_COMMAND		1024

void drone_take_off(int socket, int flags, int seq, struct sockaddr_in droneAddr_command);

void drone_land(int socket, int flags, int seq, struct sockaddr_in droneAddr_command);

void drone_hovering(int socket, int flags, int seq, struct sockaddr_in droneAddr_command);

void drone_spin(int socket, int flags, int seq, struct sockaddr_in droneAddr_command, float _yawSpeed);

void drone_lift(int socket, int flags, int seq, struct sockaddr_in droneAddr_command, float _vSpeed);

void drone_translate(int socket, int flags, int seq, struct sockaddr_in droneAddr_command, float _pitch, float _roll);
//__DRONE_CONTROL_H__
#endif
