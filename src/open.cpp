//comand for compiling 
//g++ $(pkg-config --libs --cflags opencv) -o file open.cpp

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include <iostream>
#include "ColorSegmentation.h"

int main( int argc, char** argv )
{
    cv::Mat image;
    raspicam::RaspiCam_Cv Camera;
    Camera.set(CV_CAP_PROP_FORMAT,CV_8UC3);
    if(!Camera.open())                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
	while(cv::waitKey(1) != 'q')
	{
		Camera.grab();
		Camera.retrieve(image);
		cv::resize(image,image,cv::Size(300,300));
		imshow("screen",image);
	}
	Camera.release();
    return 0;
}
