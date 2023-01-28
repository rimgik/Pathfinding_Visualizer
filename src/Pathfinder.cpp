#include "Pathfinder.hpp"

Pathfinder::Pathfinder(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos){
	// _vis = std::vector<std::vector<bool>> (size.x, std::vector<bool>(size.y, false));
	_vis = wall;
	_parent = std::vector<std::vector<vec2d<int>>> (size.x, std::vector<vec2d<int>> (size.y));
	_startPos = startPos;
	_endPos = endPos;
	_size = size;
	_ended = false;
	_targetFound = false;
	_parent[_startPos.x][_startPos.y] = vec2d<int>(-1, -1);
}

Pathfinder::~Pathfinder(){

}

const vec2d<int>& Pathfinder::getCurrentNode(){
	return _currNode;
}

const std::vector<std::vector<bool>>& Pathfinder::getVisitedNode(){
	return _vis;
}

const bool& Pathfinder::hasEnded(){
	return _ended;
}

const bool& Pathfinder::targetIsFound(){
	return _targetFound;
}

std::unique_ptr<std::vector<vec2d<int>>> Pathfinder::getCurrentPath(){
	std::unique_ptr<std::vector<vec2d<int>>> ptr = std::make_unique<std::vector<vec2d<int>>>();
	auto tmp = _currNode;
	while(tmp != vec2d<int>(-1, -1)){
		ptr->emplace_back(tmp);
		tmp = _parent[tmp.x][tmp.y];
	}	
	return ptr;
}

Dfs::Dfs(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos) 
	: Pathfinder(wall, size, startPos, endPos){
		_local_vis = wall;
		_currNum = 0;
		run(_startPos);
}

bool Dfs::step(){
	if(_targetFound)return 1;

	if(_currNum >= _order.size()){
		_ended = 1;
		_targetFound = 0;
		return 0;
	}

	if(_order[_currNum] == _endPos){
		_ended = 1;
		_targetFound = 1;
		_currNode = _order[_currNum];
		return 1;
	}

	_vis[_order[_currNum].x][_order[_currNum].y] = 1;
	_currNode = _order[_currNum++];
	return 0;
}

bool Dfs::run(vec2d<int> v){
	_local_vis[v.x][v.y] = 1;
	_order.push_back(v);
	if(v == _endPos){
		return 1;
	}
	for(int i = 0; i < 4; ++i){
		vec2d<int> tmp = v + vec2d<int>(dx[i], dy[i]);
		if(tmp.x >= 0 && tmp.x < _size.x && tmp.y >= 0 && tmp.y < _size.y && !_local_vis[tmp.x][tmp.y]){
			_parent[tmp.x][tmp.y] = v;
			if(run(tmp))return 1;
		}
	}
	return 0;
}

Bfs::Bfs(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos) 
	: Pathfinder(wall, size, startPos, endPos){
		q.push(startPos);
}

bool Bfs::step(){
	//running the bfs algorithm
	if(!q.empty() && !_ended){
		auto f = q.front();
		q.pop();
		_currNode = f;
		if(_vis[f.x][f.y]){
			return targetIsFound();
		}
		_vis[f.x][f.y] = true;
		if(f == _endPos){
			_ended = true;
			_targetFound = true;
			return 1;
		}
		for(int i = 0; i < 4; ++i){
			vec2d<int> tmp = f + vec2d<int>(dx[i], dy[i]);
			if(tmp.x >= 0 && tmp.x < _size.x && tmp.y >= 0 && tmp.y < _size.y && !_vis[tmp.x][tmp.y]){
				_parent[tmp.x][tmp.y] = f;
				q.emplace(tmp);
			}
		}
	}else if(q.empty()){
		_ended = true;
	}
	return targetIsFound();
}

AStar::AStar(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos) 
	: Pathfinder(wall, size, startPos, endPos){
		pq.push({0, {0, _startPos}});
		_dist = std::vector<std::vector<int>> (size.x, std::vector<int>(size.y, 0x3f3f3f3f));
}

bool AStar::step(){
	if(!pq.empty() && !_ended){
		auto f = pq.top();
		pq.pop();	
		f.first *= -1;
		int x = f.second.second.x;
		int y = f.second.second.y;
		
		if(f.second.second == _endPos){
			_dist[x][y] = f.second.first;
			_vis[x][y] = 1;
			_ended = 1;
			_targetFound = 1;
			_currNode = _endPos;
			return 1;
		}else if(_vis[x][y]){
			step();
			return targetIsFound();
		}

		_currNode = f.second.second;
		_dist[x][y] = f.second.first;
		_vis[x][y] = 1;

		for(int i = 0;i < 4;++i){
			int xx = x + dx[i];
			int yy = y + dy[i];

			// restrict area of searching
			if(xx >= 0 && yy >= 0 && xx < _size.x && yy < _size.y && !_vis[xx][yy]){
				int cost = vec2d<int>(xx, yy).Manhattan(_endPos);
				if(_dist[xx][yy] > f.second.first + cost + 1){
					_dist[xx][yy] = f.second.first + cost + 1;
					_parent[xx][yy] = vec2d<int>(x, y);
				}
				pq.push({-f.second.first - 1 - cost, {f.second.first + 1, vec2d<int>(xx, yy)}});
			}
		}
	}else if(pq.empty()){
		_ended = 1;
	}
	return targetIsFound();
}