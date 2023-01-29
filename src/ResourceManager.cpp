#include "ResourceManager.hpp"

ResourceManager::ResourceManager(){

}

ResourceManager::~ResourceManager(){
	FreeTexture();
}

ResourceManager& ResourceManager::GetInstance(){
	static ResourceManager *res = new ResourceManager;
	return *res;
}

void ResourceManager::FreeTexture(){
	for(auto &i : _texture){
		SDL_DestroyTexture(i.second);	
		std::cout << "ResourceManager: Texture : " << i.first << " has been free." << std::endl;
	}
}

SDL_Texture* ResourceManager::LoadTextureFromFile(SDL_Renderer* renderer,const std::string& filepath){
	auto search = _texture.find(filepath);
	if(search != _texture.end()){
		return _texture[filepath];
	}

    SDL_Surface *image;
    image = IMG_Load(filepath.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    _texture[filepath] = texture;
    return texture;
}
