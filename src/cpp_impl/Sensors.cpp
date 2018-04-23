#ifdef __linux
#include "Sensors.h"
#include <iostream>
#include <thread>
#include <ctime>
Sensors::Sensors()
{
	wiringPiSetupGpio();
    pinMode(m_IR_D0, INPUT); 
    pinMode(m_TRIG, OUTPUT); 
    pinMode(m_ECHO, INPUT); 

}
int Sensors::getIR()
{
	// Read input from pin 4
    int status = digitalRead(m_IR_D0);
    std::cout<<"IR blocked = "<<status<<std::endl;
}
int Sensors::getDistance()
{
	std::clock_t pulse_start,pulse_end;
	digitalWrite(m_TRIG,0);
	//printf("Stabilize sensor\n");
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	//printf("Start reading\n");
	digitalWrite(m_TRIG,1);
	std::this_thread::sleep_for(std::chrono::microseconds(10));
	digitalWrite(m_TRIG,0);
	while(digitalRead(m_ECHO)==0)
	{
		pulse_start=std::clock();
	}
	while(digitalRead(m_ECHO)==1)
	{
		pulse_end=std::clock();
	}
	double pulse_duration=double(pulse_end-pulse_start)/100000;
	double distance=pulse_duration*1715;

	std::cout<<"Distance = "<<distance<<" cm"<<std::endl;
}
#endif
