#ifndef __DRONE_TYPES_H__
#define __DRONE_TYPES_H__


#define CAMERA_WIDTH		320
#define CAMERA_HEIGHT		240

#define STEREO_KEY			8800
#define DRONE_KEY			8700
#define SVO_KEY				8600

#define CPDBG(msg, ...) printf(msg, ##__VA_ARGS__)

enum DRONE_COMMAND {
	TAKEOFF,
	LAND,
	HOVERING,
	SPINR, // spin right/left
	SPINL,
	MOVEF, // move forward/right/left/backward/up/down
	MOVER,
	MOVEL,
	MOVEB,
	MOVEU,
	MOVED,
	STOP
};


typedef struct {
	long msgtype;
	unsigned char mData[CAMERA_WIDTH * CAMERA_HEIGHT];
} STEREO_IN;

// DRONE_* will not be used if analyzer & data handler are located in the same thread.

typedef struct {
	float vx, vy, vz; // vx vy vz
	float theta, phi, psi;
	int altitude;
	bool is_flying;
} NAVDATA;

typedef struct {
	long msgtype;
	NAVDATA nav;
} DRONE_IN;

typedef struct {
	long msgtype;
	enum DRONE_COMMAND cmd;
} DRONE_OUT; // for controller

typedef struct {
	int x, y, z;
} SVODATA;

typedef struct {
	long msgtype;
	SVODATA svo;
} SVO_IN;

void print_cmd(enum DRONE_COMMAND cmd);

//__DRONE_TYPES_H__
#endif
