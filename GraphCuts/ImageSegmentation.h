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
	int width, height, channels; //ͼ��Ŀ�ȣ��߶ȣ�Ƶ��
	Image image;
	int **mark; // ��ǣ�1=ǰ��, 2=����, or 0=δ��ʶ��
	MaxFlow *mf;
	bool calculated; //��ǣ��Ƿ��Ѿ�����
	int max_weight; // ���Ȩ��


	double getPr(int x, int y, int t);
	int get_weight(int x1, int y1, int x2, int y2); //��ȡȨ��
	void update(int x, int y, int t);//����
	void calculate();//����
	double getSigma();//����sigma
	void createGraph();//��ͼ

public:

	double beta; // Ȩֵ��ʽB{p,q}����
	double lambda = 0.0; // 
	ImageSegmentation(); //Ĭ�Ϲ��캯��
	ImageSegmentation(const std::string &filename); //���캯��
	~ImageSegmentation(); //��������
	void setForeground(const vector<Point> &points);//��δ֪���ص�����Ϊǰ��
	void setBackground(const vector<Point> &points);//��δ֪���ص�����Ϊ����
	Image getForeground();//��ȡǰ��
	Image getBackground();//��ȡ����
};

#endif
