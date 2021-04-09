#include <stdio.h>
#include <math.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <chrono>
#include <stdio.h>
#include <iostream>

#include "depth_imaging/disparity_map.h"
#include "depth_imaging/backup.h"

using namespace cv;
using namespace std;
using namespace disparity_map;

int main()
{
    Mat imgl,imgr,disp_img,disp_img1;
    int block_size=7;
    int disparity_levels=64;
    CensusTransform CT;
    CensusTransform1 CT1;

    // imgl = imread("/home/ubuntu1804/eyantra_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im2.png" , IMREAD_GRAYSCALE ); // Load an image
    imgl = imread("/home/chandravaran/catkin_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im2.png" , IMREAD_GRAYSCALE ); // Load an image
    // imgl = imread("/home/chandravaran/catkin_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/left_image.png" , IMREAD_GRAYSCALE ); // Load an image


    // imgr = imread("/home/ubuntu1804/eyantra_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im6.png", IMREAD_GRAYSCALE ); // Load an image
    imgr = imread("/home/chandravaran/catkin_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/im6.png", IMREAD_GRAYSCALE ); // Load an image
    // imgr = imread("/home/chandravaran/catkin_ws/src/3_D_Mapping_and_Localization/depth_imaging/src/right_image.png", IMREAD_GRAYSCALE ); // Load an image

    imshow("left image",imgl);
    imshow("right image",imgr);
    // waitKey();
    auto start = std::chrono::high_resolution_clock::now();
    // disp_img = CT.census_convolution(imgl,imgr,block_size,disparity_levels);
    disp_img1 = CT1.census_convolution(imgl,imgr,block_size,disparity_levels);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    cout << "Time taken in Microseconds: " << microseconds <<endl;
    
    // imshow("disp image",disp_img);
    imshow("disp image 1",disp_img1);
    waitKey();
    return 0;
}
