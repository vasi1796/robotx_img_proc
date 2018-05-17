//comand for compiling 
//g++ $(pkg-config --libs --cflags opencv) -o file open.cpp

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#ifdef __linux
#include <raspicam/raspicam_cv.h>
#include "Sensors.h"
#include "pi2c.h"
void testLinuxFunctions()
{
    Pi2c arduino1(4); //Create a new object "arduino" using address "0x07"
    Pi2c arduino2(5);
    char receive1[16]; //Create a buffer of char (single bytes) for the data
    char receive2[16];

    //Receive from the Arduino and put the contents into the "receive" char array
    arduino1.i2cRead(receive1, 16);
    arduino2.i2cRead(receive2, sizeof(float) * 3);
    float u1;
    float u2;
    float u3;
    memcpy(&u1, &receive2[0], sizeof(float));
    memcpy(&u2, &receive2[sizeof(float)], sizeof(float));
    memcpy(&u3, &receive2[sizeof(float) * 2], sizeof(float));
    //Print out what the Arduino is sending...
    std::cout << "Arduino1 Says: " << receive1 << std::endl;
    std::cout << "Arduino2 Says: " << u1 << "," << u2 << "," << u3 << std::endl;

    //Send an 16 bit integer
    arduino1.i2cWriteArduinoInt(69);
    arduino2.i2cWriteArduinoInt(96);
    /*Sensors sensor;
    while (true)
    {
    sensor.getIR();
    sensor.getDistance();
    }*/
}
#elif _WIN32
// windows code goes here
#endif
#include <iostream>
#include "ObjectFinder.h"
int main( int argc, char** argv )
{
    cv::Mat image;
    ObjectFinder finder("../res/ref.jpg");

#ifdef __linux
	testLinuxFunctions();
    return 0;
	cv::VideoCapture cap(0);
    /*raspicam::RaspiCam_Cv cap;
    cap.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    if (!cap.open())                              // Check for invalid input
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }*/
#elif _WIN32
    cv::VideoCapture cap(1);
    
    if (!cap.isOpened())
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
#endif
    while (cv::waitKey(1) != 'q')
    {
        cap.grab();
        cap.retrieve(image);
        cv::resize(image, image, cv::Size(400, 400));
        
        finder.processImage(image);

        cv::rectangle(image, cv::Rect(180, 0, 50, 400), cv::Scalar(255, 0, 0), 2);
        cv::rectangle(image, cv::Rect(180, 290, 50, 400), cv::Scalar(0, 255, 0), 2);
        imshow("original", image);
    }
    cv::destroyAllWindows();
#ifdef __linux
    cap.release();
#endif
    return 0;
}
