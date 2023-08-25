#include "Camera.hpp"

Camera::Camera(int x, int y, int w, int h, int wx, int wy, int ww, int wh){
	// The camera w, h, and the output window, ww, wh 
	// must have the same ascept ratio
	// as ww, wh 
	// otherwise the texture might look strected

        //camera size ans position
	_pos = vec2d<float>(x, y);
	_cameraSize = vec2d<float>(w, h);
	_scaledCameraSize = vec2d<float>(w, h);

//window offset x and y (on screen coor)
//window width and height
	_windowOffset = vec2d<int> (wx, wy);
	_windowSize = vec2d<int> (ww, wh);

// zoom
	_zoomFactor = .1;
	_scaleFactor = vec2d<float>(1.0, 1.0);
}

void Camera::Move(const vec2d<int>& v){
	//everything shown on screen is move by (x, y) pixel
	_pos += vec2d<float>(-_scaledCameraSize.x / _windowSize.x * v.x, -_scaledCameraSize.y / _windowSize.y * v.y);
}

void Camera::ZoomIn(const vec2d<int>& pos, const int& magnitude){
	_scaleFactor *= pow(1 - _zoomFactor, magnitude);
	float t = (pos.x - _windowOffset.x)  / (float)_windowSize.x;
	float t2 = (pos.y - _windowOffset.y) / (float)_windowSize.y;
	_pos += vec2d<float>(t * (1 - pow(1 - _zoomFactor, magnitude)) * _scaledCameraSize.x, (t2 * ((1 - pow(1 - _zoomFactor, magnitude)) * _scaledCameraSize.y)));
	_scaledCameraSize = vec2d<float>(_cameraSize.x * _scaleFactor.x, _cameraSize.y * _scaleFactor.y);
}

void Camera::ZoomOut(const vec2d<int>& pos, const int& magnitude){
	_scaleFactor *= pow(1 + _zoomFactor, magnitude);
	float t = (pos.x - _windowOffset.x) / (float)_windowSize.x;
	float t2 = (pos.y - _windowOffset.y) / (float)_windowSize.y;
	_pos += vec2d<float>(t * (1 - pow(1 + _zoomFactor, magnitude)) * _scaledCameraSize.x, (t2 * ((1 - pow(1 + _zoomFactor, magnitude)) * _scaledCameraSize.y)));
	_scaledCameraSize = vec2d<float>(_cameraSize.x * _scaleFactor.x, _cameraSize.y * _scaleFactor.y);
}

vec2d<float> Camera::ScreenToWorld(const vec2d<int>& v) const{
	float xx = (float)(v.x - _windowOffset.x) / _windowSize.x * _scaledCameraSize.x + _pos.x;
	float yy = (float)(v.y - _windowOffset.y) / _windowSize.y * _scaledCameraSize.y + _pos.y;
	return vec2d<float>(xx, yy);
}

vec2d<int> Camera::WorldToScreen(const vec2d<float>& v) const{
	int xx = (int)(round((v.x - _pos.x) /_scaledCameraSize.x * (float)_windowSize.x)) + _windowOffset.x;
	int yy = (int)(round((v.y - _pos.y) /_scaledCameraSize.y * (float)_windowSize.y)) + _windowOffset.y;
	return vec2d<int>(xx, yy);
}

std::unique_ptr<SDL_Rect> Camera::GetRect() const{
	//Get Rect for camera in window coor
	std::unique_ptr<SDL_Rect> p = std::make_unique<SDL_Rect>();
	p->x = _windowOffset.x;
	p->y = _windowOffset.y;
	p->w = _windowSize.x;
	p->h = _windowSize.y;
	return p;
}

int Camera::GetWindowHeight() const{
	return _windowSize.y;
}

int Camera::GetWindowWidth() const{
	return _windowSize.x;
}

float Camera::GetScaling() const{
	return 1 / _scaleFactor.x;
}
