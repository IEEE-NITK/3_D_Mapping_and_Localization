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

        Mat census_convolution(Mat imgl, Mat imgr, int kernel_size[2],int block_size, int disparity_levels)
        {
            Mat padded_image,output_image(imgr.rows,imgr.cols,CV_8U);
            int row_padding,col_padding,bits=0;
            row_padding = kernel_size[0]/2;
            col_padding = kernel_size[1]/2;
            // copyMakeBorder(imgl,padded_image,row_padding,row_padding,col_padding,col_padding,BORDER_CONSTANT,0);
            int csr[kernel_size[0]*kernel_size[1]-1]={0};
            int csl[kernel_size[0]*kernel_size[1]-1]={0};
            uint8_t *img_datar = imgr.data;
            uint8_t *img_datal = imgl.data;
            int r      = imgr.rows;
            int c      = imgr.cols;
            int stride = imgr.step; 
            int mid    = block_size/2;
            int count  = 0;
            int count1 = 0;
            int min    = 0;
            int cmp    = 0;
            int size   = 0;
            int ii;
            int jj;
            int kk;
            int i;
            int j;
            int k;

            for(i=mid; i<r-mid; i++)
            {
                for(j=mid; j<c-mid ; j++)
                {
                    count=0;
                    cout << "i: \n" << i;
                    cout << "j: \n" << j;
                    for(ii=i-mid ; ii<i+mid+1 ; ii++)
                    {
                        for( jj=j-mid ; jj<j+mid+1 ; jj++)
                        {
                            if( img_datar[ii*stride + jj] < img_datar[i*stride+j])
                            {
                                csr[count]=0;
                                // cout << csr[count] ;
                            }
                            else 
                            {
                                csr[count]=1;
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

                    size = j-min;
                    cout << "size: " << size;
                    double error[size];
                    count1=0;
                    for( k=j; k<min; k++ )
                    {
                        count=0;
                        cmp  =0;
                        for( ii=i-mid ; ii<i+mid+1 ; ii++)
                        {
                            for( kk=k-mid ; kk<k+mid+1 ; kk++)
                            {
                                if( img_datal[ii*stride + kk] < img_datal[i*stride+k])
                                {
                                    csl[count++]=0;
                                    // cout << csl[count-1] ;
                                }
                                else 
                                {
                                    csl[count++]=1;
                                    // cout << csr[count-1] ;
                                }

                                if(csl[count]!=csr[count])
                                {
                                    cmp++;
                                }
                            }
                        }

                        error[count1++] = cmp;
                    }
 
                    output_image.data[i,j] = (uint8_t) indexofSmallestElement(error,size);  
                    cout << "done with one pixel\n";                      
                }
            }
            return output_image;
        }

        int indexofSmallestElement(double array[], int size)
        {
            int index = 0;

            for(int i = 1; i < size; i++)
            {
                if(array[i] < array[index])
                    index = i;              
            }

            return index;
        }


    };
}

#endif