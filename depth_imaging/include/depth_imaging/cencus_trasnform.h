/*
 * @file cencus_transform.h
 *
 * Created : 25 April, 2021
 * Author  : Chandravaran Kunjeti
 */

#ifndef __CENCUS_TRANSFORM__
#define __CENCUS_TRANSFORM__

#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

namespace disparity
{
    class CencusTransform
    {
        public:
    
            Mat image;
    
            Mat census_convolution(Mat imgl, Mat imgr,int block_size, int disparity_levels)
            {
            
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
    
                uchar* row_right_outer;
                uchar* row_right_inner;
                uchar* row_left_outer;
                uchar* row_left_inner;
                uchar* row_output;
    
                double filter_value = block_size*block_size/10;
    
                for(i=mid; i<r-mid; i++)
                {
                    row_right_outer = imgr.ptr<uchar>(i);
                    row_left_outer  = imgl.ptr<uchar>(i);
                    row_output= output_image.ptr<uchar>(i);
                    for(j=mid; j<c-mid ; j++)
                    {
                        count=0;
                        for(ii=i-mid ; ii<i+mid+1 ; ii++)
                        {
                            // One mistake done previously is that we did not change the pointer value when it was in the inner loop
                            row_right_inner = imgr.ptr<uchar>(ii);
                            for(jj=j-mid ; jj<j+mid+1 ; jj++)
                            {
                                if( row_right_inner[jj] < row_right_outer[j])
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
                        for( k=j+1; k<min; k++ )
                        {
                            count=0;
                            cmp  =0;
                            for( ii=i-mid ; ii<i+mid+1 ; ii++)
                            {
                                row_left_inner  = imgl.ptr<uchar>(ii);
                                for( kk=k-mid ; kk<k+mid+1 ; kk++)
                                {
                                    if( row_left_inner[kk] < row_left_outer[k])
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
    
                if(mini>filter_value)
                {
                    idx = 0;
                }
    
                return idx;        
            }

            Mat censusTransformMbm(std::vector<Mat> imgl, std::vector<Mat> imgr )
            {
                // The differnt kernel to be used for multi block matching do not kee
                int kernel[4][2] = {{1,61},{61,1},{11,11},{3,3}}; 

                for(int i=0; i<4; i++)
                {


                }

            }

            Mat censusConvolutionVector(Mat imgl, Mat imgr, int kernel_hight, int kernel_width, int disparity_levels)
            {
                int row_padding = kernel_hight/2;
                int col_padding = kernel_width/2;
                int r = imgl.rows;
                int c = imgl.cols; 
                int i;
                int j;
                int ii;
                int jj;
                Scalar value;
                Mat img_borderl;
                Mat img_borderr;
                Mat output_imagel(imgl.rows,imgl.cols,CV_64F);
                Mat output_imager(imgr.rows,imgr.cols,CV_64F);
                copyMakeBorder(imgl,img_borderl,row_padding,row_padding,col_padding,col_padding,BORDER_CONSTANT,value);
                copyMakeBorder(imgr,img_borderr,row_padding,row_padding,col_padding,col_padding,BORDER_CONSTANT,value);

                uint64* row_right;
                uint64* row_left;
                uint64* row_left_border;
                uint64* row_right_border;
                uint64* row_output_right;
                uint64* row_output_left;
                

                for(i=0; i<kernel_hight; i++)
                {
                    for(j=0; j<kernel_width; j++)
                    {
                        for(ii=i; ii<r+i ; ii++)
                        {
                            row_right = imgr.ptr<uint64>(ii);
                            row_left  = imgl.ptr<uint64>(ii);
                            row_right_border = img_borderr.ptr<uint64>(ii);
                            row_left_border  = img_borderl.ptr<uint64>(ii);
                            row_output_right = output_imagel.ptr<uint64>(ii);
                            row_output_left  = output_imager.ptr<uint64>(ii);
                            for(jj=j; jj<c+j; jj++)
                            {
                                row_output_right[jj] = (row_output_right[jj] << 1) | (row_right_border[jj] >= row_right[jj]);
                                row_output_left[jj]  = (row_output_left[jj]  << 1) | (row_left_border[jj]  >= row_left[jj]);
                            }
                        }
                    }
                }

                return output_imagel;


            }

    };

}

#endif // __CENCUS_TRANSFORM__