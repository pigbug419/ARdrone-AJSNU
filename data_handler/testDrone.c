// g++ -o testDrone_navdata testDrone_navdata.c 
// gcc -o testDrone_navdata testDrone_navdata.c 

#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h> // for sleep

#include <navdata.h>
#include <com.h>
#include <control.h>
#include <core.h>
#include <types.h>
#include <time_diff.h>
	//#include "control.c"


/*void print_cmd(enum DRONE_COMMAND cmd)
  {
  switch(cmd)
  {
  case TAKEOFF:
  printf("Take Off\n");
  break;
  case LAND:
  printf("Land\n");
  break;
  case HOVERING:
  printf("Hovering\n");
  break;
  case SPINR:
  printf("Spin right\n");
  break;
  case SPINL:
  printf("Spin left\n");
  break;
  case MOVEF:
  printf("Move forward\n");
  break;
  case MOVER:
  printf("Move to rightside\n");
  break;
  case MOVEL:
  printf("Move to leftside\n");
  break;
  case MOVEB:
  printf("Move backward\n");
  break;
  case MOVEU:
  printf("Go higher (lift)\n");
  break;
  case MOVED:
  printf("Go lower\n");
  break;
  default:
  printf("Unknown command %d\n", cmd);
  }
  }*/

//int cmd;
//float speed;
key_t drone_key;
drone_t *drone;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *nav_send_sm(){
	printf("it is nav_send_sm\n");
	int shmid = shmget((key_t)5678, sizeof(DRONE_IN), 0777 | IPC_CREAT);
	if(shmid == -1){
		printf("error in initialize  shared memory!!@@@@\n");
		return (NULL);
	}

	int i=0;

	DRONE_IN *shmsg;
	if((shmsg = (DRONE_IN*)shmat(shmid, NULL,0))==(DRONE_IN*)-1){
		printf("shmat failed!\n");
		return (NULL);
	}

	for(;;)
	{
		

		DRONE_IN msg;
		navdata_t navdata;
		update_navdata(drone);
		navdata = get_navdata(drone);
		//		navdata.navdata_option.altitude = i;
		//print_navdata(navdata);
		//make msg
		msg.msgtype = DRONE_KEY;
		msg.nav.vx = navdata.navdata_option.vx;
		msg.nav.vy = navdata.navdata_option.vy;
		msg.nav.vz = navdata.navdata_option.vz;
		msg.nav.theta = navdata.navdata_option.theta;
		msg.nav.psi = navdata.navdata_option.psi;
		msg.nav.phi = navdata.navdata_option.phi;
		msg.nav.altitude = navdata.navdata_option.altitude;
		msg.nav.is_flying = ((navdata.navdata_header.state & (1<<0))!=0);
		//		msg.nav.is_flying = true;

		int msg_size = sizeof(msg);
		memcpy(shmsg, &msg, msg_size);
		usleep(20000);
	}

}

void *nav_send(){
	printf("it is nav_send\n");
	int i=0;

	//	key_t drone_key2 = msgget(DRONE_KEY, IPC_CREAT | 0666);
	//	printf("drone_key = %d\n", drone_key2);

	for(;;i++)
	{
		//	if(cmd==8) return (NULL);
		//	pthread_mutex_lock(&mutex);

		//		update_navdata(drone);

		navdata_t navdata;
		//		navdata = get_navdata(drone);
		navdata.navdata_option.altitude = i;

		DRONE_IN msg;
		int msg_size = 0;
		int rtn;
		msg.msgtype = DRONE_KEY;
		msg.nav.vx = navdata.navdata_option.vx;
		msg.nav.vy = navdata.navdata_option.vy;
		msg.nav.vz = navdata.navdata_option.vz;
		msg.nav.theta = navdata.navdata_option.theta;
		msg.nav.psi = navdata.navdata_option.psi;
		msg.nav.phi = navdata.navdata_option.phi;
		msg.nav.altitude = navdata.navdata_option.altitude;
		msg.nav.is_flying = ((navdata.navdata_header.state & (1<<0))!=0);

		msg_size = sizeof(msg) - sizeof(msg.msgtype);
		rtn = msgsnd(drone_key, &msg, msg_size, 0);
		printf("i : %d\n", i);

		if(rtn == -1)
		{
			printf("Fail to send Navdata!!!@@@\n");
			return (NULL);
		}
		//sleep(1);

		//	pthread_mutex_unlock(&mutex);
	}
}

void *control_receive(){
	for(;;)
	{
		printf("it is control_receive\n");
		DRONE_OUT msg;
		ssize_t nbytes;
		int msg_size = sizeof(msg)-sizeof(msg.msgtype);
		printf("2\n");
		nbytes = msgrcv(drone_key, &msg, msg_size, 1, 0);
		printf("after receive\n");
		if(nbytes<0)
		{
			printf("Fail to receive Control data!!!@@@\n");
			return (NULL);
		}
		enum DRONE_COMMAND cmd;
		memcpy(&cmd, &msg.cmd, msg_size);
		print_cmd(cmd);

		switch(cmd){
			case TAKEOFF:
				takeoff_drone(drone);
				break;
			case LAND:
				land_drone(drone);
				break;
			case HOVERING:
				hovering_drone(drone);
				break;
			case SPINR:	
				spin_drone(drone, 0.4f);
				break;
			case SPINL:
				spin_drone(drone, -0.4f);
				break;
			case MOVEF:
				translate_drone(drone, -0.05f, 0.0f);
				break;
			case MOVER:
				translate_drone(drone, 0.0f, 0.1f);
				break;
			case MOVEL:
				translate_drone(drone, 0.0f, -0.1f);
				break;
			case MOVEB:
				translate_drone(drone, 0.2f, 0.0f);
			case MOVEU:
				lift_drone(drone,0.4f);
				break;				
			case STOP:
				return;
				break;
		}
		//usleep(50000);
		//hovering_drone(drone);
		CPDBG("touch\n");
	}
}

int main(int argc, char *argv[]) {

	drone_key = DRONE_KEY;
	drone_key = msgget(drone_key, IPC_CREAT | 0666);
	if(drone_key ==-1)
	{
		printf("Fail to create a message que - drone_key\n");
		return 0;
	}
	navdata_t navdata;
	drone = initialize_drone(drone);

	pthread_t thread_nav, thread_control;
	pthread_create(&thread_nav, NULL, nav_send_sm, NULL);
	pthread_create(&thread_control, NULL, control_receive, NULL);

	pthread_join(thread_nav, NULL);
	pthread_join(thread_control, NULL);

	// close socket
	pthread_exit(NULL);
	close_drone(drone);
	msgctl(drone_key , IPC_RMID, 0);
	//	msgctl(drone_key2, IPC_RMID, 0);
	return 0;

}

