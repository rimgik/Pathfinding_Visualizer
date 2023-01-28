#pragma once

#include <memory>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


class SDLApp{
public:
	SDLApp(const std::string& title, const int& winWidth, const int& winHeight);
	~SDLApp();

	void clearScreen();
	void presentScreen();

	SDL_Renderer* getRenderer();

private:
	int _winWidth;
	int _winHeight;

	std::string _title;

	SDL_Window* _window ;
	SDL_Renderer* _renderer;
};