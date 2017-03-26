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
	//构造函数
	Image(int width_, int height_ = 0, int channels_ = 0, const std::string &name = "");
	Image(const std::string & filename);
	//析构函数
	~Image();
	//返回图像名称
	const std::string & name() const { return image_name; }
	//返回图像维度
	int dimensions() const { return dims; }
	//计算相邻像素值之间的距离
	int stride(int dim) const { return stride_[dim]; }
	int width() const { return dim_values[0]; }//图像宽
	int height() const { return dim_values[1]; }//图像高
	int channels() const { return dim_values[2]; }//图像频道
	//用于输出
	void write(const std::string & filename) const;
	void output(int num) const; 

	const float & operator()(int x) const;
	const float & operator()(int x, int y) const;
	const float & operator()(int x, int y, int z) const;
	float & operator()(int x);
	float & operator()(int x, int y);
	float & operator()(int x, int y, int z);
	//元素个数
	long long number_of_elements() const;

private:
	unsigned int dims; //维度个数
	unsigned int dim_values[3]; // 纬度大小
	unsigned int stride_[3]; 
	std::string image_name; // 图像名
	std::vector<float> image_data; //用于存储像素值
	//用于图像数据类型的转换 uint8:[0-255] float:[-1,1]or[0,1]
	static float uint8_to_float(const unsigned char & in); 
	static unsigned char float_to_uint8(const float &in); 
	void initImgData(int x, int y, int z, const std::string &name_); //初始化数据

};

#endif
