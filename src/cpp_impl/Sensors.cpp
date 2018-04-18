#include "Sensors.h"
#include <stdio.h>
#include <thread>
#include <chrono>
#include <iostream>
Sensors::Sensors()
{
	wiringPiSetup();
    pinMode(m_IR_D0, INPUT); 
    pinMode(m_TRIG, OUTPUT); 
    pinMode(m_ECHO, INPUT); 

}
int Sensors::getIR()
{
	// Read input from pin 4
    int status = digitalRead(m_IR_D0);
    printf("Pin Input = %d\n", status);
}
int Sensors::getDistance()
{
	std::chrono::high_resolution_clock::time_point pulse_start,pulse_end;
	digitalWrite(m_TRIG,0);
	printf("Stabilize sensor\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	digitalWrite(m_TRIG,1);
	printf("Start reading\n");
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	digitalWrite(m_TRIG,0);
	while(digitalRead(m_ECHO)==0)
	{
		std::cout<<digitalRead(m_ECHO)<<std::endl;
		pulse_start=std::chrono::high_resolution_clock::now();
	}
	while(digitalRead(m_ECHO)==1)
	{
		pulse_end=std::chrono::high_resolution_clock::now();
	}
	auto pulse_duration=pulse_end-pulse_start;
	double distance=pulse_duration.count()*17150;

	printf("Distance = %lf\n", distance);
}
