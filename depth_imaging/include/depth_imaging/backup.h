/*
 * census_transform.h
 *
 * Created : 6 April, 2021
 * Author  : Chandravaran Kunjeti
 */


#ifndef _DISPARITY1_MAP_H_
#define _DISPARITY1_MAP_H_

#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

namespace disparity_map
{
    class CensusTransform1
    {
        public:

        Mat image;

        Mat census_convolution(Mat imgl, Mat imgr,int block_size, int disparity_levels)
        {
            CV_Assert(imgr.depth() == CV_8U);

            Mat output_image(imgr.rows,imgr.cols,CV_8U);
            int csr[block_size*block_size];
            int csl[block_size*block_size];
            int channels = imgr.channels();
            int r      = imgr.rows;
            int c      = imgr.cols*channels;
            int mid    = block_size/2;
            int count  = 0;
            int count1 = 0;
            int min    = 0;
            int cmp    = 0;
            int size   = 0;
            int i      = 0; 
            int j      = 0; 
            int k      = 0; 
            int ii     = 0; 
            int jj     = 0; 
            int kk     = 0; 

            uchar* row_right;
            uchar* row_left;
            uchar* row_output;

            // if (imgr.isContinuous())
            // {
            //     c *= r;
            //     r = 1;
            // }

            double filter_value = block_size*block_size/10;

            for(i=mid; i<r-mid; i++)
            {
                row_right = imgr.ptr<uchar>(i);
                row_left  = imgl.ptr<uchar>(i);
                row_output= output_image.ptr<uchar>(i);
                for(j=mid; j<c-mid ; j++)
                {
                    count=0;
                    for(ii=i-mid ; ii<i+mid+1 ; ii++)
                    {
                        for(jj=j-mid ; jj<j+mid+1 ; jj++)
                        {
                            if( row_right[jj] < row_right[j])
                            {
                                csr[count]=0;
                            }
                            else 
                            {
                                csr[count]=1;
                            }

                            count++;
                        }
                    }

                    if((c-mid)>(j+disparity_levels))
                    {
                        min = j+disparity_levels;
                    }
                    else
                    {
                        min = c-mid;
                    }

                    size = min-j;
                    double error[size];
                    for( k=j; k<min; k++ )
                    {
                        count=0;
                        cmp  =0;
                        for( ii=i-mid ; ii<i+mid+1 ; ii++)
                        {
                            for( kk=k-mid ; kk<k+mid+1 ; kk++)
                            {
                                if( row_left[kk] < row_left[k])
                                {
                                    csl[count]=0;
                                }
                                else 
                                {
                                    csl[count]=1;
                                }

                                if(csl[count]!=csr[count])
                                {
                                    cmp++;
                                }

                                count++;
                            }
                        }

                        error[k-j] = cmp;
                    }
                    
                    row_output[j] =(uchar) (((double) indexofSmallestElement(error,disparity_levels,filter_value)/(double)disparity_levels*255));                 
                }
                
            }
            return output_image;
        }

        int indexofSmallestElement(double array[], int size, double filter_value)
        {
            int mini = array[0], idx = 0;

            for(int i = 1; i < size; i++)
            {
                if(array[i] < mini)
                {
                    idx = i;
                    mini = array[i];
                }        
                
            }

            // if(mini>filter_value)
            // {
            //     idx = 0;
            // }

            return idx;        
        }
    };
}

#endif