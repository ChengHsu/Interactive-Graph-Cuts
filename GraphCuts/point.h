#ifndef POINT_H
#define POINT_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

class Point {
public:
	int x, y; // ������
	Point() { x = 0; y = 0; } //Ĭ�Ϲ��캯��
	//���캯��
	Point(int xp, int yp)
	{
		x = xp; y = yp;
	}
	Point(const Point& other) {
		x = other.x;
		y = other.y;
	};
	Point& operator= (const Point& other) {
		x = other.x;
		y = other.y;
		return *this;
	};
	void print() { printf("(%d, %d)\n", x, y); }
	~Point() {};
};



#endif
