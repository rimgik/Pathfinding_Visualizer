#include "TileManager.hpp"

TileManager::TileManager(const vec2d<int>& size, const int& length){
	// _grid = std::vector<std::vector<bool>> (size.x, std::vector<bool>(size.y, false));
	_length = length;
	_grid = std::make_shared<std::vector<std::vector<bool>>>(std::vector<std::vector<bool>> (size.x, std::vector<bool>(size.y, false)));
	_size = size;
}

void TileManager::render(SDL_Renderer* renderer, const Camera& cam) const{
    // Draw the grid line, only draw those are visible

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	vec2d<float> top_left = cam.ScreenToWorld(vec2d<int>(0, 0));
	vec2d<float> bottom_right = cam.ScreenToWorld(vec2d<int>(cam.GetWindowWidth(), cam.GetWindowHeight()));
	
	int left_most_line = std::max(0, ((int)(top_left.x / _length) * _length));
	int top_most_line = std::max(0, ((int)(top_left.y / _length) * _length));
	int bottom_most_line = std::min(_size.x * _length, ((int)(bottom_right.y + _length - 1) / _length) * _length );
	int right_most_line = std::min(_size.y * _length,  ((int)(bottom_right.x + _length - 1) / _length) * _length );


	for(int i = left_most_line ; i <= _size.y * _length	&& cam.WorldToScreen(vec2d<float>(i,0)).x <= cam.GetWindowWidth(); i += _length){
		auto toScreen = cam.WorldToScreen(vec2d<float>(i, top_most_line)); 
		auto toScreen2 = cam.WorldToScreen(vec2d<float>(i, bottom_most_line));
		SDL_RenderDrawLine(renderer, toScreen.x, toScreen.y, toScreen.x, toScreen2.y);
	}
	for(int i = top_most_line; i <= _size.x * _length && cam.WorldToScreen(vec2d<float>(0,i)).y <= cam.GetWindowHeight(); i += _length){
		auto toScreen = cam.WorldToScreen(vec2d<float>(left_most_line, i)); 
		auto toScreen2 = cam.WorldToScreen(vec2d<float>(right_most_line, i));
		SDL_RenderDrawLine(renderer, toScreen.x, toScreen.y, toScreen2.x, toScreen.y);
	}

	// drawing wall, those aren't visible are drawn too
	for(int i = 0; i < _size.x; ++i){
		for(int j = 0; j < _size.y;++j){
			if(_grid->at(i)[j] == 0)continue;
			auto toScreen = cam.WorldToScreen(vec2d<float>(j * _length, i * _length));
			auto toScreen2 = cam.WorldToScreen(vec2d<float>(j * _length + _length, i * _length + _length));
			SDL_Rect r = {toScreen.x, toScreen.y, toScreen2.x - toScreen.x, toScreen2.y - toScreen.y};
			SDL_RenderFillRect(renderer, &r);
		}
	}

}

void TileManager::renderPath(SDL_Renderer* renderer, const Camera& cam, std::unique_ptr<std::vector<vec2d<int>>> ptr) const{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	for(int i = 1; i < ptr->size(); ++i){
		auto prev = TileToWorld(ptr->at(i-1), 1);
		auto curr = TileToWorld(ptr->at(i), 1);
		auto prevScreen = cam.WorldToScreen(prev);
		auto currScreen = cam.WorldToScreen(curr);
		SDL_RenderDrawLine(renderer, prevScreen.x, prevScreen.y, currScreen.x, currScreen.y);
	}
}

void TileManager::renderVisited(SDL_Renderer* renderer, const Camera& cam, const std::vector<std::vector<bool>>& vis) const{
	SDL_SetRenderDrawColor(renderer, 0, 100, 100, SDL_ALPHA_OPAQUE);
	for(int i = 0; i < _size.x; ++i){
		for(int j = 0; j < _size.y;++j){
			if(vis[i][j] == 0)continue;
			auto toScreen = cam.WorldToScreen(vec2d<float>(j * _length, i * _length));
			auto toScreen2 = cam.WorldToScreen(vec2d<float>(j * _length + _length, i * _length + _length));
			SDL_Rect r = {toScreen.x, toScreen.y, toScreen2.x - toScreen.x, toScreen2.y - toScreen.y};
			SDL_RenderFillRect(renderer, &r);
		}
	}	
}

vec2d<int> TileManager::WorldToTile(const vec2d<float>& v) const{
	return vec2d<int>(
			v.y >= 0 ? (int)(v.y / _length) : (int)((v.y - _length + 1) / _length),
			v.x >= 0 ? (int)(v.x / _length) : (int)((v.x - _length + 1) / _length)
		);
}

vec2d<float> TileManager::TileToWorld(const vec2d<int>& v, bool flag = 0) const{
	return vec2d<float>(
			v.y * _length + (flag ? (float)_length / 2 : 0),
			v.x * _length + (flag ? (float)_length / 2 : 0)
		);
}

bool TileManager::ToggleWallFromGrid(const vec2d<int>& v){
	if(v.x >= _size.x || v.x < 0 || v.y >= _size.y || v.y < 0)return 2;
	return _grid->at(v.x)[v.y] = 1 - _grid->at(v.x)[v.y];
}

bool TileManager::ToggleWallFromWorld(const vec2d<float>& v){
	return ToggleWallFromGrid(WorldToTile(v));
}

bool TileManager::SetWallFromGrid(const vec2d<int>& v){
	if(v.x >= _size.x || v.x < 0 || v.y >= _size.y || v.y < 0)return 2;
	return _grid->at(v.x)[v.y] = 1;
}

bool TileManager::SetWallFromWorld(const vec2d<float>& v){
	return SetWallFromGrid(WorldToTile(v));
}

bool TileManager::ClearWallFromGrid(const vec2d<int>& v){
	if(v.x >= _size.x || v.x < 0 || v.y >= _size.y || v.y < 0)return 2;
	return _grid->at(v.x)[v.y] = 0;
}

bool TileManager::ClearWallFromWorld(const vec2d<float>& v){
	return ClearWallFromGrid(WorldToTile(v));
}

bool TileManager::IsWallFromGrid(const vec2d<int>& v) const{
	if(v.x >= _size.x || v.x < 0 || v.y >= _size.y || v.y < 0)return 2;
	return _grid->at(v.x)[v.y];
}

bool TileManager::IsWallFromWorld(const vec2d<float>& v) const{
	return IsWallFromGrid(WorldToTile(v));
}

std::shared_ptr<std::vector<std::vector<bool>>> TileManager::getGrid() const{
	return _grid;	
}

bool TileManager::checkBoundry(const vec2d<int>& v) const {
	if(v.x >= _size.x || v.x < 0 || v.y >= _size.y || v.y < 0)return 0;
	return 1;
}