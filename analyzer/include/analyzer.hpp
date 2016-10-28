#ifndef __DRONE_ANALYZER_HPP__
#define __DRONE_ANALYZER_HPP__

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "types.h"
#include "time_diff.h"

#define PARR_LENGTH 5
// T = 100ms
#define COMMAND_INTERVAL 100*1000
#define LOOKASIDE_INTERVAL 20

enum DRONESTATE{
	NORMAL,
	SIDLE,
	PASSBY,
	LOOKASIDE,
	HEADSTRAIGHT,
	RETURN
};

enum STEREOTYPE{
	MESSAGE_STREAM,
	VIDEO_STREAM,
	IMAGE_STREAM // not used yet
}; // for debuging!

class Analyzer
{
public:
	Analyzer();
	bool Initialize(); // false - fail to set the message queues
	void Prepare(); // takeoff & lift
	bool Run(); // run only one step...? TBD
	bool Test(); // function for testing
	void Set_video_source(char name[]);
	void Set_runttime(int time){runtime = time;}
	void PrintInfo();
	void Land();
private:
//	bool ReceiveSVO();
	bool ReceiveStereo();
	bool ReceiveNavdata(); // failed = false
	void SendCommand(DRONE_COMMAND cmd);	

	DRONE_COMMAND NormalMode();
	DRONE_COMMAND SidleMode();
	void SetSidle();	
	DRONE_COMMAND PassbyMode();
	DRONE_COMMAND LookasideMode();
	DRONE_COMMAND HeadMode();
	DRONE_COMMAND ReturnMode();

	void ProcessNoise();
	void ProcessStereo();
	bool CenterBlocked();
	int LeftDepth();
	int RightDepth();

	void PrintProcessed();
	float DeltaPsi();
//	key_t svo_key;
	key_t stereo_key;
	key_t drone_key;
	
	// NORMAL -> SIDLE -> PASSBY -> LOOKASIDE <-> HEADSTRAIGHT -> RETURN (FSM), other transition is not permitted
//	SVODATA svo_data;
	DRONESTATE state;
	
	NAVDATA nav_data;
	cv::Mat stereo_data;
	unsigned char previous_data[PARR_LENGTH][PARR_LENGTH];
	unsigned char processed_data[PARR_LENGTH][PARR_LENGTH];
	STEREOTYPE stereo_source;
	cv::VideoCapture* video_source;

	int sequences;
	int runtime;
	bool mode_changed;
	bool passed;
	struct timeval stop_timer;
	struct timeval cmd_timer;
	// lookaside for 2 sec
	struct timeval lookaside_timer;
		
	DRONE_COMMAND sidle_cmd;
	// ver 1 : compare w/ init_psi
	float init_psi;
	// ver 2 : counting # of spin cmd
	int spin_cnt;

	int move_cnt; // - : left, + : right;
};

//__DRONE_ANALYZER_HPP__
#endif
