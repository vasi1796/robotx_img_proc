//comand for compiling 
//g++ $(pkg-config --libs --cflags opencv) -o file open.cpp

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#ifdef __linux__ 
#include <raspicam/raspicam_cv.h>
#elif _WIN32
// windows code goes here
#endif
#include <iostream>
#include "ColorSegmentation.h"

int main( int argc, char** argv )
{
    cv::Mat image;
    cv::Mat msk(400, 400, CV_8UC1);
    std::vector<std::vector<cv::Point> > contours;
#ifdef __linux__ 
    raspicam::RaspiCam_Cv Camera;
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    if (!Camera.open())                              // Check for invalid input
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    while (cv::waitKey(1) != 'q')
    {
        Camera.grab();
        Camera.retrieve(image);
        cv::resize(image, image, cv::Size(400, 400));
        ColorSegmentation::OthaSpaceThresholdingRGB(image, true, false, false, msk);
        cv::findContours(msk.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
        imshow("original", image);
        imshow("mask", msk);
}
    cv::destroyAllWindows();
    Camera.release();
#elif _WIN32
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        return -1;
    }
    while (cv::waitKey(1) != 'q')
    {
        cap >> image;
        cv::resize(image, image, cv::Size(400, 400));
        ColorSegmentation::OthaSpaceThresholdingRGB(image, true, false, false, msk);
        cv::findContours(msk.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
        imshow("original", image);
        imshow("mask", msk);
    }
#endif
    return 0;
}

