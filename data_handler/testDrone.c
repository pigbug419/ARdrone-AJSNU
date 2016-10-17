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

#include <navdata.h>
#include <com.h>
#include <control.h>
#include <core.h>
//#include "control.c"

#ifdef __cplusplus 
}
#endif

int main(int argc, char *argv[]) {
  
  // declare variables
  int	i, cmd;
  navdata_t navdata;
	drone_t *drone = NULL;
	drone = initialize_drone(drone);
  
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
		
/*		
		switch(cmd){
		
		case 0: //Landing
			land_drone(drone);
			break;
		case 1:// Take Off
			takeoff_drone(drone);
			break;
		case 2:// Spin Right
			spin_drone(drone, 0.1f);
			break;
		case 3:// Spin Left
			spin_drone(drone, -0.1f);
			break;
		case 4:
			// Move front
			translate_drone(drone, 0.1f, 0.0f);
			break;
		case 5:// Move back
			translate_drone(drone, -0.1f, 0.0f);
			break;
		case 6:// Move Right
			translate_drone(drone, 0.0f, 0.1f);
			break;
		case 7:// Move Left
			translate_drone(drone, 0.0f, -0.1f);
			break;
		case 8://Stop!
			land_drone(drone);
			break;
		default : system("exit");
		}*/
		for(i=0; i<30; i++){
			update_navdata(drone);
			navdata = get_navdata(drone);
			print_navdata(navdata);
			printf("i : %d\n", i);
		}
	}while(cmd!=8);

  // close socket
	close_drone(drone);

  return 0;

}

