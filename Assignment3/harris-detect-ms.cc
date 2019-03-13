#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat levels[5];
Mat grays[5];
void harris_detect_ms();
void harris_detect(Mat img,Mat img_gray,int lv);

int main( int argc, char** argv )
{
	levels[0] = imread( argv[1], 1 );
	cvtColor( levels[0], grays[0], COLOR_BGR2GRAY );
        harris_detect_ms();
	waitKey(0);
	return(0);
}

void harris_detect_ms()
{
	for(int i=0;i<4;i++)
	{
		pyrDown(levels[i],levels[i+1], Size(levels[i].cols/2,levels[i].rows/2)); 
		cvtColor( levels[i], grays[i], COLOR_BGR2GRAY );	
	}
	cvtColor( levels[4], grays[4], COLOR_BGR2GRAY );
	for(int i=0;i<5;i++)
	{
		harris_detect(levels[i],grays[i],i);
	}
}

void harris_detect(Mat img,Mat img_gray,int lv)
{
	Mat dst, dst_norm, dst_norm_scaled;
  	dst = Mat::zeros( img.size(), CV_32FC1 );
	int count = 0;
	int threshold = 126;
  	int blockSize = 2;
 	int apertureSize = 3;
 	double k = 0.04;
 	cornerHarris( img_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
 	normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  	convertScaleAbs( dst_norm, dst_norm_scaled );
 	for( int j = 0; j < dst_norm.rows ; j++ )
     	{ for( int i = 0; i < dst_norm.cols; i++ )
          {
		if((int) dst_norm.at<float>(j,i)>threshold)
		{
			printf("Coordinates : <%d,%d>  ",i,j);	
           		printf("Cornesness score : %d ",(int) dst_norm.at<float>(j,i));
			printf("Pyramid level : %d \n",lv);
			count++;
		}
          }
        }
	printf("%d harris corners detected with %d threshold value at level %d. \n",count,threshold,lv);
}
