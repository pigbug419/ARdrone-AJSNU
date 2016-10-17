
#ifndef __DRONE_COM_H__
#define __DRONE_COM_H__
#include <unistd.h>
#include <time.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create socket for designated address and port
void createSocket(int *sd, struct sockaddr_in *droneAddr, 
                  struct sockaddr_in *clientAddr, struct hostent *h, int port);

// send command to designated address and port
void sendCommand(int socket, char* command, int flag, struct sockaddr_in droneAddr);

#endif
