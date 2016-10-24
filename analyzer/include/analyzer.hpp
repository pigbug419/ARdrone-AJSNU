#ifndef __DRONE_ANALYZER_HPP__
#define __DRONE_ANALYZER_HPP__

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "types.h"

#define PARR_LENGTH 5


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
	bool Run2(); // run scenario 2
	bool Test(); // function for testing
	void Set_video_source(char name[]);
	void PrintInfo();
	void SendCommand(DRONE_COMMAND cmd);
private:
	bool ReceiveSVO();
	bool ReceiveStereo();
	bool ReceiveNavdata(); // failed = false

	// Scenario 1
	DRONE_COMMAND NormalMode();
	DRONE_COMMAND SwerveMode();
	DRONE_COMMAND GopastMode();
	DRONE_COMMAND ReturnMode();
	
	// Scenario without SVO
	DRONE_COMMAND next_swerve;
	DRONE_COMMAND SwitchSwerve();

	// subfunctions related to stereo vision
	
	void ProcessNoise();
	void ProcessStereo();
	bool CenterBlocked();
	int LeftDepth();
	int RightDepth();

	void PrintProcessed();
	key_t stereo_key;
	key_t svo_key;
	key_t drone_key;
	
	int state; // 0 : go straight , 1 : swerve, 2: go past ,3 : retrun to origin route
	// 0 -> 1 <-> 2 -> 3 -> 0 (FSM), other transition is not permitted
	NAVDATA nav_data;
	SVODATA svo_data;
	cv::Mat stereo_data;
	cv::Mat previous_data;
	unsigned char processed_data[PARR_LENGTH][PARR_LENGTH];
	STEREOTYPE stereo_source;
	cv::VideoCapture* video_source;
};

//__DRONE_ANALYZER_HPP__
#endif
