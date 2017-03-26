#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Image.h"
#include "ImageSegmentation.h"
#include "max_flow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>

using namespace std;
double sigma = 0.0; //���ڱ�ʾ�������ؼ�Ĳ����ƶ�

// Ĭ�Ϲ��캯��
ImageSegmentation::ImageSegmentation() : width(0), height(0), channels(0), image(0), calculated(false), max_weight(0), beta(0.0) {
	mark = new int*[0];
	mf = new MaxFlow();
}

// ���캯��
ImageSegmentation::ImageSegmentation(const std::string &filename) : image(filename), calculated(false) {
	width = image.width(); cout << width << endl;
	height = image.height(); cout << height << endl;
	channels = image.channels();
	mark = new int*[height];

	for (int y = 0; y < height; ++y) {
		mark[y] = new int[width]();
	}
	sigma = getSigma();
	// set up the max flow
	mf = new MaxFlow(height * width);
	createGraph();
}

//��ͼ
void ImageSegmentation::createGraph()
{
	cout << " Please input lambda :" << endl;
	cin >> lambda;
	cout << " Please input beta :" << endl;
	cin >> beta;
	// ������ڵ��ıߣ������� S-link T-link
	max_weight = 0;
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int pt1 = x + y * width;
			if (x > 0) {
				int pt2 = (x - 1) + y * width;
				int weight = get_weight(x, y, x - 1, y);
				max_weight = max(max_weight, weight);
				mf->add_edge(pt1, pt2, weight, weight);
			}
			if (y > 0) {
				int pt2 = x + (y - 1) * width;
				int weight = get_weight(x, y, x, y - 1);
				max_weight = max(max_weight, weight);
				mf->add_edge(pt1, pt2, weight, weight);
			}
		}
	}
	// �������Դ��ͻ��ı�
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int pt1 = x + y * width;
			double fg_prior = getPr(x, y, 1);
			double bg_prior = getPr(x, y, 2);

			mf->add_edge(mf->s(), pt1, (int)(lambda * fg_prior), (int)(lambda * fg_prior));
			mf->add_edge(pt1, mf->t(), (int)(lambda * bg_prior), (int)(lambda * bg_prior));
		}
	}
}

ImageSegmentation::~ImageSegmentation() {
	for (int y = 0; y < height; ++y) {
		delete[] mark[y];
	}
	delete[] mark;
	delete mf;
}

double ImageSegmentation::getPr(int x, int y, int t) {
	return 0.5;
}



// ��������ı�Ȩ
int ImageSegmentation::get_weight(int x1, int y1, int x2, int y2) {
	double temp = 0.0;
	double dist = 0.0;
	for (int c = 0; c < image.channels(); ++c)
	{
		temp += (image(x1, y1, c) - image(x2, y2, c)) * (image(x1, y1, c) - image(x2, y2, c));
	}
	return (int)(beta *exp(-0.25*temp / (2 * sigma*sigma)));

}

//����sigma
double ImageSegmentation::getSigma() {
	sigma = 0.0;
	int total = 0;
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			if (x > 0) {
				total++;
				for (int c = 0; c < channels; ++c) {
					sigma += (image(x, y, c) - image(x - 1, y, c)) * (image(x, y, c) - image(x - 1, y, c));
				}
			}
			if (y > 0) {
				total++;
				for (int c = 0; c < channels; ++c) {
					sigma += (image(x, y, c) - image(x, y - 1, c)) * (image(x, y, c) - image(x, y - 1, c));
				}
			}
		}
	}
	if (sigma < e) {
		return sigma = 0;
	}
	else {
		// nomalized
		sigma /= total;
		cout << "sigma : " << sigma << endl;
		return sigma;
	}
}

// ����
void ImageSegmentation::update(int x, int y, int t) {
	mark[y][x] = t;
	int pt1 = x + y * width;
	double fg_prior = getPr(x, y, 1);
	double bg_prior = getPr(x, y, 2);
	if (t == 1) {
		// foreground
		// ����t-link��weight:lambda*Rp ( Rp = -ln Pr �˴�ȡ0.5 ��
		mf->update_edge(mf->s(), pt1, (int)(lambda * bg_prior), (int)(lambda * bg_prior));
		mf->update_edge(pt1, mf->t(), (int)(lambda * fg_prior) + max_weight + 1, (int)(lambda * fg_prior) + max_weight + 1);
	}
	else if (t == 2) {
		// background
		mf->update_edge(mf->s(), pt1, (int)(lambda * bg_prior) + max_weight + 1, (int)(lambda * bg_prior) + max_weight + 1);
		mf->update_edge(pt1, mf->t(), (int)(lambda * fg_prior), (int)(lambda * fg_prior));
	}
}

//��δ֪���ص�����Ϊǰ��
void ImageSegmentation::setForeground(const vector<Point> &points) {
	calculated = false;
	for (int i = 0; i < points.size(); ++i) {
		//cout << mark[points[i].y][points[i].x] << endl;
		if (mark[points[i].y][points[i].x] == 0) {
			// �����ص�δ����� ����Ϊǰ��
			update(points[i].x, points[i].y, 1);
		}
	}
}

//��δ֪���ص�����Ϊ����
void ImageSegmentation::setBackground(const vector<Point> &points) {
	calculated = false;

	for (int i = 0; i < points.size(); ++i) {
		if (mark[points[i].y][points[i].x] == 0) {
			// �����ص�δ����� ����Ϊ����
			update(points[i].x, points[i].y, 2);
		}
	}
}

//���������������calculated�����Ϊtrue
void ImageSegmentation::calculate() {
	mf->max_flow();
	calculated = true;
}

//��ȡǰ��
Image ImageSegmentation::getForeground() {
	if (!calculated) {
		calculate();
	}
	vector<int> fg = mf->sink_component();
	cout << "Foreground Size : " << fg.size() << endl;
	Image output(width, height, channels);
	for (int i = 0; i < fg.size(); ++i) {
		if (fg[i] == mf->s() || fg[i] == mf->t()) {
			continue;
		}
		int x = fg[i] % width;
		int y = fg[i] / width;
		for (int c = 0; c < channels; ++c) {
			output(x, y, c) = image(x, y, c);
		}
	}
	return output;
}

//��ȡ����
Image ImageSegmentation::getBackground() {
	if (!calculated) {
		calculate();
	}

	vector<int> bg = mf->source_component();
	Image output(width, height, channels);
	for (int i = 0; i < bg.size(); ++i) {
		if (bg[i] == mf->s() || bg[i] == mf->t()) {
			continue;
		}
		int x = bg[i] % width;
		int y = bg[i] / width;
		for (int c = 0; c < channels; ++c) {
			output(x, y, c) = image(x, y, c);
		}
	}
	return output;
}
