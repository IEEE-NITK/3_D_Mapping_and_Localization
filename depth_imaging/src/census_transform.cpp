#include <stdio.h>
#include <math.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>

// #include "depth_imaging/disparity_map.h"

using namespace cv;
using namespace std;

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
            int r      = imgr.rows;
            int c      = imgr.cols;
            int stride = imgr.step; 
            int mid    = block_size/2;
            int count  = 0;
            int count1 = 0;
            int min    = 0;
            int cmp    = 0;
            int size   = 0;

            for(int i=mid; i<r-mid; i++)
            {
                for(int j=mid; j<c-mid ; j++)
                {
                    count=0;
                    for(int ii=i-mid ; ii<i+mid+1 ; ii++)
                    {
                        for(int jj=j-mid ; jj<j+mid+1 ; jj++)
                        {
<<<<<<< HEAD
                            if( imgr.at<uchar>(ii, jj) < imgr.at<uchar>(i,j))
=======
                            if( imgr.at<uchar>(Point(jj, ii)) < imgr.at<uchar>(Point(j, i)))
>>>>>>> 5cee8285ebaf71a77a19eb070c7a697307f57d39
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
                                if( imgl.at<uchar>(ii , kk) < imgl.at<uchar>(i,k))
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
                    
                    output_image.at<uchar>(i,j) =(uint8_t) (indexofSmallestElement(error,disparity_levels));
                    // cout << "Index: "<< endl;
                    // cout << indexofSmallestElement(error,disparity_levels) <<endl;
                    // cout << "Image pixel value: "<< endl;
                    // cout << (int)output_image.at<uchar>(i,j) <<endl;
                    // cout << "done with one pixel\n";                      
                }
                
            }
            return output_image;
        }

        int indexofSmallestElement(double array[], int size)
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

            return idx;
        }
    };

int main()
{
    Mat imgl,imgr,disp_img;
    int block_size=7;
    int disparity_levels=32;
    CensusTransform CT;

    // imgl = imread("/home/ubuntu1804/eyantra_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im2.png" , IMREAD_GRAYSCALE ); // Load an image
    imgl = imread("/home/chandravaran/catkin_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im2.png" , IMREAD_GRAYSCALE ); // Load an image
    // imgr = imread("/home/ubuntu1804/eyantra_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im6.png", IMREAD_GRAYSCALE ); // Load an image
    imgr = imread("/home/chandravaran/catkin_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im6.png", IMREAD_GRAYSCALE ); // Load an image

    imshow("left image",imgl);
    imshow("right image",imgr);
    // waitKey();
    disp_img = CT.census_convolution(imgl,imgr,block_size,disparity_levels);
    
    imshow("disp image",disp_img);
    waitKey();
    return 0;
}
