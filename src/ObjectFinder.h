#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
class ObjectFinder 
{
private:
    std::vector<std::vector<cv::Point> > m_refContours;
    cv::Mat m_refObject;
    int m_refContourIdx;
    int m_largestContourIndex;
    std::vector<std::vector<cv::Point> > m_contours;

    void drawFoundContour(cv::Mat image);
    void initReferenceObject(std::string pathToRefImg);
    int findLargestContour(std::vector<std::vector<cv::Point> > contours);
    bool findObjectInFrame(cv::Mat frame, cv::Mat frameMask);
    bool centerObject(cv::Mat image);
    void driveToObject();
public:
    ObjectFinder(std::string pathToRefImg); 
    void processImage(cv::Mat image);
    
};