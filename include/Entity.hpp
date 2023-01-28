#pragma once

#include "SDL2/SDL.h"

#include "vec2d.hpp"
#include "Camera.hpp"

class Entity{
public:
	// Entity(const vec2d<float> pos);
	Entity(const vec2d<float>& pos, const vec2d<float>& size, SDL_Texture* texture);
	~Entity();

	std::unique_ptr<SDL_Rect> getRect(const Camera& cam);

	void render(SDL_Renderer* renderer, const Camera& cam);
	void renderWithPos(SDL_Renderer* renderer, const Camera& cam, const vec2d<float>& v);

	void setPos(const vec2d<float>& v);
	const vec2d<float>& getPos();
	const vec2d<float>& getSize();

private:

	vec2d<float> _pos;
	vec2d<float> _size;

	SDL_Texture* _texture;



};