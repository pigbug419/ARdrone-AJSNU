#include <stdio.h>
#include "types.h"


void print_cmd(DRONE_COMMAND cmd)
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
