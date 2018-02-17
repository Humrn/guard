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
	//��Ա����
	Mat mSrc_img;
	Mat binary_img;
	//���캯��
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
	//����ͼ������ؾ�ֵ
	float Avetage()
	{
		if (mSrc_img.empty())
		{
			cout << "error:λ��MyImgProc::Avetage,�������ֵ��Դͼû�����ݣ����ʼ��mSrc_img��Ա����" << endl;
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
		binary_img.at<uchar>(y, x) = 100;//������ص�Ϊ�Ѿ����������ص�
		if (x - 1 >= 0 && binary_img.at<uchar>(y, x-1) != 100)//��߻�������
		{
			if (binary_img.at<uchar>(y, x - 1) == 0)//��ɫ��߽��
			{
				edgePoints.push_back(Point(x - 1,y ));
			}
			else//����ɫ��ı߽��,�������
			{
				getBlockPoints(x - 1, y, edgePoints);
			}
		}
		if (x + 1 <binary_img.cols && binary_img.at<uchar>(y, x+1) != 100)//�ұ߱߻�������
		{
			if (binary_img.at<uchar>(y,x + 1) == 0)//��ɫ��߽��
			{
				edgePoints.push_back(Point(x + 1, y));
			}
			else//����ɫ��ı߽��,�������
			{
				getBlockPoints(x + 1, y, edgePoints);
			}
		}
		if (y - 1 >= 0 && binary_img.at<uchar>(y-1, x) != 100)//�ϱ߻�������
		{
			if (binary_img.at<uchar>(y - 1, x) == 0)//��ɫ��߽��
			{
				edgePoints.push_back(Point(x, y - 1));
			}
			else//����ɫ��ı߽��,�������
			{
				getBlockPoints(x, y - 1, edgePoints);
			}
		}
		if (y+1  < binary_img.rows && binary_img.at<uchar>( y + 1,x) != 100)//�±߻�������
		{
			if (binary_img.at<uchar>(y+1,x ) == 0)//��ɫ��߽��
			{
				edgePoints.push_back(Point(x , y+ 1));
			}
			else//����ɫ��ı߽��,�������
			{
				getBlockPoints(x, y+1, edgePoints);
			}
		}

	}
};

#endif // !MYIMGPROC_H