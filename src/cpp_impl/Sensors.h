#ifdef __linux
#include<wiringPi.h>

class Sensors
{
private:
	int m_TRIG=2; 
	int m_ECHO=3; 
	int m_IR_D0=4; 
public:
	Sensors();
	int getIR();
	int getDistance();
};
#endif
