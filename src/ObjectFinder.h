#pragma once
#include <opencv2/opencv.hpp>
class ObjectFinder 
{
private:
    std::vector<std::vector<cv::Point> > m_refContours;
    cv::Mat m_refObject;
    int m_refContourIdx;
public:
    std::vector<std::vector<cv::Point> > m_contours;
    ObjectFinder(std::string pathToRefImg);
    void initReferenceObject(std::string pathToRefImg);
    int findLargestContour(std::vector<std::vector<cv::Point> > contours);
    int findObjectInFrame(cv::Mat frame,cv::Mat frameMask);
};