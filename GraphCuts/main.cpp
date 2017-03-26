#include "Image.h"
#include "ImageException.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv/cvaux.h>
#include "max_flow.h"
#include "ImageSegmentation.h"


cv::Mat fg_labeledImg;
cv::Mat bg_labeledImg;
cv::Mat originalImg;



void MaxFlowTest() {
	cout << "Testing max flow alogrithm..." << endl;
	MaxFlow mf(4);
	mf.add_edge(mf.s(), 0, 3);
	mf.add_edge(mf.s(), 1, 3);
	mf.add_edge(0, 1, 2);
	mf.add_edge(0, 2, 3);
	mf.add_edge(1, 3, 2);
	mf.add_edge(2, 3, 4);
	mf.add_edge(2, mf.t(), 2);
	mf.add_edge(3, mf.t(), 3);
	mf.display();
	cout << "Max flow : " << mf.max_flow() << endl;
	vector<int> comp1 = mf.source_component();
	vector<int> comp2 = mf.sink_component();
	for (int i = 0; i < comp1.size(); ++i) {
		cout << comp1[i] << " is in the source component" << endl;
	}
	for (int i = 0; i < comp2.size(); ++i) {
		cout << comp2[i] << " is in the sink component" << endl;
	}
	mf.display();
	cout << "Begin Update Edges:" << endl;
	mf.update_edge(1, 3, 1);
	mf.update_edge(3, mf.t(), 1);
	cout << "New Max Flow:" << mf.max_flow() << endl;
	mf.display();
	cout << "Max FLow Test Ended." << endl;
	cout << endl;
}


int test() {
	cout << "Begin Test" << endl;

	ImageSegmentation seg("Input/test_pic.png");
	vector<Point> fg, bg;

	//提取前景、背景标记点
	char* fg_labeledImage = "Input/fgtest_pic.png";
	char* bg_labeledImage = "Input/bgtest_pic.png";
	fg_labeledImg = cv::imread(fg_labeledImage, 1);
	bg_labeledImg = cv::imread(bg_labeledImage, 1);
	if (!fg_labeledImg.data)
	{
		cout << "Could not open or find the labeled fore ground image: " << *fg_labeledImage << endl;
		return -1;
	}

	if (!bg_labeledImg.data)
	{
		cout << "Could not open or find the labeled image: " << *bg_labeledImage << endl;
		return -1;
	}
	int height = fg_labeledImg.rows;
	int length = fg_labeledImg.cols;
	cout << "Height: " << height << endl;
	cout << "Length: " << length << endl;

	int maxsize = length * height;
	int ** fg_pos;
	int ** bg_pos;
	fg_pos = new int*[maxsize];
	bg_pos = new int*[maxsize];

	for (int j = 0; j<maxsize; j++)
	{
		fg_pos[j] = new int[3];
	}
	//初始化数组元素
	for (int i = 0; i<maxsize; i++)
	{
		for (int j = 0; j<2; j++)
		{
			fg_pos[i][j] = 0;
		}
		fg_pos[i][2] = 1;
	}

	for (int j = 0; j<maxsize; j++)
	{
		bg_pos[j] = new int[3];
	}
	//初始化数组元素
	for (int i = 0; i<maxsize; i++)
	{
		for (int j = 0; j<2; j++)
		{
			bg_pos[i][j] = 0;
		}
		bg_pos[i][2] = 1;
	}

	int count1 = 0;
	//遍历foreground labeled image	
	for (int y = 0; y < fg_labeledImg.rows; y++)
	{
		for (int x = 0; x < fg_labeledImg.cols; x++)
		{
			//当前像素点的RGB值
			int b = (int)fg_labeledImg.at<cv::Vec3b>(y, x)[0];
			int g = (int)fg_labeledImg.at<cv::Vec3b>(y, x)[1];
			int r = (int)fg_labeledImg.at<cv::Vec3b>(y, x)[2];

			//白色标记前景
			if ((r == 255) && (g == 255) && (b == 255))
			{
				fg_pos[count1][0] = x;
				fg_pos[count1][1] = y;
				count1++;
			}

		}
	}

	int count2 = 0;
	//遍历boreground labeled image	
	for (int y = 0; y < bg_labeledImg.rows; y++)
	{
		for (int x = 0; x < bg_labeledImg.cols; x++)
		{
			//当前像素点的RGB值
			int b = (int)bg_labeledImg.at<cv::Vec3b>(y, x)[0];
			int g = (int)bg_labeledImg.at<cv::Vec3b>(y, x)[1];
			int r = (int)bg_labeledImg.at<cv::Vec3b>(y, x)[2];


			//白色标记背景(因为输入图片分为一张前景和一张背景，所以用同种颜色标记没有问题）
			if ((r == 255) && (g == 255) && (b == 255))
			{

				bg_pos[count2][0] = x;
				bg_pos[count2][1] = y;
				count2++;
			}
		}
	}
	//提取完毕
	for (int i = 0; i < count1; ++i) {
		fg.push_back(Point(fg_pos[i][0], fg_pos[i][1]));
	}
	for (int i = 0; i < count2; ++i) {
		bg.push_back(Point(bg_pos[i][0], bg_pos[i][1]));
	}
	//将人工标记好的像素点分别设置为前景和背景
	seg.setForeground(fg);
	seg.setBackground(bg);
	Image fgimg = seg.getForeground();
	Image bgimg = seg.getBackground();
	//1:foreground 2:background
	fgimg.output(1);
	bgimg.output(2);

	cout << "Test ended. " << endl;
}

int main() {
	cout << "Begin Max Flow Test:" << endl;
	MaxFlowTest();
	cout << "Testing Interactive Graph Cuts alogrithm... " << endl;
	int flag = test();
	if (flag != -1)
		cout << "Graph Segmentation succeed." << endl;
	return 0;
}
