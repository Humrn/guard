#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "MyImgProc.h"
#include "Assistant.h"
#include "宏.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
int myMin = 300;
int myMax = 60;

void main()
{
    Mat src_Image1,tmp_Image,dst_Image,mask_Image1,mask_Image2;
	src_Image1 = imread("a.jpg",0);
	threshold(src_Image1, mask_Image1, 40, 255, THRESH_BINARY_INV);
	Ptr<SURF> detector=SURF::create(myMin);
	vector<KeyPoint> trainKeyPoint1;
	
	//检测特征点
	detector->detect(src_Image1, trainKeyPoint1,mask_Image1);
	
	Ptr<SURF> extractor = SURF::create();
	Mat descriptor1;
	//计算特征向量
	extractor->compute(src_Image1, trainKeyPoint1, descriptor1);
	

	FlannBasedMatcher matcher;//实例化一个特征匹配对象
	vector<KeyPoint>  testKeyPoint;
	Mat  testDescriptor;
	vector<DMatch> matches;
	VideoCapture myCamera(0);
	myCamera.set(CV_CAP_PROP_FRAME_WIDTH, 300);
	myCamera.set(CV_CAP_PROP_FRAME_HEIGHT, 300);
	while (true)
	{
		double startTime = getTickCount();
		myCamera >> tmp_Image;
		cvtColor(tmp_Image, tmp_Image, CV_RGB2GRAY);//转化为灰度图
		threshold(tmp_Image, mask_Image2, 40, 255, THRESH_BINARY_INV);//获取掩膜
		
		if(tmp_Image.data==NULL)
			continue;
		detector->detect(tmp_Image, testKeyPoint,mask_Image2);
		extractor->compute(tmp_Image,testKeyPoint, testDescriptor);
		if (testDescriptor.empty())
			continue;
		matcher.match(descriptor1, testDescriptor, matches);
		/*matcher.match(descriptor2, testDescriptor, matches[1]);
		matcher.match(descriptor3, testDescriptor, matches[2]);
		matcher.match(descriptor4, testDescriptor, matches[3]);*/
		//就行轮廓检测
		vector<vector<Point>> contours;
		vector<Vec4i>         hierarchy;
		//findContours(mask_Image2, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		if (matches.size() != 0 && testKeyPoint.size() != 0 && trainKeyPoint1.size() != 0)//存在匹配点
		{
			drawMatches(src_Image1, trainKeyPoint1, tmp_Image, testKeyPoint, matches, dst_Image);
			vector<KeyPoint> googKeyPoint;
			vector<Point2f>  googPoint;
			float minDistance = 100, maxDistance = 0;
			int   bestIndex=0;
			for (int i = 0; i < matches.size(); i++)
			{
				if (matches[0].distance < minDistance) {
					minDistance = matches[i].distance;
					bestIndex = i;
				}
				if (matches[0].distance > maxDistance)
					maxDistance = matches[i].distance;
			}
			cout << "最小匹配距离：" << minDistance << endl;
			cout << "最大匹配距离：" << maxDistance << endl;
			cout << "行：" << mask_Image2.rows << "  列：" << mask_Image2.cols << endl;
			googKeyPoint.push_back(testKeyPoint[matches[bestIndex].trainIdx]);
			/*int n = 0;
			for (int j = 0; j < matches.size(); j++)
			{
				if (matches[j].distance <= minDistance)
				{
					KeyPoint tmpPoint = testKeyPoint[matches[j].trainIdx];
					googKeyPoint.push_back(tmpPoint);
					googPoint.push_back(googKeyPoint[n].pt);
					n++;
				}
			}*/
			drawKeypoints(tmp_Image, googKeyPoint, tmp_Image);

			MyImgProc myFactory;
			myFactory.setBinaryImg(mask_Image2);
			vector<Point>  targetContours;
			Point          targetPoint = testKeyPoint[matches[bestIndex].trainIdx].pt;
			//myFactory.getBlockPoints(targetPoint.x, targetPoint.y, targetContours);
			
			
		}
		imshow("原图显示", tmp_Image);
		imshow("a的匹配图", dst_Image);
		imshow("掩膜图train", mask_Image1);
		imshow("掩膜图test", mask_Image2);

		cout << "帧率：" << getTickFrequency() / (getTickCount() - startTime) << endl;
		int returnKey=waitKey(1);
		if (returnKey == 27)
			break;
	}
}