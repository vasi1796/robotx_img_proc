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

int main( int argc, char** argv )
{
    cv::Mat image;
    cv::Mat msk(400, 400, CV_8UC1);
    std::vector<std::vector<cv::Point> > contours;
#ifdef __linux
    raspicam::RaspiCam_Cv Camera;
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    if (!Camera.open())                              // Check for invalid input
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
#elif _WIN32
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        return -1;
    }
#endif

    while (cv::waitKey(1) != 'q')
    {
#ifdef __linux
        Camera.grab();
        Camera.retrieve(image);
#elif _WIN32
        cap >> image;
        cv::resize(image, image, cv::Size(400, 400));
#endif
        ColorSegmentation::OthaSpaceThresholdingRGB(image, true, false, false, msk);
        //dilate the msk to cover white spots
        cv::dilate(msk, msk, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);

        cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
        cv::findContours(msk.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        int largest_area = 0;
        int largest_contour_index = 0;
        cv::Rect bounding_rect;
        for (size_t i = 0; i< contours.size(); i++)
        {
            double area = contourArea(contours[i]);

            if (area > largest_area)
            {
                largest_area = area;
                largest_contour_index = i;               
                bounding_rect = boundingRect(contours[i]);
            }
        }
        if (bounding_rect.area())
        {
            cv::rectangle(image, bounding_rect, cv::Scalar(0, 255, 0), 2, 8, 0);
        }
        drawContours(contourImage, contours, largest_contour_index, cv::Scalar(0, 255, 0), 2);

        imshow("original", image);
        imshow("contours", contourImage);
        imshow("mask", msk);
    }
    cv::destroyAllWindows();
#ifdef __linux
    Camera.release();
#endif
    return 0;
}

