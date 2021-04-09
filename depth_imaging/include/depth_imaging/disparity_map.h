/*
 * census_transform.h
 *
 * Created : 6 April, 2021
 * Author  : Chandravaran Kunjeti
 */


#ifndef _DISPARITY_MAP_H_
#define _DISPARITY_MAP_H_

#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

namespace disparity_map
{
    class CensusTransform
    {
        public:

        Mat image;

        Mat census_convolution(Mat imgl, Mat imgr,int block_size, int disparity_levels)
        {
            Mat padded_image,output_image(imgr.rows,imgr.cols,CV_8U);
            // copyMakeBorder(imgl,padded_image,row_padding,row_padding,col_padding,col_padding,BORDER_CONSTANT,0);
            int csr[block_size*block_size];
            int csl[block_size*block_size]={0};
            uint8_t *img_datar = imgr.data;
            uint8_t *img_datal = imgl.data;
            int channels = imgr.channels();
            int r      = imgr.rows;
            int c      = imgr.cols*channels;
            int stride = imgr.step; 
            int mid    = block_size/2;
            int count  = 0;
            int count1 = 0;
            int min    = 0;
            int cmp    = 0;
            int size   = 0;

            double filter_value = block_size*block_size/10;

            for(int i=mid; i<r-mid; i++)
            {
                for(int j=mid; j<c-mid ; j++)
                {
                    count=0;
                    for(int ii=i-mid ; ii<i+mid+1 ; ii++)
                    {
                        for(int jj=j-mid ; jj<j+mid+1 ; jj++)
                        {
                            if( imgr.at<uchar>(Point(jj,ii)) < imgr.at<uchar>(Point(j,i)))
                            {
                                csr[count]=0;
                                // cout << imgr.data[ii, jj] ;
                            }
                            else 
                            {
                                csr[count]=1;
                                // cout << imgr.data[ii, jj] ;
                                // cout << csr[count] ;
                            }

                            count++;
                            // cout << count << "\n";
                        }
                        // cout <<"final count: " <<count;
                    }
                    // cout <<"final count: " <<count;

                    if((c-mid)>(j+disparity_levels))
                    {
                        min = j+disparity_levels;
                    }
                    else
                    {
                        min = c-mid;
                    }

                    
                    // cout << "CSR: "<< endl;
                    // for (size_t i = 0; i < sizeof(csr)/sizeof(csr[0]); i++) {
                    //     std::cout << csr[i] << ' ';
                    // }
                    // cout << " " << endl;


                    size = min-j;
                    // cout << "size: " << size;
                    double error[size];
                    for(int k=j; k<min; k++ )
                    {
                        count=0;
                        cmp  =0;
                        for(int ii=i-mid ; ii<i+mid+1 ; ii++)
                        {
                            for(int kk=k-mid ; kk<k+mid+1 ; kk++)
                            {
                                if( imgl.at<uchar>(Point(kk,ii)) < imgl.at<uchar>(Point(k,i)))
                                {
                                    csl[count]=0;
                                    // cout << csl[count-1] ;
                                }
                                else 
                                {
                                    csl[count]=1;
                                    // cout << csr[count-1] ;
                                }

                                if(csl[count]!=csr[count])
                                {
                                    cmp++;
                                }

                                count++;
                            }
                        }

                        // cout << "CSL: "<< endl;
                        // for (size_t i = 0; i < sizeof(csl)/sizeof(csl[0]); i++) {
                        //     std::cout << csl[i] << ' ';
                        // }
                        // cout << " " << endl;
                        
                        error[k-j] = cmp;
                    }

                    // for (size_t i = 0; i < sizeof(error)/sizeof(error[0]); i++) {
                    //         std::cout << error[i] << ' ';
                    //     }
                    // cout << " " << endl;
                    
                    output_image.at<uchar>(i,j) =(uint8_t) (((double) indexofSmallestElement(error,disparity_levels,filter_value)/(double)disparity_levels*255));
                    // cout << "Index: "<< endl;
                    // cout << indexofSmallestElement(error,disparity_levels) <<endl;
                    // cout << "Image pixel value: "<< endl;
                    // cout << (int)output_image.at<uchar>(i,j) <<endl;
                    // cout << "done with one pixel\n";                      
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
    };
}

#endif