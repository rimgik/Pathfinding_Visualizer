// #include "vec2d.h"

template<typename T>
vec2d<T>::vec2d(){
	x = 0, y = 0;	
}

template<typename T>
vec2d<T>::vec2d(T xx, T yy){
	x = xx, y = yy;
}

template<typename T>
int vec2d<T>::Manhattan(const vec2d<T>& xx) const{
	return abs(xx.x - x) + abs(xx.y - y);
}

template<typename T>
int vec2d<T>::Manhattan(const vec2d<T>&& xx) const{
	return abs(xx.x - x) + abs(xx.y - y);
}

template<typename T>
float vec2d<T>::Distance(const vec2d<T>& xx) const{
	return sqrt((xx.x - x) * (xx.x - x) + (xx.y - y) * (xx.y - y));
}

template<typename T>
float vec2d<T>::Distance(const vec2d<T>&& xx) const{
	return sqrt((xx.x - x) * (xx.x - x) + (xx.y - y) * (xx.y - y));
}

template<typename T>
void vec2d<T>::Show() const{
	std::cout << x << "," << y << std::endl;
}

template<typename T>
vec2d<T> vec2d<T>::operator + (const vec2d<T>& rhs) const{
	return vec2d<T> (x + rhs.x, y + rhs.y);
}

template<typename T>
vec2d<T> vec2d<T>::operator - (const vec2d<T>& rhs) const{
	return vec2d<T> (x - rhs.x, y - rhs.y);
}

template<typename T>
vec2d<T> vec2d<T>::operator * (const T& rhs) const{
	return vec2d<T> (x * rhs, y * rhs);
}

template<typename T>
vec2d<T> vec2d<T>::operator / (const T& rhs) const{
	return vec2d<T> (x / rhs, y / rhs);
}

template<typename T>
vec2d<T>& vec2d<T>::operator = (const vec2d<T>& rhs){
	x = rhs.x, y = rhs.y;
	return *this;
}

template<typename T>
vec2d<T>& vec2d<T>::operator *= (const T& rhs){
	x *= rhs, y *= rhs;
	return *this;
}

template<typename T>
vec2d<T>& vec2d<T>::operator += (const vec2d<T>& rhs){
	x += rhs.x, y += rhs.y;
	return *this;
}

template<typename T>
vec2d<T>& vec2d<T>::operator -= (const vec2d<T>& rhs){
	x -= rhs.x, y -= rhs.y;
	return *this;
}

template<typename T>
bool vec2d<T>::operator == (const vec2d<T>& rhs) const{
	return (rhs.x == x) && (rhs.y == y);
}

template<typename T>
bool vec2d<T>::operator != (const vec2d<T>& rhs) const{
	return (rhs.x != x) || (rhs.y != y);
}