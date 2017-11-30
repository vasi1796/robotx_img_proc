//comand for compiling 
//g++ $(pkg-config --libs --cflags opencv) -o file open.cpp

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#ifdef __linux
#include <raspicam/raspicam_cv.h>
#elif _WIN32
// windows code goes here
#endif
#include <iostream>
#include "ColorSegmentation.h"
#include "ObjectFinder.h"

int main( int argc, char** argv )
{
    cv::Mat image;
    ObjectFinder finder("../res/ball.jpg");

#ifdef __linux
    raspicam::RaspiCam_Cv cap;
    cap.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    if (!cap.open())                              // Check for invalid input
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
#elif _WIN32
    cv::VideoCapture cap(0);
    
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

        imshow("original", image);
    }
    cv::destroyAllWindows();
#ifdef __linux
    cap.release();
#endif
    return 0;
}

