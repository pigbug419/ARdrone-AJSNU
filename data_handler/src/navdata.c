#include <navdata.h>
#include <stdio.h>

void print_navdata(navdata_t navdata)
{	
    printf("navdata header:\n");
    printf("\t%13x:%s\n", navdata.navdata_header.header, "Header"          );
    printf("\t%13x:%s\n", navdata.navdata_header.state,  "drone's state"   );
    printf("\t%13d:%s\n", navdata.navdata_header.seq,    "sequence number" );
    printf("\t%13d:%s\n", navdata.navdata_header.vision, "vision flag"     );
    printf("\t%13d:%s\n", navdata.navdata_option.id,     "Option1 ID"      );
    printf("\t%13d:%s\n", navdata.navdata_option.size,   "Option1 size"    );
    printf("drone's state:\n");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  0))!=0, "FLY MASK");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  1))!=0, "VIDEO MASK");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  2))!=0, "VISION MASK");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  3))!=0, "CONTROL ALGO");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  4))!=0, "ALTITUDE CONTROL ALGO");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  5))!=0, "USER feedback");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  6))!=0, "Control command ACK");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  7))!=0, "Trim command ACK");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  8))!=0, "Trim running");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 <<  9))!=0, "Trim result");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 10))!=0, "Navdata demo");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 11))!=0, "Navdata bootstrap");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 12))!=0, "Motors status");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 13))!=0, "Communication Lost");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 14))!=0, "problem with gyrometers");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 15))!=0, "VBat low");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 16))!=0, "VBat high");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 17))!=0, "Timer elapsed");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 18))!=0, "Power");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 19))!=0, "Angles");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 20))!=0, "Wind");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 21))!=0, "Ultrasonic sensor");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 22))!=0, "Cutout system detection");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 23))!=0, "PIC Version number OK");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 24))!=0, "ATCodec thread");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 25))!=0, "Navdata thread");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 26))!=0, "Video thread");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 27))!=0, "Acquisition thread");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 28))!=0, "CTRL watchdog");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 29))!=0, "ADC Watchdog");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 30))!=0, "Communication Watchdog");
    printf("\t%13d:%s\n",(navdata.navdata_header.state & (1 << 31))!=0, "Emergency landing");
    printf("navdata_demo:\n");
    printf("\t%13d:%s\n",   navdata.navdata_option.ctrl_state,             "Control State");
    printf("\t%13d:%s\n",   navdata.navdata_option.vbat_flying_percentage, "battery");
    printf("\t%13.3f:%s\n", navdata.navdata_option.theta,                  "pitch angle");
    printf("\t%13.3f:%s\n", navdata.navdata_option.phi,                    "roll  angle");
    printf("\t%13.3f:%s\n", navdata.navdata_option.psi,                    "yaw   angle");
    printf("\t%13d:%s\n",   navdata.navdata_option.altitude,               "altitude");
    printf("\t%13.3f:%s\n", navdata.navdata_option.vx,                     "estimated vx");
    printf("\t%13.3f:%s\n", navdata.navdata_option.vy,                     "estimated vy");
    printf("\t%13.3f:%s\n", navdata.navdata_option.vz,                     "estimated vz");
}

float32_t vx(navdata_t navdata)
{
	return navdata.navdata_option.vx;
}
float32_t vy(navdata_t navdata)
{
	return navdata.navdata_option.vy;
}
float32_t vz(navdata_t navdata)
{
	return navdata.navdata_option.vz;
}

float32_t theta(navdata_t navdata)
{
	return navdata.navdata_option.theta;
}
float32_t psi(navdata_t navdata)
{
	return navdata.navdata_option.psi;
}
float32_t phi(navdata_t navdata)
{
	return navdata.navdata_option.phi;
}

int32_t altitude(navdata_t navdata)
{
	return navdata.navdata_option.altitude;
}

int32_t battery(navdata_t navdata)
{
	return navdata.navdata_option.vbat_flying_percentage;
}

