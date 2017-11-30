#pragma once
#include <opencv2/opencv.hpp>
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
    void centerObject(cv::Mat image, cv::Mat msk);
public:
    ObjectFinder(std::string pathToRefImg); 
    void processImage(cv::Mat image);
    
};