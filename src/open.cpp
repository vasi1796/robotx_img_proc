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
int returnLargestContour(std::vector<std::vector<cv::Point>> contours)
{
    int largest_area = 0;
    int largest_contour_index = 0;
    for (size_t index = 0; index< contours.size(); index++)
    {
        double area = contourArea(contours[index]);

        if (area > largest_area)
        {
            largest_area = area;
            largest_contour_index = index;
        }
    }
    return largest_contour_index;
}

int main( int argc, char** argv )
{
    cv::Mat image;
    cv::Mat msk(400, 400, CV_8UC1);
    std::vector<std::vector<cv::Point> > contours;

    std::vector<std::vector<cv::Point> > ref_contours;
    cv::Mat ref_obj=cv::imread("ball.jpg");
    cv::Mat ref_msk = ref_obj.clone();
    cv::cvtColor(ref_msk, ref_msk, cv::COLOR_BGR2GRAY);
    ColorSegmentation::OthaSpaceThresholdingRGB(ref_obj, true, false, false, ref_msk);
    cv::findContours(ref_msk, ref_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    int ref_contour_idx = returnLargestContour(ref_contours);
    drawContours(ref_obj, ref_contours, ref_contour_idx, cv::Scalar(0, 255, 0), 2);
    //cv::imshow("ball", ref_obj);
    //cv::waitKey(0);

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
        ColorSegmentation::OthaSpaceThresholdingRGB(image, true, false, false, msk);
        //dilate the msk to cover white spots
        cv::dilate(msk, msk, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);

        cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
        cv::findContours(msk.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        int largest_area = -1;
        int largest_contour_index = -1;

        ////maybe use matchShapes
        //double hu[7];
        //if (!contours.empty())
        //{
        //    cv::Moments mom=cv::moments(contours[largest_contour_index]);
        //    cv::HuMoments(mom, hu);
        //}
        //double avg = cv::matchShapes(contours[largest_contour_index], ref_contours[ref_contour_idx], 1, 0);
        
        for (size_t index = 0; index < contours.size(); index++)
        {
            double area = contourArea(contours[index]);

            if (area > 1000)
            {
                if (cv::matchShapes(contours[index], ref_contours[ref_contour_idx], 1, 0) < 0.1) 
                {
                    largest_contour_index = index;
                }
            }
        }
        if (largest_contour_index > 0)
        {
            drawContours(contourImage, contours, largest_contour_index, cv::Scalar(0, 255, 0), 2);
        }

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

