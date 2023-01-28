// C++ standard library
#include <iostream>
#include <memory>
#include <vector>

// SDL library
#include "SDL2/SDL.h"

// include
#include "Pathfinder.hpp"
#include "vec2d.hpp"
#include "Camera.hpp"
#include "SDLApp.hpp"
#include "TileManager.hpp"
#include "ResourceManager.hpp"
#include "Entity.hpp"

int main(){
	const int windowWidth = 1280;
	const int widnowHeight = 720;

	SDLApp window("Pathfinding Visualizer", windowWidth, widnowHeight);
	Camera cam(0, 0, windowWidth, widnowHeight, 0, 0, windowWidth, widnowHeight);

	int row = 20;
	int col = 40;
	int gridSize = 30;

	TileManager* tm = new TileManager(vec2d<int>(row, col), gridSize);

	SDL_Texture* target_texture = ResourceManager::GetInstance().LoadTextureFromFile(window.getRenderer(), "./data/target.png");
	SDL_Texture* startingPoint_texture = ResourceManager::GetInstance().LoadTextureFromFile(window.getRenderer(), "./data/start.png");
	Entity target(vec2d<float>(gridSize * (col - 1), gridSize * (row - 1)), vec2d<float>(gridSize, gridSize), target_texture);
	Entity startingPoint(vec2d<float>(0, 0), vec2d<float>(gridSize, gridSize), startingPoint_texture);

	Pathfinder* ptr = nullptr;

	//selected algorithm
	int bfs = 1;
	int dfs = 1<<1;
	int astar = 1<<2;
	int selectedAlgorithm = 1;

	// init 
	bool isRunning = 1;
	bool setWallMode = 1;
	bool draggingTarget = 0;	
	bool draggingStartingPoint = 0;
	bool holdingMouseWall = 0;

	while(isRunning){
		SDL_Event event;

    	int mouse_x, mouse_y;
	    Uint32 mouseState = SDL_GetMouseState(&mouse_x, &mouse_y); 
	    while(SDL_PollEvent(&event)){
	    	// mouse thing
	    	mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
	        vec2d<int> mouse = vec2d<int>(mouse_x, mouse_y);
	        vec2d<float> toWorld = cam.ScreenToWorld(mouse);
	
	    	if(event.type == SDL_QUIT){
	    		isRunning = false;
	    		break;

	    	}else if(event.type == SDL_MOUSEWHEEL){
	            if(event.wheel.y < 0){
	                //Zoom out
	                cam.ZoomOut(mouse, -event.wheel.y);
	            }else if(event.wheel.y > 0){
	                //Zoom in
	                cam.ZoomIn(mouse, event.wheel.y);
	            }
            }else if(event.type == SDL_MOUSEMOTION && event.button.clicks < 1){
	            if(mouseState & SDL_BUTTON(3)){
	                // right mouse drag
	            	// drag is relative to screen world
	                cam.Move(vec2d<int>(event.motion.xrel, event.motion.yrel));
	            }
	        }else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !holdingMouseWall && !draggingTarget && !draggingStartingPoint && (ptr==nullptr || ptr->hasEnded())){
	        	// this is trigger when mouse is first press
	        	// if the mouse click at the map, set wall mode is activated, 
	        	// else if the mouse click is on the target/starting Point, dragging mode is activated

	        	// Reset the pathfinding
	        	delete ptr;
	        	ptr = nullptr;

	        	// check if the target/starting point is selected 
	        	SDL_Point* temp = new SDL_Point{mouse.x, mouse.y};
	        	if(SDL_PointInRect(temp , target.getRect(cam).get())){
	        		draggingTarget = 1;
	        	}else if(SDL_PointInRect(temp, startingPoint.getRect(cam).get())){
	        		draggingStartingPoint = 1;
	        	}

	        	delete temp;
	        	// if it's dragging mode then quit
	        	if(draggingTarget || draggingStartingPoint)continue;	

	        	// else it is in set wall mode, depends on the first contact
	        	// setWallMode = 0 -> clearing Wall
	        	// setWallMode = 1 -> setting Wall
	        	if(tm->IsWallFromWorld(toWorld)){
	        		setWallMode = 0;
	        	}else setWallMode = 1;
	        	// is in setting wall mode
	        	holdingMouseWall = 1;

	        }
	        if((mouseState & SDL_BUTTON(1) )&& event.button.button == SDL_BUTTON_LEFT && holdingMouseWall){
	        	// holding left mouse button, this is used to toggle wall
	        	if(ptr != nullptr && !ptr->hasEnded())continue;
	        	if(setWallMode)
		        	tm->SetWallFromWorld(toWorld);
		        else tm->ClearWallFromWorld(toWorld);
	        }else if(((mouseState & SDL_BUTTON(1))== 0)){
	        	// left mouse button released
	        	holdingMouseWall = 0;

	        	if(draggingTarget){
	        		draggingTarget = 0;
	        		if(tm->checkBoundry(tm->WorldToTile(cam.ScreenToWorld(mouse)))){
	        			tm->ClearWallFromWorld(toWorld);
		        		target.setPos(tm->TileToWorld(tm->WorldToTile(cam.ScreenToWorld(mouse)), 0));
	        		}
	        	}else if(draggingStartingPoint){
	        		draggingStartingPoint = 0;	
	        		if(tm->checkBoundry(tm->WorldToTile(cam.ScreenToWorld(mouse)))){
	        			tm->ClearWallFromWorld(toWorld);
		        		startingPoint.setPos(tm->TileToWorld(tm->WorldToTile(cam.ScreenToWorld(mouse)), 0));
	        		}
	        	}

	        }
	        //These are for changing the size of grid, and the space_key is for starting searching
	        if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
	        	delete ptr;
	        	if(selectedAlgorithm & bfs){
		        	ptr = new Bfs(*tm->getGrid(), vec2d<int>(row, col), tm->WorldToTile(startingPoint.getPos()), tm->WorldToTile(target.getPos()));	
	        	}else if(selectedAlgorithm & dfs){
		        	ptr = new Dfs(*tm->getGrid(), vec2d<int>(row, col), tm->WorldToTile(startingPoint.getPos()), tm->WorldToTile(target.getPos()));	
	        	}else{
		        	ptr = new AStar(*tm->getGrid(), vec2d<int>(row, col), tm->WorldToTile(startingPoint.getPos()), tm->WorldToTile(target.getPos()));	
	        	}
	        }else if(event.key.keysym.scancode == SDL_SCANCODE_DOWN && (ptr == nullptr || ptr->hasEnded())){
	        	// reset tm and pathfinding after changing the size
	        	delete tm;
	        	delete ptr;
	        	ptr = nullptr;
	        	tm = new TileManager(vec2d<int>(++row, col), gridSize); 

	        }else if(event.key.keysym.scancode == SDL_SCANCODE_UP && (ptr == nullptr || ptr->hasEnded())){
	        	if(row == 1)continue;
	        	delete tm;
	        	delete ptr;
	        	ptr = nullptr;
	        	tm = new TileManager(vec2d<int>(--row, col), gridSize);
	        }else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT && (ptr == nullptr || ptr->hasEnded())){
	        	delete tm;
	        	delete ptr;
	        	ptr = nullptr;
	        	tm = new TileManager(vec2d<int>(row, ++col), gridSize);
	        }else if(event.key.keysym.scancode == SDL_SCANCODE_LEFT && (ptr == nullptr || ptr->hasEnded())){
	        	if(col == 1)continue;
	        	delete tm;
	        	delete ptr;
	        	ptr = nullptr;
	        	tm = new TileManager(vec2d<int>(row, --col), gridSize);
	        }else if(event.key.keysym.scancode == SDL_SCANCODE_1){
	        	selectedAlgorithm = 1;
	        }else if(event.key.keysym.scancode == SDL_SCANCODE_2){
	        	selectedAlgorithm = 2;
	        }else if(event.key.keysym.scancode == SDL_SCANCODE_3){
	        	selectedAlgorithm = 4;
	        }
        	if(tm->checkBoundry(tm->WorldToTile(target.getPos())) == false){
        		target.setPos(vec2d<float>((col - 1) * gridSize , (row - 1) *gridSize));
        	}
        	if(tm->checkBoundry(tm->WorldToTile(startingPoint.getPos())) == false){
        		startingPoint.setPos(vec2d<float>(0, 0));
        	}
	    }

	    // pathfinding stepping
		if(ptr != nullptr && !ptr->hasEnded()){
			ptr->step();
		}

	    // rendering
	    // clear screen before drawing
	    window.clearScreen();
		
		// render grid line
	    if(ptr != nullptr){
	    	// render current path
	    	tm->renderVisited(window.getRenderer(), cam, ptr->getVisitedNode());
	    	tm->renderPath(window.getRenderer(), cam, ptr->getCurrentPath());
	    }
	    tm->render(window.getRenderer(), cam);


	    if(draggingTarget){
			target.renderWithPos(window.getRenderer(), cam, cam.ScreenToWorld(vec2d<int>(mouse_x, mouse_y)) - target.getSize() / 2);
	    }else target.render(window.getRenderer(), cam);

	    if(draggingStartingPoint){
	    	startingPoint.renderWithPos(window.getRenderer(), cam, cam.ScreenToWorld(vec2d<int>(mouse_x, mouse_y)) - startingPoint.getSize() / 2);
	    }else startingPoint.render(window.getRenderer(), cam);

	    // show screen 
	    window.presentScreen();

	    SDL_Delay(2);
	}
	delete ptr;
}
