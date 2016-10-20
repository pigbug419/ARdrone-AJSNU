#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;
using namespace cv;

Analyzer::Analyzer()
{
	stereo_source = MESSAGE_STREAM;
	video_source = NULL;
	state = 0;
	stereo_key = STEREO_KEY;
	svo_key = SVO_KEY;
	drone_key = DRONE_KEY;
}

bool Analyzer::Initialize()
{
	bool flag = true;
	controldata.msgtype = drone_key;
	drone_key = msgget(drone_key, IPC_CREAT | 0666);
	if(drone_key = -1) {
		CPDBG("Fail to create a message queue - drone key\n");
		flag = false;
	}
	if(stereo_source == MESSAGE_STREAM)
	{
		camera_key = msgget(camera_key, IPC_CREAT | 0666);
		if(camera_key = -1) {
			CPDBG("Fail to create a message queue - camera key\n");
			flag = false;
		}
	}
	else
	{

	}

	svo_key = msgget(svo_key, IPC_CREAT | 0666);
	if(svo_key = -1) {
		CPDBG("Fail to create a message queue - svo key\n");
		flag = false;
	}
	CPDBG("Initialization finished\n");
	return flag;
}

void Analyzer::Prepare()
{
	//1. takeoff & confirm it
	//2. lift while altitude satisfies condition
}

void Analyzer::PrintInfo()
{
	printf("State:%d\nStereoSource:%d\nCamerakey:%d\nSvokey:%d\nDronekey%d\n", state, stereo_source, camera_key, svo_key, drone_key);
}

void Analyzer::Run()
{
	// run only one step! (for signal handling.. & scalability)
	DRONE_COMMAND cmd = HOVERING;
	switch(state)
	{
		case 0:
			if(!ReceiveStereo()){
				CPDBG("Fail to get a stereo info");
				break;
			}
			if(!ReceiveNavdata()){
				CPDBG("Fail to get a navdata");
				break;
			}
			NormalMode();
			break;
		case 1:
			if(!ReceiveStereo()){
				CPDBG("Fail to get a stereo info");
				break;
			}
			if(!ReceiveNavdata()){
				CPDBG("Fail to get a navdata");
				break;
			}
			if(!ReceiveSVO()){
				CPDBG("Fail to get a svo info");
				break;
			}
			SwerveMode();
			break;
		case 2:
			if(!ReceiveStereo()){
				CPDBG("Fail to get a stereo info");
				break;
			}
			if(!ReceiveNavdata()){
				CPDBG("Fail to get a navdata");
				break;
			}
			if(!ReceiveSVO()){
				CPDBG("Fail to get a svo info");
				break;
			}
			ReturnMode();
			break;
		default:
			CPDBG("shit error!\n");
			cmd = LAND;
			break;
	}
	SendCommand(cmd);	
}

void Analyzer::Test()
{
	//will be implemented..
}

void Set_video_source(char name[])
{
	video_source = new VideoCapture(name);
	if(!video_source->opened()){
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
	rtn = msgsnd(drone_key, msg, msg_size, 0);

	if(rtn == -1)
	{
		CPDBG("Fail to send command!!!@@@ \n");
	}
}

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

bool Analyzer::ReceiveNavdata()
{
	DRONE_IN msg;
	ssize_t nbytes;
	int msg_size = sizeof(msg) - sizeof(msg.msgtype);

	nbytes = msgrcv(drone_key, &msg, msg_size, DRONE_KEY, 0);
	if(nbytes < 0)
	{
		CPDBG("Fail to receive Navigation data\n");
		return false;
	}
	memcpy(&nav_data, &msg.navdata, msg_size);
	return true;
}

bool Analyzer::ReceiveStereo()
{
	switch(stereo_source)
	{
		case MESSAGE_STREAM:
			STEREO_IN msg;
			ssize_t nbytes;
			int msg_size = sizeof(msg) - sizeof(msg.msgtype);

			nbytes = msgrcv(stereo_key, &msg, msg_size, STEREO_KEY, 0);
			if(nbytes < 0)
			{
				CPDBG("Fail to receive Stereo data\n");
				return false;
			}
			stereo_data = Mat(Size(CAMERA_WIDTH, CAMERA_HEIGHT), CV_8UC1, msg.mData);
			break;
		case VIDEO_STREAM:
			video_source >> stereo_data;	
			if(img.empty()){
				CPDBG("Video stream finished\n");
				return false;
			}
			break;
		default:
			CPDBG("Not implemented yet\n");
	}
}

DRONE_COMMAND Analyzer::NormalMode()
{
	DRONE_COMMAND cmd = MOVEF;
	return cmd;
}

DRONE_COMMAND Analyzer::SwerveMode()
{
}

DRONE_COMMAND Analyzer::ReturnMode()
{

}