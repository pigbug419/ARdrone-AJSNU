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

#ifdef __cplusplus 
extern "C" {
#endif

#include <navdata.h>
#include <com.h>
#include <control.h>
#include <core.h>
#include <types.h>
	//#include "control.c"

#ifdef __cplusplus 
}
#endif

void print_cmd(enum DRONE_COMMAND cmd)
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
}
//int cmd;
float speed;
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

	for(;;i++)
	{
		if(i>100000) i=0;
		
		DRONE_IN msg;
		navdata_t navdata;
		navdata.navdata_option.altitude = i;
	
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

		int msg_size = sizeof(msg);
		memcpy(shmsg, &msg, msg_size);	
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
	//	printf("it is control_receive\n");
		DRONE_OUT msg;
		ssize_t nbytes;
		int msg_size = sizeof(msg)-sizeof(msg.msgtype);
	//	printf("2\n");
		nbytes = msgrcv(drone_key, &msg, msg_size, 1, 0);
	//	printf("after receive\n");
		if(nbytes<0)
		{
			printf("Fail to receive Control data!!!@@@\n");
			return (NULL);
		}
		enum DRONE_COMMAND cmd;
		memcpy(&cmd, &msg.cmd, msg_size);
		print_cmd(cmd);

	/*	switch(cmd){
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
				spin_drone(drone, speed);
				break;
			case SPINL:
				spin_drone(drone, -speed);
				break;
			case MOVEF:
				translate_drone(drone, speed, 0.0f);
				break;
			case MOVER:
				translate_drone(drone, 0.0f, speed);
				break;
			case MOVEL:
				translate_drone(drone, 0.0f, -speed);
				break;
			case MOVEB:
				translate_drone(drone, -speed, 0.0f);
			case MOVEU:
				lift_drone(drone,speed);
				break;				
		}*/
	}
}

float time_diff(struct timeval t0, struct timeval t1)
{
	return (t1.tv_sec - t0.tv_sec)*1000.0f + (t1.tv_usec- t0.tv_usec)/1000.0f;
}

int main(int argc, char *argv[]) {

	drone_key = DRONE_KEY;
	drone_key = msgget(drone_key, IPC_CREAT | 0666);
	if(drone_key ==-1)
	{
		printf("Fail to create a message que - drone_key\n");
		return 0;
	}
	// declare variables
//	int	i, cmd;
	float speed;
	printf("speed : ");
	scanf("%f", &speed);
	//	navdata_t navdata;
	//	drone_t *drone = NULL;
//	drone = initialize_drone(drone);

	pthread_t thread_nav, thread_control;
	pthread_create(&thread_nav, NULL, nav_send_sm, NULL);
	pthread_create(&thread_control, NULL, control_receive, NULL);

	pthread_join(thread_nav, NULL);
	pthread_join(thread_control, NULL);


/*	do{
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
		printf("cmd : ");
		scanf("%d", &cmd);

		//	printf("cmd is %d ##########################################\n",cmd);

		//		struct timeval t0;
		//		struct timeval t1;
		//		float diff = 0;
		//		gettimeofday(&t0, 0);
		//	while(diff<2000){
		switch(cmd){

			case 0: //Landing
				land_drone(drone);
				break;
			case 1:// Take Off
				takeoff_drone(drone);
				break;
			case 2:// Spin Right
				spin_drone(drone, speed);
				break;
			case 3:// Spin Left
				spin_drone(drone, -speed);
				break;
			case 4:
				// Move front
				translate_drone(drone, speed, 0.0f);
				break;
			case 5:// Move back
				translate_drone(drone, -speed, 0.0f);
				break;
			case 6:// Move Right
				translate_drone(drone, 0.0f, speed);
				break;
			case 7:// Move Left
				translate_drone(drone, 0.0f, -speed);
				break;
			case 8://Stop!
				land_drone(drone);
				break;
			case 9:
				hovering_drone(drone);
				break;
			case 10:
				lift_drone(drone,speed);
				break;
			default : system("exit");
		}
		//for(i=0; i<30; i++){
		//			update_navdata(drone);
		//			navdata = get_navdata(drone);
		//			print_navdata(navdata);
		//	printf("i : %d\n", i);
		//}
		//		gettimeofday(&t1, 0);
		//		diff = time_diff(t0, t1);
		//		printf("time dif = %f ms\n", diff);
		//		}
	}while(cmd!=8);
*/
	// close socket
	pthread_exit(NULL);
	close_drone(drone);
	msgctl(drone_key , IPC_RMID, 0);
//	msgctl(drone_key2, IPC_RMID, 0);
	return 0;

}

