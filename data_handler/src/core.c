#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include <core.h>
#define CPDBG(fmt, ...)	printf(fmt, ##__VA_ARGS__)

void initialize_drone(drone_t* drone)
{
	struct hostent *h;
	if(drone)
		close_drone(drone);
	drone = malloc(sizeof(drone_t));
	drone->flags = 0;
	drone->sequence = 1;
	drone->socketsize = sizeof(drone->drone_navdata_addr);
	
	h = gethostbyname(DRONE_IP_ADDRESS);
	if(h==NULL){
		printf("\tunknown host '%s' \n", DRONE_IP_ADDRESS);
		exit(1);
	}

	createSocket(&drone->sd_navdata, &drone->drone_navdata_addr, &drone->client_navdata_addr, h, DRONE_NAVDATA_PORT);
	createSocket(&drone->sd_command, &drone->drone_command_addr, &drone->client_command_addr, h, DRONE_COMMAND_PORT);
	
	// start communication
	CPDBG("\tnavigation data start\n");
	sprintf(drone->command, "\x01\x00");
	sendCommand(drone->sd_navdata, drone->command, drone->flags, drone->drone_navdata_addr);
  
	// stop bootstrap mode
  CPDBG("\tstop bootstrap mode\n"); 
  sprintf(drone->command, "AT*CONFIG=%d,\"general:navdata_demo\",\"TRUE\"\r",drone->sequence++);
  sendCommand(drone->sd_command, drone->command, drone->flags, drone->drone_command_addr);
    
  // send ack to start navdata
  CPDBG("\tsend ack\n");
  sprintf(drone->command, "AT*CTRL=%d,0\r",drone->sequence++);
  sendCommand(drone->sd_command, drone->command, drone->flags, drone->drone_command_addr);
  
  // send command to trim sensors
  CPDBG("\ttrim sensors\n"); 
  sprintf(drone->command, "AT*FTRIM=%d,\r",drone->sequence++);
  sendCommand(drone->sd_command, drone->command, drone->flags, drone->drone_command_addr);
  
	CPDBG("Initializing drone finished\n");
}

void close_drone(drone_t* drone)
{
	if(!drone) return;
	if(close(drone->sd_navdata)<0)
	{
		printf("cannot close socket for navdata port\n");
	}
	if(close(drone->sd_command)<0)
	{
		printf("cannot close socket for command port\n");
	}
}
//navdata

navdata_t get_navdata(drone_t* drone)
{
	if(!drone){
		printf("drone is not initialized yet!\n");
		exit(-1);
	}
	return drone->navdata;
}

void update_navdata(drone_t* drone)
{
	int navdata_size;
	if(!drone){
		printf("drone is not initialized yet!\n");
		return;
	}
	CPDBG("send AT*COMWDG\n");
	sprintf(drone->command, "AT*COMWDG=%d\r",drone->sequence++);
	sendCommand(drone->sd_command, drone->command, drone->flags, drone->drone_command_addr);
			
	// tickle drone's port: drone send one packet of navdata in navdata_demo mode 
	sendCommand(drone->sd_navdata, "\x01\x00", drone->flags, drone->drone_navdata_addr);
	
	//receive data 
	memset(drone->navdata_arr, '\0', sizeof(drone->navdata_arr)); 
	navdata_size = recvfrom(drone->sd_navdata, drone->navdata_arr, sizeof(drone->navdata_arr), 0, (struct sockaddr *)&drone->drone_navdata_addr, &drone->socketsize);
	CPDBG("received navdata %d bytes\n",navdata_size);
	
	// printf("decode navdata_struct %d bytes\n",sizeof(navdata_struct));
	memcpy(&drone->navdata, drone->navdata_arr, sizeof(drone->navdata));

  // reset emergency if it's in emergency mode
  if ((drone->navdata.navdata_header.state & (1 << 31))!=0) {
    sprintf(drone->command, "AT*REF=%d,290717952\r",drone->sequence++);
    sendCommand(drone->sd_command, drone->command, drone->flags, drone->drone_command_addr);
  }
}

//control

void takeoff_drone(drone_t* drone)
{
	if(!drone){
		printf("drone is not initialized yet!\n");
		return;
	}
	drone_take_off(drone->sd_command, drone->flags, drone->sequence++, drone->drone_command_addr);
}

void land_drone(drone_t* drone)
{
	if(!drone){
		printf("drone is not initialized yet!\n");
		return;
	}
	drone_land(drone->sd_command, drone->flags, drone->sequence++, drone->drone_command_addr);
}

void hovering_drone(drone_t* drone)
{
	if(!drone){
		printf("drone is not initialized yet!\n");
		return;
	}
	drone_hovering(drone->sd_command, drone->flags, drone->sequence++, drone->drone_command_addr);
}

void spin_drone(drone_t* drone, float _yawSpeed)
{
	if(!drone){
		printf("drone is not initialized yet!\n");
		return;
	}
	drone_spin(drone->sd_command, drone->flags, drone->sequence++, drone->drone_command_addr, _yawSpeed);
}

void lift_drone(drone_t* drone, float _vSpeed)
{
	if(!drone){
		printf("drone is not initialized yet!\n");
		return;
	}
	drone_lift(drone->sd_command, drone->flags, drone->sequence++, drone->drone_command_addr, _vSpeed);
}

void translate_drone(drone_t* drone, float _pitch, float _roll)
{
	if(!drone){
		printf("drone is not initialized yet!\n");
		return;
	}
	drone_translate(drone->sd_command, drone->flags, drone->sequence++, drone->drone_command_addr, _pitch, _roll);
}


