#ifndef __DRONE_CORE_H__
#define __DRONE_CORE_H__

#include <control.h>
#include <navdata.h>

#define DRONE_COMMAND_PORT 5556
#define DRONE_NAVDATA_PORT 5554
#define DRONE_IP_ADDRESS "192.168.1.1"

#define MAX_COMMAND        1024
#define MAX_NAVDATA        1024

typedef struct _drone_t {
	int sd_navdata;
	struct sockaddr_in drone_navdata_addr;
	struct sockaddr_in client_navdata_addr; // not necessary
	
	int sd_command;
	struct sockaddr_in drone_command_addr;
	struct sockaddr_in client_command_addr; // not necessary

	int navdata_arr[MAX_NAVDATA];
	char command[MAX_COMMAND];

	int flags;
	int sequence;
	int socketsize;
	navdata_t navdata;
} drone_t;

drone_t* initialize_drone(drone_t* drone);

void close_drone(drone_t* drone);

//navdata

navdata_t get_navdata(drone_t* drone);

void update_navdata(drone_t* drone);

//control

void takeoff_drone(drone_t* drone);

void land_drone(drone_t* drone);

void hovering_drone(drone_t* drone);

void spin_drone(drone_t* drone, float _yawSpeed);

void lift_drone(drone_t* drone, float _vSpeed);

void translate_drone(drone_t* drone, float _pitch, float _roll);


#endif
