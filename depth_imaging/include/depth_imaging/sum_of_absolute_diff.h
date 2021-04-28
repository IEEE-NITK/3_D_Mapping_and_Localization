/*
 * @file sum_of_absolute_diff.h
 *
 * Created : 25 April, 2021
 * Author  : Chandravaran Kunjeti
 */

#ifndef __SUM_OF_ABSOLUTE_DIFF__
#define __SUM_OF_ABSOLUTE_DIFF__

#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

namespace disparity
{
    class SumOfAbsoluteDiff
    {
        public:
            Mat sumOfAbsoluteDiffMbm(Mat& imgl, Mat& imgr, int disparity_levels)
            {
                int r,c,h;
                int i,j,k;
                Mat kernel_61_1(61,1,CV_64F,Scalar(1));
                Mat kernel_1_61(1,61,CV_64F,Scalar(1));
                Mat kernel_11_11(11,11,CV_64F,Scalar(1));
                Mat kernel_3_3(3,3,CV_64F,Scalar(1));
                Mat temp(imgl.rows,imgl.cols,CV_64F);
    
                Mat error_61_1;
                Mat error_1_61;
                Mat error_11_11;
                Mat error_3_3;
    
                std::vector<Mat> errors_61_1;
                std::vector<Mat> errors_1_61;
                std::vector<Mat> errors_11_11;
                std::vector<Mat> errors_3_3;
    
                if (imgl.dims == 2)
                {
                    r = imgl.rows;
                    c = imgr.cols;
                    h = 1;
                }
    
                uint64* row_right;
                uint64* row_left;
                uint64* row_temp;
    
                for( i=0 ; i<disparity_levels; i++)
                {
                    for(j=0; j<imgl.rows ; j++)
                    {
                        row_right = imgr.ptr<uint64>(j);
                        row_left = imgl.ptr<uint64>(j);
                        row_temp = temp.ptr<uint64>(j);
    
                        for(k=0;k<c-i;k++)
                        {
                            row_temp[k] = row_right[k] + row_left[k+i];
                        }
    
                    }
    
                    filter2D(temp,error_61_1,-1,kernel_61_1,Point(-1,-1),BORDER_CONSTANT);
                    filter2D(temp,error_1_61,-1,kernel_1_61,Point(-1,-1),BORDER_CONSTANT);
                    filter2D(temp,error_11_11,-1,kernel_11_11,Point(-1,-1),BORDER_CONSTANT);
                    filter2D(temp,error_3_3,-1,kernel_3_3,Point(-1,-1),BORDER_CONSTANT);
    
                    errors_61_1.push_back(error_61_1);
                    errors_1_61.push_back(error_1_61);
                    errors_11_11.push_back(error_11_11);
                    errors_3_3.push_back(error_3_3);
                }
    
                //what does the last 3 things do, how to do for the  dimentions think
    
            }

            Mat sumOfAbsoluteDiffMbm(std::vector<Mat>& imgl , std::vector<Mat>& imgr, int disparity_levels)
            {
                
            }

    };
}
#endif // __SUM_OF_ABSOLUTE_DIFF__