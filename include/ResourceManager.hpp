#pragma once
//
#include <iostream>
#include <string>
#include <unordered_map>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


class ResourceManager{
private:
	ResourceManager();
	~ResourceManager();
public: 
	//Get this instance, singleton class
	static ResourceManager& GetInstance();
	//Free all texture Stored in GPU
	void FreeTexture();
	//Load png file to texture(GPU)
	SDL_Texture* LoadTextureFromFile(SDL_Renderer* renderer, const std::string& filepath);

private:
	std::unordered_map<std::string, SDL_Texture*> _texture;
};