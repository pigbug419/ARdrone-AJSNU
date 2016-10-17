// g++ -o testDrone_navdata testDrone_navdata.c 
// gcc -o testDrone_navdata testDrone_navdata.c 

#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus 
extern "C" {
#endif

#include "navdata.h"
#include "com.h"
#include "control.h"
//#include "control.c"

#ifdef __cplusplus 
}
#endif

#define DRONE_COMMAND_PORT 5556
#define DRONE_NAVDATA_PORT 5554
#define DRONE_IP_ADDRESS "192.168.1.1"

#define MAX_COMMAND        1024
#define MAX_NAVDATA        1024



int main(int argc, char *argv[]) {
  
  // declare variables
  int                 i, seq;
  int                 navdata_size;
  int                 sd1, sd2, flags, cmd;
  int                 navdata[MAX_NAVDATA];  
  char                command[MAX_COMMAND];
  float               arg1, arg2, arg3, arg4;
  struct hostent      *h;
  struct sockaddr_in  droneAddr_navdata;
  struct sockaddr_in  droneAddr_command;
  struct sockaddr_in  clientAddr_navdata;
  struct sockaddr_in  clientAddr_command;

  navdata_t          navdata_struct;
  socklen_t          socketsize;

  // initialize variables
  seq   = 1;
  flags = 0;
  socketsize = sizeof(droneAddr_navdata);
  
  //socket initialization
  // get server IP address (no check if input is IP address or DNS name)
  h = gethostbyname(DRONE_IP_ADDRESS);
  if(h==NULL) {
    printf("\tunknown host '%s' \n", DRONE_IP_ADDRESS);
    exit(1);
  }

  // create socket for each communication port
  createSocket(&sd1, &droneAddr_navdata, &clientAddr_navdata, h, DRONE_NAVDATA_PORT);
  createSocket(&sd2, &droneAddr_command, &clientAddr_command, h, DRONE_COMMAND_PORT);
  
  
  // start communication
  // tap drone's port: drone starts to send navdata in bootstrap mode
  printf("\tnavigation data start\n");
  sprintf(command, "\x01\x00");
  sendCommand(sd1, command, flags, droneAddr_navdata);
  
  // stop bootstrap mode
  printf("\tstop bootstrap mode\n"); 
  sprintf(command, "AT*CONFIG=%d,\"general:navdata_demo\",\"TRUE\"\r",seq++);
  sendCommand(sd2, command, flags, droneAddr_command);
    
  // send ack to start navdata
  printf("\tsend ack\n");
  sprintf(command, "AT*CTRL=%d,0\r",seq++);
  sendCommand(sd2, command, flags, droneAddr_command);
  
  // send command to trim sensors
  printf("\ttrim sensors\n"); 
  sprintf(command, "AT*FTRIM=%d,\r",seq++);
  sendCommand(sd2, command, flags, droneAddr_command);
  

 do{
	printf("=================Command===============\n");
	printf("\t 0 ----> Land: \n");
	printf("\t 1 ----> Take Off: \n");
	printf("\t 2 ----> Turn right: \n");
	printf("\t 3 ----> Turn left: \n");
	printf("\t 4 ----> Move front: \n");
	printf("\t 5 ----> Move back: \n");
	printf("\t 6 ----> Move right: \n");
	printf("\t 7 ----> Move left: \n");
	printf("\t 8 ----> Stop Everything: \n");

//	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");	
	scanf("%d", &cmd);
//	printf("cmd is %d ##########################################\n",cmd);
//	cmd = getchar();
//	cmd = cmd-'0';
	
  for(i=0; i<30; i++){
	
	switch(cmd){
	
	case 0: //Landing
//		printf("***********************************this is 0\n");
//		drone_land(sd2, flags, seq++, droneAddr_command);
		sprintf(command, "AT*REF=%d,290717696\r", seq);
		sendCommand(sd2, command, flags, droneAddr_command);
		break;
	case 1:// Take Off
//		drone_take_off(sd2, flags, seq++, &droneAddr_command);
		sprintf(command, "AT*REF=%d,290718464\r", seq);
		sendCommand(sd2, command, flags, droneAddr_command);
		break;
	case 2:// Spin Right
//		drone_spin(sd2, flags, seq++, &droneAddr_command, 0.1f);
		break;
	case 3:// Spin Left
//		drone_spin(sd2, flags, seq++, &droneAddr_command, -0.1f);
		break;
	case 4:// Move front
//		drone_translate(sd2, flags, seq++, &droneAddr_command, 0.1f, 0.0f);
		break;
	case 5:// Move back
//		drone_translate(sd2, flags, seq++, &droneAddr_command, -0.1f, 0.0f);
		break;
	case 6:// Move Right
//		drone_translate(sd2, flags, seq++, &droneAddr_command, 0.0f, 0.1f);
		break;
	case 7:// Move Left
//		drone_translate(sd2, flags, seq++, &droneAddr_command, 0.0f, -0.1f);
		break;
	case 8://Stop!
//		drone_land(sd2, flags, seq++, &droneAddr_command);
		break;
	default : system("exit");
	}
  // receive navdata.
//  for (i=0;i<350;i++) {
    
    /*
    // sample command sequences for take-off, turn, and landing
    if (0 <= i && i < 100) {  // take off
        printf("send AT*REF:take off\n");
        sprintf(command, "AT*REF=%d,290718208\r",seq++);
        sendCommand(sd2, command, flags, droneAddr_command);
    } else if (100 <= i && i < 300){ // rotate
        printf("send AT*PCMD\n");
        arg1 = 0; arg2 = 0; arg3 = 0; arg4 = 0.1;
        sprintf(command, "AT*PCMD=%d,1,%d,%d,%d,%d\r",seq++, 
                *(int32_t*)(&arg1), *(int32_t*)(&arg2), 
                *(int32_t*)(&arg3), *(int32_t*)(&arg4));
        sendCommand(sd2, command, flags, droneAddr_command);
    } else { // land
        printf("send AT*REF:land\n");
        sprintf(command, "AT*REF=%d,290717696\r",seq++);
        sendCommand(sd2, command, flags, droneAddr_command);
    }
    */
    
    // send watchdog if no command is sent to command port, so as to prevent drone from entering hover mode
//	system("clear");
    printf("send AT*COMWDG\n");
    sprintf(command, "AT*COMWDG=%d\r",seq++);
    sendCommand(sd2, command, flags, droneAddr_command);
        
    // tickle drone's port: drone send one packet of navdata in navdata_demo mode 
    sendCommand(sd1, "\x01\x00", flags, droneAddr_navdata);
    
    //receive data 
    memset( navdata, '\0', sizeof(navdata)); 
    navdata_size = recvfrom(sd1, navdata, sizeof(navdata), 0, (struct sockaddr *)&droneAddr_navdata, &socketsize);
    printf("received navdata %d bytes\n",navdata_size);
    
    // printf("decode navdata_struct %d bytes\n",sizeof(navdata_struct));
    memcpy(&navdata_struct, navdata, sizeof(navdata_struct));
    print_navdata(navdata_struct);
		printf("i : %d\n", i);
    
    // reset emergency if it's in emergency mode
    if ((navdata_struct.navdata_header.state & (1 << 31))!=0) {
      sprintf(command, "AT*REF=%d,290717952\r",seq++);
      sendCommand(sd2, command, flags, droneAddr_command);
    }
  }
}while(cmd!=8);

  // close socket
  if (close(sd1)<0) {
    printf("\tcannnot close socket for navdata port");
    exit(1);
  }
  if (close(sd2)<0) {
    printf("\tcannnot close socket for command port");
    exit(1);
  }

  return 0;

}

