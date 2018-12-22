#pragma once
/******************************
 * Point 类头文件，处理2D点   *
 * 定义基本运算               *
 * created on 2018/12/22      *
 * author 王元炜              *
 ******************************/
#include <cmath>
struct Point {
	double x, y;
	Point(double x = 0, double y = 0):  x(x), y(y) {}
	friend Point operator+(const Point &a, const Point &b);
	friend Point operator-(const Point &a, const Point &b);
	friend double dot(const Point &a, const Point &b);
	double distanceTo(const Point &other) const {
		Point tmp = *this - other;
		return sqrt(dot(tmp, tmp));
	}
	double distanceToSqr(const Point &other) const {
		Point tmp = *this - other;
		return dot(tmp, tmp);
	}
};

inline Point operator+(const Point &a, const Point &b) {
	return Point(a.x + b.x, a.y + b.y);
}

inline Point operator-(const Point &a, const Point &b) {
	return Point(a.x - b.x, a.y - b.y);
}

inline Point operator*(const Point &a, const double b) {
	return Point(a.x*b, a.y*b);
}

inline double dot(const Point &a, const Point &b) {
	return a.x*b.x + a.y*b.y;
}

inline double cross(const Point &a, const Point &b) {
	return a.x*b.y - a.y*b.x;
}
