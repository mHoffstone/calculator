#ifndef VECTOR_H
#define VECTOR_H

#include <wx/geometry.h>

struct vec2d{
	double x = 0.0, y = 0.0;

	vec2d& operator+=(const vec2d& rhs);
	vec2d operator+(const vec2d& rhs) const;

	vec2d& operator-=(const vec2d& rhs);
	vec2d operator-(const vec2d& rhs) const;
	vec2d operator-() const;

	vec2d& operator*=(const double& rhs);
	vec2d operator*(const double& rhs) const;
	vec2d& operator*=(const vec2d& rhs);
	vec2d operator*(const vec2d& rhs) const;

	double length() const;
	double length2() const;
	void normalize();

	double dot(const vec2d& v2) const;

	operator wxPoint() const;
};

#endif