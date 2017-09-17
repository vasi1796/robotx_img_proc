#pragma once
#include <opencv2/opencv.hpp>
class ColorSegmentation
{
public:
	ColorSegmentation();
	static bool OthaSpaceThresholdingRGB(const cv::Mat& src, bool bSegmentRed, bool bSegmentBlue, bool bSegmentYellow, cv::Mat& dstMask, float dThR1 = 0.024, float dThR2 = -0.027, float dThB1 = -0.04, float dThB2 = 0.2, float dThY1 = 0.071, float dThY2 = 0.027, unsigned int nThL = 100, unsigned int nRGDif = 15, unsigned int nGBDif = 25, unsigned int nBRDif = 14);
	//static bool OthaSpaceThresholdingRGB(const cv::Mat& src, bool bSegmentRed, bool bSegmentBlue, bool bSegmentYellow, cv::Mat& dstMask, float dThR1 /*= 0.024*/, float dThR2 /*= -0.027*/, float dThB1 /*= -0.04*/, float dThB2 /*= 0.2*/, float dThY1 /*= 0.071*/, float dThY2 /*= 0.027*/, unsigned int nThL /*= 100*/, unsigned int nRGDif /*= 15*/, unsigned int nGBDif /*= 25*/, unsigned int nBRDif /*= 14*/);
};