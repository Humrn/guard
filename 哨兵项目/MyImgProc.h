#ifndef MYIMGPROC_H
#define MYIMGPROC_H


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
class MyImgProc {
public:
	//成员数据
	Mat mSrc_img;
	Mat binary_img;
	//构造函数
	MyImgProc(Mat src)
	{
		src.copyTo(mSrc_img);
	}
	MyImgProc() {}
	void MyCreat(Mat src)
	{
		src.copyTo(mSrc_img);
	}
	
	void setBinaryImg(Mat img)
	{
		binary_img = img;
	}
	//返回图像的像素均值
	float Avetage()
	{
		if (mSrc_img.empty())
		{
			cout << "error:位置MyImgProc::Avetage,用于求均值的源图没有数据，请初始化mSrc_img成员变量" << endl;
			return -1;
		}
		float avetage=0;
		int rows = mSrc_img.rows;
		int cols = mSrc_img.cols*mSrc_img.channels();
		long int pixel = rows*cols;
		for (int i = 0; i < rows; i++)
		{
			uchar * data = mSrc_img.ptr<uchar>(i);
			for (int j = 0; j < cols;j++)
			{
				avetage += (float)data[j] / pixel;
			}
		}
		return avetage;
	}
	void getBlockPoints(const int x,const int y,vector<Point>& edgePoints)
	{
		binary_img.at<uchar>(y, x) = 100;//标记像素点为已经遍历的像素点
		if (x - 1 >= 0 && binary_img.at<uchar>(y, x-1) != 100)//左边还有像素
		{
			if (binary_img.at<uchar>(y, x - 1) == 0)//是色块边界点
			{
				edgePoints.push_back(Point(x - 1,y ));
			}
			else//不是色块的边界点,继续检测
			{
				getBlockPoints(x - 1, y, edgePoints);
			}
		}
		if (x + 1 <binary_img.cols && binary_img.at<uchar>(y, x+1) != 100)//右边边还有像素
		{
			if (binary_img.at<uchar>(y,x + 1) == 0)//是色块边界点
			{
				edgePoints.push_back(Point(x + 1, y));
			}
			else//不是色块的边界点,继续检测
			{
				getBlockPoints(x + 1, y, edgePoints);
			}
		}
		if (y - 1 >= 0 && binary_img.at<uchar>(y-1, x) != 100)//上边还有像素
		{
			if (binary_img.at<uchar>(y - 1, x) == 0)//是色块边界点
			{
				edgePoints.push_back(Point(x, y - 1));
			}
			else//不是色块的边界点,继续检测
			{
				getBlockPoints(x, y - 1, edgePoints);
			}
		}
		if (y+1  < binary_img.rows && binary_img.at<uchar>( y + 1,x) != 100)//下边还有像素
		{
			if (binary_img.at<uchar>(y+1,x ) == 0)//是色块边界点
			{
				edgePoints.push_back(Point(x , y+ 1));
			}
			else//不是色块的边界点,继续检测
			{
				getBlockPoints(x, y+1, edgePoints);
			}
		}

	}
};

#endif // !MYIMGPROC_H