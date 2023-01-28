#pragma once

// C++ standard library
#include <iostream>

// SDL library
#include "SDL2/SDL.h"

template <typename T>
class vec2d{
public:
	vec2d();
	vec2d(T xx, T yy);
	T x;
	T y;

	int Manhattan(const vec2d<T>& xx) const;
	int Manhattan(const vec2d<T>&& xx) const;

	// euclidean distance
	float Distance(const vec2d<T>& xx) const;
	float Distance(const vec2d<T>&& xx) const;

	void Show() const;
	vec2d<T> operator + (const vec2d<T>& rhs) const;
	vec2d<T> operator - (const vec2d<T>& rhs) const;
	vec2d<T> operator * (const T& rhs) const;
	vec2d<T> operator / (const T& rhs) const;
	vec2d<T>& operator = (const vec2d<T>& rhs);
	vec2d<T>& operator *= (const T& rhs);
	vec2d<T>& operator += (const vec2d<T>& rhs);
	vec2d<T>& operator -= (const vec2d<T>& rhs);

	bool operator == (const vec2d<T>& rhs) const;
	bool operator != (const vec2d<T>& rhs) const;
};

#include "vec2d.tpp"