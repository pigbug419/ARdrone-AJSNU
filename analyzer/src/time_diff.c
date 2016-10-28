#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

//set start time stamp
void init_timeval(struct timeval *t0){
	gettimeofday(t0, 0);
}

// return the time diff between the function call and start time
float diff_timeval(struct timeval t0){
	struct timeval t1;
	gettimeofday(&t1, 0);
	return (t1.tv_sec-t0.tv_sec)*1000.0f + (t1.tv_usec-t0.tv_usec)/1000.0f;
}
