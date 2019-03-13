#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src, src_gray;
void harris_detect();

int main( int argc, char** argv )
{
	src = imread( argv[1], 1 );
	cvtColor( src, src_gray, COLOR_BGR2GRAY );
        harris_detect();
	waitKey(0);
	return(0);
}

void harris_detect()
{
	Mat dst, dst_norm, dst_norm_scaled;
  	dst = Mat::zeros( src.size(), CV_32FC1 );
	int count = 0;
	int threshold = 126;
  	int blockSize = 2;
 	int apertureSize = 3;
 	double k = 0.04;
 	cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
 	normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  	convertScaleAbs( dst_norm, dst_norm_scaled );
 	for( int j = 0; j < dst_norm.rows ; j++ )
     	{ for( int i = 0; i < dst_norm.cols; i++ )
          {
		if((int) dst_norm.at<float>(j,i)>threshold)
		{
			printf("Coordinates : <%d,%d>  ",i,j);	
           		printf("Cornerness score : %d \n",(int) dst_norm.at<float>(j,i));
			count++;
		}
          }
        }
	printf("%d harris corners detected with %d threshold value. \n",count,threshold);
}
