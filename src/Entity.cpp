#include "Entity.hpp"

// Entity::Entity(const vec2d<float>& pos){
// 	_pos = pos;	
// }

Entity::Entity(const vec2d<float>& pos, const vec2d<float>& size, SDL_Texture* texture){
	_pos = pos;	
	_texture = texture;
	_size = size;
}

Entity::~Entity(){
	SDL_DestroyTexture(_texture);
}

std::unique_ptr<SDL_Rect> Entity::getRect(const Camera& cam){
	auto ptr = std::make_unique<SDL_Rect>();
	auto tmp = cam.WorldToScreen(_pos);
	auto tmp2 = cam.WorldToScreen(_pos + _size);
	ptr->x = tmp.x;
	ptr->y = tmp.y;
	ptr->w = tmp2.x - tmp.x;
	ptr->h = tmp2.y - tmp.y;	
	return ptr;
}

void Entity::render(SDL_Renderer* renderer, const Camera& cam){
	if(_texture == nullptr){
		return;
	}	
	SDL_RenderCopy(renderer, _texture, NULL, getRect(cam).get());
}

void Entity::renderWithPos(SDL_Renderer* renderer, const Camera& cam, const vec2d<float>& v){
	if(_texture == nullptr){
		return;
	}	
	auto tmp = cam.WorldToScreen(v);
	auto tmp2 = cam.WorldToScreen(v + _size);
	SDL_Rect r = {tmp.x, tmp.y, tmp2.x - tmp.x, tmp2.y - tmp.y};
	SDL_RenderCopy(renderer, _texture, NULL, &r);
}

void Entity::setPos(const vec2d<float>& v){
	_pos = v;
}

const vec2d<float>& Entity::getPos(){
	return _pos;
}

const vec2d<float>& Entity::getSize(){
	return _size;
}