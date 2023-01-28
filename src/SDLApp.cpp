#include "SDLApp.hpp"

SDLApp::SDLApp(const std::string& title, const int& winWidth, const int& winHeight){
	_winWidth  = winWidth;	
	_winHeight = winHeight;
	_title = title;

	// create window at position(50, 50)
	_window = SDL_CreateWindow((const char*)&_title, 50, 50, _winWidth, _winHeight, SDL_WINDOW_SHOWN);
	
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG); 
    SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengl", SDL_HINT_OVERRIDE);


    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

SDLApp::~SDLApp(){
	SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

void SDLApp::clearScreen(){
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);
}

void SDLApp::presentScreen(){
	SDL_RenderPresent(_renderer);
}

SDL_Renderer* SDLApp::getRenderer(){
	return _renderer;
}

