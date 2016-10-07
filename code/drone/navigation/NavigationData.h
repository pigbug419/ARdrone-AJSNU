///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-07
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	This document defines the basic ARdrone navdata structure.


#ifndef ARDRONEPP_DRONE_NAVIGATION_NAVIGATION_DATA_H_
#define ARDRONEPP_DRONE_NAVIGATION_NAVIGATION_DATA_H_

#include <cstdint>

namespace ardronepp{
	namespace navdata{
		enum NAVDATA_TAG {
			DEMO_TAG = 0, TIME_TAG = 1,
			RAW_MEASURES_TAG = 2, PHYS_MEASURES_TAG = 3,
			GYROS_OFFSETS_TAG = 4, EULER_ANGLES_TAG = 5,
			REFERENCES_TAG = 6, TRIMS_TAG = 7,
			RC_REFERENCES_TAG = 8, PWM_TAG = 9,
			ALTITUDE_TAG = 10, VISION_RAW_TAG = 11,
			VISION_OF_TAG = 12, VISION_TAG = 13,
			VISION_PERF_TAG = 14, TRACKERS_SEND_TAG = 15,
			VISION_DETECT_TAG = 16, WATCHDOG_TAG = 17,
			IPHONE_ANGLES_TAG = 18, ADC_DATA_FRAME_TAG = 18,
			VIDEO_STREAM_TAG = 19, GAME_TAG = 20,
			PRESSURE_RAW_TAG = 21, MAGNETO_TAG = 22,
			WIND_TAG = 23, KALMAN_PRESSURE_TAG = 24,
			HDVIDEO_STREAM_TAG = 25, WIFI_TAG = 26,
			ZIMMU3000_TAG = 27, GPS_TAG = 27,
			CKS_TAG = 0xFFFF
		};

		struct EulerMat {
			float m11, m12, m13;
			float m21, m22, m23;
			float m31, m32, m33;
		};

		struct Point3 {
			float x, y, z;
		};

		struct DemoData {
			std::uint32_t	controlState;		/*!< Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum. */
			std::uint32_t	batLevel;			/*!< battery voltage filtered (mV) */

			float			theta;				/*!< UAV's pitch in milli-degrees */
			float			phi;				/*!< UAV's roll  in milli-degrees */
			float			psi;				/*!< UAV's yaw   in milli-degrees */

			std::int32_t	altitude;			/*!< UAV's altitude in centimeters */

			float			vx;					/*!< UAV's estimated linear velocity */
			float			vy;					/*!< UAV's estimated linear velocity */
			float			vz;					/*!< UAV's estimated linear velocity */

			std::uint32_t	numFrames;			/*!< streamed frame index */ // Not used -> To integrate in video stage.

			// Camera parameters compute by detection
			EulerMat		rotByCamera;		/*!<  Deprecated ! Don't use ! */
			Point3			tranByCamera;		/*!<  Deprecated ! Don't use ! */
			std::uint32_t	detectedTagIndex;	/*!<  Deprecated ! Don't use ! */

			std::uint32_t	tagType;			/*!<  Type of tag searched in detection */

			// Camera parameters compute by drone
			EulerMat		drone_camera_rot;	/*!<  Deprecated ! Don't use ! */
			Point3			drone_camera_trans;	/*!<  Deprecated ! Don't use ! */
		};

		struct Altitude{
			unsigned short tag;
			unsigned short size;
			int            altitude_vision;
			float          altitude_vz;
			int            altitude_ref;
			int            altitude_raw;
			float          obs_accZ;
			float          obs_alt;
			float			obs_x[3];
			unsigned int   obs_state;
			float			est_vb[2];
			unsigned int   est_state;
		};

		struct TimeStamp {
			unsigned short tag;
			unsigned short size;
			unsigned int   time;
		};

		struct Speeds {
			float x;
			float y;
			float z;
		};

		struct WatchDog {
			unsigned short tag;
			unsigned short size;
			int            watchdog;
		};


		struct RawImu {
			unsigned short tag;
			unsigned short size;
			float          accs_temp;
			unsigned short gyro_temp;
			float          phys_accs[3];
			float          phys_gyros[3];
			unsigned int   alim3V3;         // 3.3 volt alim       [LSB]
			unsigned int   vrefEpson;       // ref volt Epson gyro [LSB]
			unsigned int   vrefIDG;         // ref volt IDG gyro   [LSB]
		};

		struct RawPressure {
			unsigned short tag;
			unsigned short size;
			unsigned int   up;
			unsigned short ut;
			unsigned int   temperature_meas;
			unsigned int   pression_meas;
		};

		struct Magneto {
			unsigned short	tag;
			unsigned short	size;
			short			mx;
			short			my;
			short			mz;
			Point3			magneto_raw;             // magneto in the body frame, in mG
			Point3			magneto_rectified;
			Point3			magneto_offset;
			float			heading_unwrapped;
			float			heading_gyro_unwrapped;
			float			heading_fusion_unwrapped;
			char			magneto_calibration_ok;
			unsigned int	magneto_state;
			float			magneto_radius;
			float			error_mean;
			float			error_var;
			float			tmp1, tmp2;              // dummy ?
		};

		struct Wind {
			unsigned short tag;
			unsigned short size;
			float          wind_speed;              // estimated wind speed [m/s]
			float          wind_angle;              // estimated wind direction in North-East frame [rad] e.g. if wind_angle is pi/4, wind is from South-West to North-East
			float          wind_compensation_theta;
			float          wind_compensation_phi;
			float          state_x1;
			float          state_x2;
			float          state_x3;
			float          state_x4;
			float          state_x5;
			float          state_x6;
			float          magneto_debug1;
			float          magneto_debug2;
			float          magneto_debug3;
		};

		struct PressureFiltered {
			unsigned short tag;
			unsigned short size;
			float          offset_pressure;
			float          est_z;
			float          est_zdot;
			float          est_bias_PWM;
			float          est_biais_pression;
			float          offset_US;
			float          prediction_US;
			float          cov_alt;
			float          cov_PWM;
			float          cov_vitesse;
			bool           bool_effet_sol;
			float          somme_inno;
			bool           flag_rejet_US;
			float          u_multisinus;
			float          gaz_altitude;
			bool           flag_multisinus;
			bool           flag_multisinus_debut;
		};

		struct Wifi {
			unsigned short tag;
			unsigned short size;
			unsigned int   link_quality;
		};

		struct CheckSum {
			unsigned short tag;
			unsigned short size;
			unsigned int   cks;
		};

	}	//	namespace navdata
}	//	namespace ardronepp
#endif	//	ARDRONEPP_DRONE_NAVIGATION_NAVIGATION_DATA_H_