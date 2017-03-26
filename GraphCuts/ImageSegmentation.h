#ifndef IMAGE_SEGMENTATION_H
#define IMAGE_SEGMENTATION_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "Image.h"
#include "max_flow.h"
#include "point.h"
using namespace std;

const double e = 1.0e-10;
class ImageSegmentation {
private:
	int width, height, channels; //图像的宽度，高度，频道
	Image image;
	int **mark; // 标记：1=前景, 2=背景, or 0=未标识的
	MaxFlow *mf;
	bool calculated; //标记：是否已经计算
	int max_weight; // 最大权重


	double getPr(int x, int y, int t);
	int get_weight(int x1, int y1, int x2, int y2); //获取权重
	void update(int x, int y, int t);//更新
	void calculate();//计算
	double getSigma();//计算sigma
	void createGraph();//建图

public:

	double beta; // 权值公式B{p,q}比例
	double lambda = 0.0; // 
	ImageSegmentation(); //默认构造函数
	ImageSegmentation(const std::string &filename); //构造函数
	~ImageSegmentation(); //析构函数
	void setForeground(const vector<Point> &points);//将未知像素点设置为前景
	void setBackground(const vector<Point> &points);//将未知像素点设置为背景
	Image getForeground();//获取前景
	Image getBackground();//获取背景
};

#endif
