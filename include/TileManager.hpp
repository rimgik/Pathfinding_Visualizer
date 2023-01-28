#pragma once

// C++ standard lib
#include <vector>
#include <memory>
#include <iostream>

// SDL
#include "SDL2/SDL.h"

//include
#include "vec2d.hpp"
#include "Camera.hpp"


class TileManager{
public:
	TileManager(const vec2d<int>& size, const int& length);

	void render(SDL_Renderer* renderer, const Camera& cam) const;
	void renderPath(SDL_Renderer* renderer, const Camera& cam, std::unique_ptr<std::vector<vec2d<int>>> ptr) const;
	void renderVisited(SDL_Renderer* renderer, const Camera& cam, const std::vector<std::vector<bool>>& vis) const;

	vec2d<int> WorldToTile(const vec2d<float>& v) const;
	vec2d<float> TileToWorld(const vec2d<int>& v, bool flag) const;

	bool ToggleWallFromGrid(const vec2d<int>& v);	
	bool ToggleWallFromWorld(const vec2d<float>& v);	
	bool SetWallFromGrid(const vec2d<int>& v);
	bool SetWallFromWorld(const vec2d<float>& v);
	bool ClearWallFromGrid(const vec2d<int>& v);
	bool ClearWallFromWorld(const vec2d<float>& v);
	bool IsWallFromGrid(const vec2d<int>& v) const;
	bool IsWallFromWorld(const vec2d<float>& v) const;
	// return true if inside grid
	bool checkBoundry(const vec2d<int>& v) const;

	std::shared_ptr<std::vector<std::vector<bool>>> getGrid() const;
private:

	int _length;
	// std::vector<std::vector<bool>> _grid;
	vec2d<int> _size;
	std::shared_ptr<std::vector<std::vector<bool>>> _grid;
};