#include "vector.h"
#include <cmath>

vec2d& vec2d::operator+=(const vec2d& rhs){
	x += rhs.x;
	y += rhs.y;
	return *this;
}
vec2d vec2d::operator+(const vec2d& rhs) const{
	vec2d v = *this;
	v += rhs;
	return v;
}

vec2d& vec2d::operator-=(const vec2d& rhs){
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}
vec2d vec2d::operator-(const vec2d& rhs) const{
	vec2d v = *this;
	v -= rhs;
	return v;
}
vec2d vec2d::operator-() const{
	return vec2d{-x, -y};
}

vec2d& vec2d::operator*=(const double& rhs){
	x *= rhs;
	y *= rhs;
	return *this;
}
vec2d vec2d::operator*(const double& rhs) const{
	vec2d v = *this;
	v *= rhs;
	return v;
}

vec2d& vec2d::operator*=(const vec2d& rhs){
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}
vec2d vec2d::operator*(const vec2d& rhs) const{
	vec2d v = *this;
	v *= rhs;
	return v;
}

double vec2d::length() const{
	return std::sqrt(x*x + y*y);
}
double vec2d::length2() const{
	return x*x + y*y;
}
void vec2d::normalize(){
	*this *= 1.0f/length();
}

double vec2d::dot(const vec2d& v2) const{
	return x * v2.x + y * v2.y;
}

vec2d::operator wxPoint() const{
	return wxPoint(x, y);
}