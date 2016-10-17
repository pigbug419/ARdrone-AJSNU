#include <stdio.h>
#include <control.h>
#define CPDBG(fmt, ...)	printf(fmt, ##__VA_ARGS__)

char command[MAX_COMMAND];

void drone_take_off(int socket, int flags, int seq, struct sockaddr_in droneAddr_command){
	CPDBG("\ttake off\n");
	sprintf(command, "AT*REF=%d,290718464\r", seq);
	sendCommand(socket, command, flags, droneAddr_command);
}



void drone_land(int socket, int flags, int seq, struct sockaddr_in droneAddr_command){
	CPDBG("\tland\n");
	sprintf(command, "AT*REF=%d,290717696\r", seq);
	sendCommand(socket, command, flags, droneAddr_command);
}


void drone_hovering(int socket, int flags, int seq, struct sockaddr_in droneAddr_command){
	CPDBG("\thovering\n");
	sprintf(command, "AT*PCMD=%d,1,0,0,0,0\r", seq);
	sendCommand(socket, command, flags, droneAddr_command);
}


void drone_spin(int socket, int flags, int seq, struct sockaddr_in droneAddr_command, float _yawSpeed){
	CPDBG("\tspin\n");
	int yawSpeed;
	memcpy(&yawSpeed, &_yawSpeed, sizeof(float));
	sprintf(command, "AT*PCMD=%d,1,0,0,0,%d\r", seq, yawSpeed);
	sendCommand(socket, command, flags, droneAddr_command);
}


void drone_lift(int socket, int flags, int seq, struct sockaddr_in droneAddr_command, float _vSpeed){
	CPDBG("\tlift\n");
	int vSpeed;
	memcpy(&vSpeed, &_vSpeed, sizeof(float));
	sprintf(command, "AT*PCMD=%d,1,0,0,%d,0\r", seq, vSpeed);
	sendCommand(socket, command, flags, droneAddr_command);
}


void drone_translate(int socket, int flags, int seq, struct sockaddr_in droneAddr_command, float _pitch, float _roll){
	CPDBG("\ttranslate\n");
	int pitch, roll;
	memcpy(&pitch, &_pitch, sizeof(float));
	memcpy(&roll, &_roll, sizeof(float));
	sprintf(command, "AT*PCMD=%d,1,%d,%d,0,0\r", seq, roll, pitch);
	sendCommand(socket, command, flags, droneAddr_command);
}
