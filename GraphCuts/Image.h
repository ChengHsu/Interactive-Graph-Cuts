#ifndef __IMAGE__H
#define __IMAGE__H
#include "lodepng.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cfloat>
#include <math.h>
#include "ImageException.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>

using namespace std;


class Image {
public:
	//���캯��
	Image(int width_, int height_ = 0, int channels_ = 0, const std::string &name = "");
	Image(const std::string & filename);
	//��������
	~Image();
	//����ͼ������
	const std::string & name() const { return image_name; }
	//����ͼ��ά��
	int dimensions() const { return dims; }
	//������������ֵ֮��ľ���
	int stride(int dim) const { return stride_[dim]; }
	int width() const { return dim_values[0]; }//ͼ���
	int height() const { return dim_values[1]; }//ͼ���
	int channels() const { return dim_values[2]; }//ͼ��Ƶ��
	//�������
	void write(const std::string & filename) const;
	void output(int num) const; 

	const float & operator()(int x) const;
	const float & operator()(int x, int y) const;
	const float & operator()(int x, int y, int z) const;
	float & operator()(int x);
	float & operator()(int x, int y);
	float & operator()(int x, int y, int z);
	//Ԫ�ظ���
	long long number_of_elements() const;

private:
	unsigned int dims; //ά�ȸ���
	unsigned int dim_values[3]; // γ�ȴ�С
	unsigned int stride_[3]; 
	std::string image_name; // ͼ����
	std::vector<float> image_data; //���ڴ洢����ֵ
	//����ͼ���������͵�ת�� uint8:[0-255] float:[-1,1]or[0,1]
	static float uint8_to_float(const unsigned char & in); 
	static unsigned char float_to_uint8(const float &in); 
	void initImgData(int x, int y, int z, const std::string &name_); //��ʼ������

};

#endif
