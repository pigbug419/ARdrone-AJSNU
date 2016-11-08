#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#include "analyzer.hpp"

using namespace std;
using namespace cv;

Analyzer::Analyzer()
{
	stereo_source = MESSAGE_STREAM;
	video_source = NULL;
	state = NORMAL;
	stereo_key = STEREO_KEY;
	drone_key = DRONE_KEY;
	sidle_cmd = MOVER;
	realtest = false;
	runtime = 10;
	sequences = 0;
	init_psi = 0;
	spin_cnt = 0;
	move_cnt = 0;
}

bool Analyzer::Initialize()
{
	bool flag = true;
	
    drone_key = msgget(drone_key, IPC_CREAT | 0666);
	if(drone_key == -1) {
		CPDBG("Fail to create a message queue - drone key\n");
		flag = false;
	}
    
	if(stereo_source == MESSAGE_STREAM)
	{
		stereo_key = msgget(stereo_key, IPC_CREAT | 0666);
		if(stereo_key == -1) {
			CPDBG("Fail to create a message queue - stereo key\n");
			flag = false;
		}
        CPDBG("stereo_key is set\n");
	}
	else
	{

	}
	/*
	svo_key = msgget(svo_key, IPC_CREAT | 0666);
	if(svo_key == -1) {
		CPDBG("Fail to create a message queue - svo key\n");
		flag = false;
	}
	*/
	init_timeval(&stop_timer);
	init_timeval(&cmd_timer);
	CPDBG("Initialization finished\n");
	
	return flag;
}

void Analyzer::Prepare()
{
	//1. takeoff & confirm it
	while(1)
	{
		if(!ReceiveNavdata())
		{
			CPDBG("Fail to receive a navdata\n");
		}
		else
		{
			if(!nav_data.isflying){
				if(realtest){
					CPDBG("send takeoff.... enter key\n");
					getchar();
				}
				SendCommand(TAKEOFF);
			}
			else break;
		}
		usleep(COMMAND_INTERVAL*1000);
	}
	//2. lift while altitude satisfies condition
	while(1)
	{
		if(!ReceiveNavdata())
		{
			CPDBG("Fail to receive a navdata\n");
		}
		else
		{
			if(nav_data.altitude <800){
				if(realtest){
					CPDBG("now altitude: %d... lift.... enter key\n", nav_data.altitude);
					getchar();
				}
				SendCommand(MOVEU); // lift while altitude > 1.0m
			}
			else break;
		}
		usleep(COMMAND_INTERVAL*1000);
	}
	init_timeval(&stop_timer);
	init_timeval(&cmd_timer);
	CPDBG("Preparing finished\n");
}

void Analyzer::PrintInfo()
{
	printf("State:%d\nStereoSource:%d\nsequences:%d\n", state, stereo_source, sequences);
	printf("Navdata-altitude:%d, psi: %f\n", nav_data.altitude, nav_data.psi);
}

void Analyzer::Land()
{
	//landing & confirm it
	while(1)
	{
		if(!ReceiveNavdata())
		{
			CPDBG("Fail to receive a navdata\n");
		}
		else
		{
			if(nav_data.isflying) SendCommand(LAND);
			else break;
		}
		usleep(COMMAND_INTERVAL*1000);
	}
}

bool Analyzer::Run()
{
	// run only one step! (for signal handling.. & scalability)
	DRONE_COMMAND cmd = HOVERING;
	mode_changed = false;
	if(diff_timeval(stop_timer) > runtime * 1000)
	{
		Land();
		return false;
	}
	if(!ReceiveStereo()){
		CPDBG("Fail to get a stereo info");
		Land();
		return false;
	}
	if(!ReceiveNavdata()){
		CPDBG("Fail to get a navdata");
		Land();
		return false;
	}

	switch(state)
	{
		case NORMAL:
			cmd = NormalMode();
			break;
		case SIDLE:
			cmd = SidleMode();
			break;
		case PASSBY:
			cmd = PassbyMode();
			break;
		case LOOKASIDE:
			cmd = LookasideMode();
			break;
		case HEADSTRAIGHT:
			cmd = HeadMode();
			break;
		case RETURN:
			cmd = ReturnMode();
			break;
		default:
			CPDBG("shit error!\n");
			cmd = STOP;
			break;
	}
	if(mode_changed || 0.9*COMMAND_INTERVAL < diff_timeval(cmd_timer))
	{
		if(state == SIDLE)
		{
			if(cmd == MOVEL) move_cnt--;
			if(cmd == MOVER) move_cnt++;
		}
		else if(state == RETURN)
		{
			if(cmd == MOVEL) move_cnt--;
			if(cmd == MOVER) move_cnt++;
		}
		SendCommand(cmd);
		init_timeval(&cmd_timer);
	}
	return true;
}

bool Analyzer::Test()
{
	DRONE_COMMAND cmd = HOVERING;
	mode_changed = false;
	if(diff_timeval(stop_timer) > runtime * 1000)
	{
		CPDBG("10sec... Landing\n");
		if(realtest) Land();
		return false;
	}
	if(!ReceiveStereo()){
		CPDBG("Fail to get a stereo info.. Landing\n");
		if(realtest) Land();
		return false;
	}
	if(!ReceiveNavdata()){
		CPDBG("Fail to get a navdata... Landing");
		if(realtest) Land();
		return false;
	}

	switch(state)
	{
		case NORMAL:
			cmd = NormalMode();
			break;
		case SIDLE:
			cmd = SidleMode();
			break;
		case PASSBY:
			cmd = PassbyMode();
			break;
		case LOOKASIDE:
			cmd = LookasideMode();
			break;
		case HEADSTRAIGHT:
			cmd = HeadMode();
			break;
		case RETURN:
			cmd = ReturnMode();
			break;
		default:
			CPDBG("shit error!\n");
			cmd = STOP;
			break;
	}
	if(mode_changed || 0.95*COMMAND_INTERVAL < diff_timeval(cmd_timer))
	{
		if(mode_changed == true)
		{
			CPDBG("state is changed, its state is %d now\n", state);
		}
		CPDBG("Diff: %f msec, from beginning: %f msec\n", diff_timeval(cmd_timer), diff_timeval(stop_timer));
		if(state == SIDLE)
		{
			if(cmd == MOVEL) move_cnt--;
			if(cmd == MOVER) move_cnt++;
		}
		else if(state == RETURN)
		{
			if(cmd == MOVEL) move_cnt--;
			if(cmd == MOVER) move_cnt++;
		}
		if(state == PASSBY)
		{
			CPDBG("lookaside_timer: %f\n", diff_timeval(lookaside_timer));
		}
		
		PrintInfo();
		PrintProcessed();
		CPDBG("Command : ");
		print_cmd(cmd);
		imshow("stereo",stereo_data);
        int waittime = 10;
        if(realtest) waittime = 10000;
		char ch = waitKey(waittime);
		if(ch == 'q'){
			Land();
			return false;
		}
		if(realtest) SendCommand(cmd);
		init_timeval(&cmd_timer);
	}
	return true;
}

void Analyzer::Set_video_source(char name[])
{
	video_source = new VideoCapture(name);
	if(!video_source->isOpened()){
		CPDBG("cannot open the video file\n");
		return;
	}
	stereo_source = VIDEO_STREAM;
}

void Analyzer::SendCommand(DRONE_COMMAND cmd) // message or be connected
{
	DRONE_OUT msg;
	int msg_size = 0;
	int rtn;

	msg.msgtype = 1; // should receive only '1' on opposite side
	msg.cmd = cmd;
	msg_size = sizeof(msg) - sizeof(msg.msgtype);
	rtn = msgsnd(drone_key, &msg, msg_size, 0);

	if(rtn == -1)
	{
		CPDBG("Fail to send command!!!@@@ \n");
	}
}

/*
bool Analyzer::ReceiveSVO()
{
	SVO_IN msg;
	ssize_t nbytes;
	int msg_size = sizeof(msg) - sizeof(msg.msgtype);

	nbytes = msgrcv(svo_key, &msg, msg_size, SVO_KEY, 0);
	if(nbytes < 0)
	{
		CPDBG("Fail to receive SVO data\n");
		return false;
	}
	memcpy(&svo_data, &msg.svo, msg_size);
	return true;
}
*/

bool Analyzer::ReceiveNavdata()
{
	DRONE_IN *msg;
	int shmid;
	ssize_t nbytes;
//	int msg_size = sizeof(msg);
	int msg_size = sizeof(NAVDATA);

	if((shmid = shmget((key_t) 5678, sizeof(DRONE_IN), 0777 | IPC_CREAT))<0){
		printf("Cannot Create shm!!!@@@\n");
		return false;
	}

	if((msg = (DRONE_IN*)shmat(shmid, NULL, 0)) == (DRONE_IN *) -1){
		printf("Failed attach memory\n");
		return false;
	}

/*	nbytes = msgrcv(drone_key, &msg, msg_size, DRONE_KEY, 0);
	if(nbytes < 0)
	{
		CPDBG("Fail to receive Navigation data\n");
		return false;
	}*/
	memcpy(&nav_data, &(msg->nav), msg_size);
	return true;
}

bool Analyzer::ReceiveStereo()
{
	int msg_size;
	STEREO_IN msg;
	ssize_t nbytes;
	switch(stereo_source)
	{
		case MESSAGE_STREAM:
			msg_size = sizeof(msg) - sizeof(msg.msgtype);
            msg.msgtype = STEREO_KEY;
			nbytes = msgrcv(stereo_key, (void *)&msg, msg_size, STEREO_KEY, 0);
			if(nbytes == -1)
			{
				CPDBG("Fail to receive Stereo data\n");
				return false;
			}

			stereo_data = Mat(Size(msg.width, msg.height), CV_8UC1, msg.mData);
			break;
		case VIDEO_STREAM:
			usleep(1000*COMMAND_INTERVAL/3);
			(*video_source) >> stereo_data;	
			if(stereo_data.empty()){
				CPDBG("Video stream finished\n");
				return false;
			}
			break;
		default:
			CPDBG("Not implemented yet\n");
	}
	if(sequences != 0)	memcpy(previous_data, processed_data, sizeof(processed_data));
	
	ProcessStereo();
	if(sequences != 0) ProcessNoise();

	sequences++;
	return true;
}


DRONE_COMMAND Analyzer::NormalMode()
{
	DRONE_COMMAND cmd = MOVEF;
	if(CenterBlocked())
	{
		cmd = HOVERING;
		SetSidle();
	}
	return cmd;
}

DRONE_COMMAND Analyzer::SidleMode()
{
	DRONE_COMMAND cmd = MOVER; // SPINR?
	if(!CenterBlocked())
	{
		cmd = HOVERING;
		state = PASSBY;
		mode_changed = true;
		init_timeval(&lookaside_timer);
	}
	else{
		cmd = sidle_cmd;
	}
	return cmd;
}

void Analyzer::SetSidle()
{
	int l = LeftDepth();
	int r = RightDepth();
	init_psi = nav_data.psi;
	mode_changed = true;
	state = SIDLE;
	passed = false;
	if(state == PASSBY) return;
	if(l > r) sidle_cmd = MOVEL;
	else{
		if(r > l) sidle_cmd = MOVER;
		else{
			if (sidle_cmd == MOVER) sidle_cmd = MOVEL;
			else sidle_cmd = MOVER;
		}
	}
}

DRONE_COMMAND Analyzer::PassbyMode()
{
	DRONE_COMMAND cmd = MOVEF;
	if(CenterBlocked())
	{
		cmd = HOVERING;
		SetSidle();
	}
	if(20 * COMMAND_INTERVAL < diff_timeval(lookaside_timer))
	{
		cmd = HOVERING;
		mode_changed = true;
		state = LOOKASIDE;
	}
	return cmd;
}

float Analyzer::DeltaPsi()
{
	float delta = nav_data.psi - init_psi;
	if(delta > 180000) return delta - 360000;
	if(delta < -180000) return delta + 360000;
	return delta;
}

DRONE_COMMAND Analyzer::LookasideMode()
{
	if(DeltaPsi() > 90000 || DeltaPsi() < -90000)
	{
		passed = true;
		mode_changed = true;
		state = HEADSTRAIGHT;
		return HOVERING;
	}
	if(CenterBlocked())
	{
		mode_changed = true;
		state = HEADSTRAIGHT;
		return HOVERING;
	}
	if(sidle_cmd == MOVEL) return SPINR;
	else return SPINL;
}

DRONE_COMMAND Analyzer::HeadMode()
{
	float delPsi = DeltaPsi();
	if(delPsi < 5000 && delPsi  > -5000)
	{
		mode_changed = true;
		if(passed) state = RETURN;
		else{
			state = PASSBY;
			init_timeval(&lookaside_timer);
		}
		return HOVERING;
	}
	if(delPsi < 0) return SPINR;
	else return SPINL;
}

DRONE_COMMAND Analyzer::ReturnMode()
{
	DRONE_COMMAND cmd;
	if(CenterBlocked())
	{
		cmd = HOVERING;
		SetSidle();
	}
	else
	{
		if(move_cnt == 0)
		{
			mode_changed = true;
			cmd = HOVERING;
			state = NORMAL;
		}
		else
		{
			if(move_cnt > 0) cmd = MOVEL;
			else cmd = MOVER;
		}
	}
	return cmd;
}


void Analyzer::ProcessNoise()
{
	//TODO: simple filtering
	
	//Process when previous data is "cv::Mat"
	/*
	int i, j;
	int delta;
	unsigned char *src;
	unsigned char *prev;
	for(i = 0; i< CAMERA_WIDTH; i++)
		for(j = 0 ; j < CAMERA_HEIGHT; j++)
		{
			src = stereo_data.ptr<unsigned char>(j,i);
			prev = previous_data.ptr<unsigned char>(j,i);
			delta = src - prev;
			// Cannot use this information.....
		}
	*/
	int i, j;
	int x, y;
	int minx, miny;
	int maxx, maxy;
	int mid = PARR_LENGTH/2;
	if(PARR_LENGTH<=4) return; 
	for(i=mid-1;i<=mid+1;i++)
		for(j=mid-1;j<=mid+1;j++)
		{
			int cnt = 0;
			int mindelta = 255;
			minx = i-1;
			miny = j-1;
			maxx = i+2;
			maxy = j+2;
			for(x=minx;x<maxx;x++)
				for(y=miny;y<maxy;y++)
				{
					int delta = processed_data[i][j] - previous_data[x][y];
					if((double)delta/(processed_data[i][j]+100) < 0.3) cnt++;
					else mindelta = mindelta < delta? mindelta:delta;
				}
			if(cnt == 0)
			{
				processed_data[i][j]-=mindelta;
			}
		}
}

void Analyzer::ProcessStereo()
{
	int i, j;
	for(i = 0; i < PARR_LENGTH ; i++)
	{
		for(j = 0 ; j < PARR_LENGTH; j++)
		{
			// process processed_data[i][j]!
			int xleftmost = (stereo_data.cols*i)/PARR_LENGTH;
			int xrightmost = (stereo_data.cols*(i+1))/PARR_LENGTH;
			int yleftmost = (stereo_data.rows*j)/PARR_LENGTH;
			int yrightmost = (stereo_data.rows*(j+1))/PARR_LENGTH;
			unsigned char temp = 0;
			unsigned char maxval = 0x0;
			int x,y;
			for(x = xleftmost ; x<xrightmost ; x++)
			{
				for(y = yleftmost ; y<yrightmost ; y++)
				{
					// should be more complex because of the noise
					temp = *(stereo_data.ptr<unsigned char>(y,x));
                    temp = temp>=150?0:temp; /// minimize noise.....
					if(temp>maxval) maxval = temp;
				}
			}
			processed_data[i][j]=maxval;
		}
	}
}

bool Analyzer::CenterBlocked()
{
	int mid = PARR_LENGTH/2;
	int bias = 0;
	if(state == SIDLE) bias = 0;
	if(state == LOOKASIDE) bias = 0;
	unsigned char basis = 20;
	int i,j;
	int cnt = 0;
	for(i=mid-1;i<=mid+1;i++)
		for(j=mid-1;j<=mid+1;j++)
			if(processed_data[j][i] > basis-bias){
				if(i==mid) cnt+=1;
				if(j==mid) cnt+=1;
				cnt++;
			}
	return cnt>=4;
}

int Analyzer::LeftDepth()
{
	int mid = PARR_LENGTH/2;
	unsigned char basis = 20;
	int depcnt = 0;
	int cnt = 0;
	int i,j;
	for(i=0;i<=mid;i++)
	{
		cnt = 0;
		for(j=mid-1;j<=mid+1;j++)
			if(processed_data[i][j] > basis) cnt++;
		if(cnt>0) break;
		else depcnt++;
	}
	return depcnt;
}

int Analyzer::RightDepth()
{
	int mid = PARR_LENGTH/2;
	unsigned char basis = 20;
	int depcnt = 0;
	int cnt = 0;
	int i,j;
	for(i=PARR_LENGTH-1;mid<=i;i--)
	{
		cnt = 0;
		for(j=mid-1;j<=mid+1;j++)
			if(processed_data[i][j] > basis) cnt++;
		if(cnt>0) break;
		else depcnt++;
	}
	return depcnt;

}

void Analyzer::PrintProcessed()
{
	int i ,j;
	for(i = 0; i < PARR_LENGTH ; i++)
	{
		for(j = 0 ; j < PARR_LENGTH; j++)
		{
			int out = processed_data[j][i];
			printf("[%d] ", out);
		}
		printf("\n");
	}
	printf("--------------------------------\n");
}
