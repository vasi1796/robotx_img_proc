#include "ColorSegmentation.h"
ColorSegmentation::ColorSegmentation()
{
}
bool ColorSegmentation::OthaSpaceThresholdingRGB(const cv::Mat& src, bool bSegmentRed, bool bSegmentBlue, bool bSegmentYellow, cv::Mat& dstMask, float dThR1 /*= 0.024*/, float dThR2 /*= -0.027*/, float dThB1 /*= -0.04*/, float dThB2 /*= 0.2*/, float dThY1 /*= 0.071*/, float dThY2 /*= 0.027*/, unsigned int nThL /*= 100*/, unsigned int nRGDif /*= 15*/, unsigned int nGBDif /*= 25*/, unsigned int nBRDif /*= 14*/)
 {
     if (src.channels() != 3)
     {
         std::cout << "OthaSpaceThresholding error! Source image must be a 3 channel image (color image)." << std::endl;
         return false;
     }

     if (dstMask.empty())
     {
         std::cout << "OthaSpaceThresholding error! Uninitialized destination mask image." << std::endl;
         return false;
     }

     // check validity of the selected color 
     if (bSegmentRed == false && bSegmentBlue == false && bSegmentYellow == false)
     {
         std::cout << "OthaSpaceThresholding error! No color selected for segmentation." << std::endl;
         return false;
     }

	 // normalized components;
	 float P1 = .0, P2 = .0;

	 int R = 0, G = 0, B = 0, I = 0, RG = 0, GB = 0, BR = 0;
     unsigned int width = src.cols, height = src.rows, row = 0, col = 0;

     for (row = 0; row < height; ++row)
     {
         const uchar* p = src.ptr(row);
         uchar* c = dstMask.ptr(row);
         
         for (col = 0; col < width; ++col)
//[[24]]
		 {            
             // read colors in order: RGB
             R = static_cast<int>(*p);
             p++;
             G = static_cast<int>(*p);
             p++;
             B = static_cast<int>(*p);
             p++;

             // compute illumination param
             I = R + G + B;

             // avoid achromatic pixels: STAGE I
             if (I < nThL)
             {
                 *c = 0;
                 c++;
                 continue;
             }

             // compute RGB differences
             RG = abs(R - G);
             GB = abs(G - B);
             BR = abs(B - R);

             // avoid achromatic pixels: STAGE II
             if (RG <= nRGDif/*15*/ &&  GB <= nGBDif/*25*/ && BR <= nBRDif/*14*/)   
             {
                 *c = 0;
                 c++;
                 continue;
             }

             // compute parameters
             //         1         R - G
             // P1 = ------- * -----------
             //      sqrt(2)    R + G + B
			 // notation 0.70710678 = (1/(sqrt(2.)))

             P1 = 0.70710678 * (((float)(R - G)) / (float)I) ; 

             //         1       2 * G - B - R
             // P1 = ------- * --------------
             //      sqrt(6)      R + G + B
			 // notation 0.40824829 = (1/(sqrt(6.)))

             P2 = 0.40824829 * (((float)((2 * G - B - R))) / (float)I); 

             // threshold the image accordingly to a series of parameters
             if (bSegmentRed)
             {
				 ((P1 >= dThR1) && (P2 <= dThR2)) ? *c |= 255 : *c &= 0;
				 c++;
             }

             if (bSegmentBlue)
             {
                 ((P1 <= dThB1) && (std::fabs(P2)<= dThB2)) ? *c |= 255 : *c &= 0;
                 c++;
             }

             if (bSegmentYellow)
             {
                 ((P1 >= dThY1) && (std::fabs(P2)<= dThY2)) ? *c |= 255 : *c &= 0;
                 c++;
             }
         }  
     }

	 return true;
}
