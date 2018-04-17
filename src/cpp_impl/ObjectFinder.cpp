#include "ObjectFinder.h"
#include "ColorSegmentation.h"
#define RIGHT_MARGIN 180
#define LEFT_MARGIN 230
#define GRAB_MARGIN 290
ObjectFinder::ObjectFinder(std::string pathToRefImg):m_textColor(0, 0, 255)
{
    initReferenceObject(pathToRefImg);
}
void ObjectFinder::processImage(cv::Mat image)
{
    cv::Mat msk(image.rows, image.cols, CV_8UC1);
    bool contourFound = findObjectInFrame(image, msk);
    if (contourFound)
    {
        if (centerObject(image))
        {
            driveToObject(image);
        }
        drawFoundContour(image);
    }
}
void ObjectFinder::drawFoundContour(cv::Mat image)
{
    drawContours(image, m_contours, m_largestContourIndex, cv::Scalar(0, 255, 0), 2);
}
int ObjectFinder::findLargestContour(std::vector<std::vector<cv::Point> > contours)
{
    double largest_area = -1;
    int largest_contour_index = -1;
    for (auto index = 0; index < contours.size(); index++)
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

void ObjectFinder::initReferenceObject(std::string pathToRefImg)
{
    m_refObject = cv::imread(pathToRefImg);
    cv::Mat ref_msk = m_refObject.clone();
    cv::cvtColor(ref_msk, ref_msk, cv::COLOR_BGR2GRAY);
    cv::inRange(m_refObject, cv::Scalar(0, 0, 100), cv::Scalar(0, 0, 255), ref_msk);
    cv::findContours(ref_msk, m_refContours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    m_refContourIdx = findLargestContour(m_refContours);
    if (m_refContourIdx >= 0)
    {
        drawContours(m_refObject, m_refContours, m_refContourIdx, cv::Scalar(0, 255, 0), 2);
    }
    //cv::imshow("reference", m_refObject);
    //cv::waitKey(0);
}

bool ObjectFinder::findObjectInFrame(cv::Mat frame, cv::Mat frameMask)
{
    bool result=ColorSegmentation::OthaSpaceThresholdingRGB(frame, false, false, true, frameMask);
    cv::imshow("msk", frameMask);
    if (result)
    {
        //dilate the msk to cover white spots
        cv::dilate(frameMask, frameMask, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);

        cv::findContours(frameMask.clone(), m_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        int largest_contour_index = -1;
        for (auto index = 0; index < m_contours.size(); index++)
        {
            double area = cv::contourArea(m_contours[index]);

            if (area > 7000 && area < 20000)
            {
                /*if (cv::matchShapes(m_contours[index], m_refContours[m_refContourIdx], 1, 0) < 0.01)
                {
                    largest_contour_index = index;
                }*/
                auto perim = cv::arcLength(m_contours[index], true);
                std::vector<cv::Point> pred;
                cv::approxPolyDP(m_contours[index], pred, 0.04*perim, true);
                cv::Rect found_shape(cv::boundingRect(pred));
                float ratio = found_shape.width / static_cast<float>(found_shape.height);
                if (ratio > 0.5&&ratio < 1.2)
                {
                    cv::rectangle(frame, found_shape, cv::Scalar(0, 255, 0), 2);
                    largest_contour_index = index;
                    //std::cout << area << std::endl;
                }
            }
        }
        if (largest_contour_index > 0)
        {
            m_largestContourIndex = largest_contour_index;
            return true;
        }
    }
    return false;
}

bool ObjectFinder::centerObject(cv::Mat image)
{
    cv::Rect objRectangle = cv::boundingRect(m_contours[m_largestContourIndex]);
    int xCenter = objRectangle.x + objRectangle.width / 2;
    int yCenter = objRectangle.y + objRectangle.height / 2;
    //cv::circle(image, cv::Point(xCenter, yCenter), 5,cv::Scalar(255,0,0));
    //cv::rectangle(image, objRectangle, cv::Scalar(0, 0, 255), 2);
    if (xCenter < RIGHT_MARGIN)
    {
        cv::putText(image, "Command to left", cvPoint(30, 30),
            cv::FONT_HERSHEY_DUPLEX, 0.8, m_textColor, 1, CV_AA);
        //std::cout << "command to right" << std::endl;
    }
    else if (xCenter > LEFT_MARGIN)
    {
        cv::putText(image, "Command to right", cvPoint(30, 30),
            cv::FONT_HERSHEY_DUPLEX, 0.8, m_textColor, 1, CV_AA);
        //std::cout << "command to left" << std::endl;
    }
    else
    {
        cv::putText(image, "Object in center", cvPoint(30, 30),
            cv::FONT_HERSHEY_DUPLEX, 0.8, m_textColor, 1, CV_AA);
        //std::cout << "object is in center" << std::endl;
        return true;
    }
    return false;
}
void ObjectFinder::driveToObject(cv::Mat image)
{
    cv::Rect objRectangle = cv::boundingRect(m_contours[m_largestContourIndex]);
    int yCenter = objRectangle.y + objRectangle.height / 2;
    if (yCenter < GRAB_MARGIN)
    {
        cv::putText(image, "Drive forward", cvPoint(30, 50),
            cv::FONT_HERSHEY_DUPLEX, 0.8, m_textColor, 1, CV_AA);
        //std::cout << "drive forward" << std::endl;
    }
    else
    {
        cv::putText(image, "Grasp object", cvPoint(30, 50),
            cv::FONT_HERSHEY_DUPLEX, 0.8, m_textColor, 1, CV_AA);
        //std::cout << "grasp object" << std::endl;
    }
}
