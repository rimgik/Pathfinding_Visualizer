#pragma once

// C++ standard library
#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <string>

// SDL library
#include "SDL2/SDL.h"

// include
#include "vec2d.hpp"

class Camera{
public:
	Camera(int x, int y, int w, int h, int wx, int wy, int ww, int wh);

	// translation 
	void Move(const vec2d<int>& v);
	// zoom in/out center at pos, with magnitude
	void ZoomIn(const vec2d<int>& pos, const int& magnitude);
	void ZoomOut(const vec2d<int>& pos, const int& magnitude);

	// world / screen coor mapping
	vec2d<float> ScreenToWorld(const vec2d<int>& v) const;
	vec2d<int> WorldToScreen(const vec2d<float>& v) const;

	//return smart pointer which point to windowRect
	std::unique_ptr<SDL_Rect> GetRect() const;


	// Getter function

	int GetWindowHeight() const;
	int GetWindowWidth() const;
	// How many an object must be scale to match the one on camera
	float GetScaling() const;
private:
	//camera position and size
	vec2d<float> _pos;
	vec2d<float> _cameraSize;
	vec2d<float> _scaledCameraSize;
	int _w;
	int _h;

	vec2d<int> _windowOffset;
	vec2d<int> _windowSize;
	int _ww;
    int _wh;

	float _zoomFactor;
	//scaled size of camera
	float _sw;
	float _sh;

	// 1/scale * original is the scaled size you see on screen
	// the scale factor for x, y are same, separate them
	// in case of implementing any visual effect later
	vec2d<float> _scaleFactor;
};

