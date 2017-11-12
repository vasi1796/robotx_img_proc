#include "ObjectFinder.h"
#include "ColorSegmentation.h"
ObjectFinder::ObjectFinder(std::string pathToRefImg)
{
    initReferenceObject(pathToRefImg);
}
int ObjectFinder::findLargestContour(std::vector<std::vector<cv::Point> > contours)
{
    double largest_area = -1;
    int largest_contour_index = -1;
    for (auto index = 0; index< contours.size(); index++)
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
int ObjectFinder::findObjectInFrame(cv::Mat frame,cv::Mat frameMask)
{
    ColorSegmentation::OthaSpaceThresholdingRGB(frame, true, false, false, frameMask);
    //dilate the msk to cover white spots
    cv::dilate(frameMask, frameMask, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);

    cv::Mat contourImage(frame.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    cv::findContours(frameMask.clone(), m_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    double largest_area = -1;
    int largest_contour_index = -1;

    for (auto index = 0; index < m_contours.size(); index++)
    {
        double area = cv::contourArea(m_contours[index]);

        if (area > 1000)
        {
            if (cv::matchShapes(m_contours[index], m_refContours[m_refContourIdx], 1, 0) < 0.1)
            {
                largest_contour_index = index;
            }
        }
    }
    if (largest_contour_index > 0)
    {
        return largest_contour_index;
    }
    return -1;
}
void ObjectFinder::initReferenceObject(std::string pathToRefImg)
{
    m_refObject = cv::imread(pathToRefImg);
    cv::Mat ref_msk = m_refObject.clone();
    cv::cvtColor(ref_msk, ref_msk, cv::COLOR_BGR2GRAY);
    ColorSegmentation::OthaSpaceThresholdingRGB(m_refObject, true, false, false, ref_msk);
    cv::findContours(ref_msk, m_refContours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    m_refContourIdx = findLargestContour(m_refContours);
    if (m_refContourIdx >= 0)
    {
    drawContours(m_refObject, m_refContours, m_refContourIdx, cv::Scalar(0, 255, 0), 2);
    }
    cv::imshow("ball", m_refObject);
    cv::waitKey(0);
}