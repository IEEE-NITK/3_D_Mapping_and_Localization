/*
 * @file sum_of_absolute_diff.h
 *
 * Created : 25 April, 2021
 * Author  : Chandravaran Kunjeti
 */

#ifndef __TOTAL_MATCHING__
#define __TOTAL_MATCHING__

#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "depth_imaging/cencus_trasnform.h"
#include "depth_imaging/sum_of_absolute_diff.h"

using namespace cv;
using namespace std;

namespace disparity
{
    class TotalMatching 
    {
        public:
            Mat totalMatchingCostGray(Mat imgl, Mat imgr)
            {
                int disparity_levels=16;
                Mat img_sad;    // output image for sum of absolute difference 
                Mat img_cencus; // output image for cencus transform 
                Mat imgLx1(imgl.rows,imgl.cols,CV_64F);
                Mat imgLy1(imgl.rows,imgl.cols,CV_64F);
                Mat imgRx1(imgr.rows,imgr.cols,CV_64F);
                Mat imgRy1(imgr.rows,imgr.cols,CV_64F);

                std::vector<Mat> imgLxy(3);
                std::vector<Mat> imgRxy(3);
                std::vector<Mat> cencus_mbm;
                std::vector<Mat> sad_gray;
                std::vector<Mat> sad_multi;

                

                // Finding the X and Y gradient images for left 
                Sobel(imgl,imgLx1,CV_64F,1,0,3);
                Sobel(imgl,imgLy1,CV_64F,0,1,3);

                // Finding the X and Y gradient images for right 
                Sobel(imgr,imgRx1,CV_64F,1,0,3);
                Sobel(imgr,imgRy1,CV_64F,0,1,3);

                // New multi dimensional left image
                imgLxy[0] = imgl;
                imgLxy[1] = imgLx1;
                imgLxy[2] = imgLy1;

                // New multi dimensional right image
                imgRxy[0] = imgr;
                imgRxy[1] = imgRx1;
                imgRxy[2] = imgRy1;

                // Finding errors using census transform by combining color and gradient and forming a 3d image 
                cencus_mbm = cencus.censusTransformMbm(imgLxy,imgRxy);
                cout << "Finished Cencus" << endl;

                // Finding errors using SAD in color space 
                sad_gray = sad.sumOfAbsoluteDiffMbm(imgl,imgr,disparity_levels);
                cout << "Finished SAD in gray space" << endl;

                // Finding errors using SAD for gradient images
                // This function is overloaded
                sad_multi = sad.sumOfAbsoluteDiffMbm(imgLxy,imgRxy,disparity_levels);
                cout << "Finished SAD in gradient space" << endl;

                
            }
        private:
            CencusTransform cencus;
            SumOfAbsoluteDiff sad;


            
    };
}
#endif //__TOTAL_MATCHING__