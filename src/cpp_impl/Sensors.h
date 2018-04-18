#ifdef __linux
#include <raspicam/raspicam_cv.h>
#include "Sensors.h"
#include<wiringPi.h>

class Sensors
{
private:
	int m_TRIG=3; //GPIO BCM 2
	int m_ECHO=5; //GPIO BCM 3
	int m_IR_D0=7; //GPIO BCM 4
public:
	Sensors();
	int getIR();
	int getDistance();
};
#endif