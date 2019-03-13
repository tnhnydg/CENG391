#include <cstdlib>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

Mat levels[5];
Mat grays[5];
void harris_detect_ms();
void harris_detect(Mat img,Mat img_gray,int lv);
Mat ComputeDescriptors(const Mat &image, vector<KeyPoint> &keyPoints);

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
	Mat dst, dsc, dst_norm, dst_norm_scaled;
  	dst = Mat::zeros( img.size(), CV_32FC1 );
	vector<KeyPoint> keypoints;
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
			KeyPoint kp((float)i,(float)j,/*sizeof(KeyPoint)*/CV_8U);
			keypoints.push_back(kp);
		}
          }
        }
	cout << "Before : " << keypoints.size() << endl;
	dsc = ComputeDescriptors(img_gray,keypoints); //Here keypoint vector size decreases and I couldn't find out why this happens. Since the size decreases, at runtime execution is being aborted.
	cout << "After : " << keypoints.size() << endl;
 	for( int j = 0; j < dst_norm.rows ; j++ )
     	{ for( int i = 0; i < dst_norm.cols; i++ )
          {
		if((int) dst_norm.at<float>(j,i)>threshold)
		{
			printf("Coordinates : <%d,%d>  ",i,j);	
           		printf("Cornesness score : %d ",(int) dst_norm.at<float>(j,i));
			printf("Pyramid level : %d ",lv);
			cout << "BDB : " << dsc.row(count) << endl; //Brief Descriptor Bytes
			count++;
		}
          }
        }
	printf("%d harris corners detected with %d threshold value at level %d. \n",count,threshold,lv);
}

Mat ComputeDescriptors(const Mat &image, vector<KeyPoint> &keyPoints)
{
    Ptr<BriefDescriptorExtractor> featureExtractor = BriefDescriptorExtractor::create(32);
    Mat descriptors;
    featureExtractor->compute(image, keyPoints, descriptors);//Here keypoint vector size decreases
    return descriptors;
}
