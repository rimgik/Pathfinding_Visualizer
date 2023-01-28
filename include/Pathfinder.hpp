#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <iostream>

#include "vec2d.hpp"

class Pathfinder{
public:
	Pathfinder(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos);	
	virtual ~Pathfinder();

	// Return current path for visualization
	virtual std::unique_ptr<std::vector<vec2d<int>>> getCurrentPath() final;

	virtual const vec2d<int>& getCurrentNode() final;

	// pathfinding stepping
	virtual bool step() = 0;

	// check if pathfinding has ended
	virtual const bool& hasEnded() final;
	// check if target has been found
	virtual const bool& targetIsFound() final;

	virtual const std::vector<std::vector<bool>>& getVisitedNode() final;

protected:
	const int dx[4] = {0, 0, -1, 1};
	const int dy[4] = {1, -1, 0, 0};

	vec2d<int> _currNode;
	bool _targetFound;
	bool _ended;
	vec2d<int> _size;
	std::vector<std::vector<bool>> _vis;
	std::vector<std::vector<vec2d<int>>> _parent;
	vec2d<int> _startPos;
	vec2d<int> _endPos;
};

class Dfs: public Pathfinder{
public:
	Dfs(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos);
	bool step() override;	

private:
	int _currNum;
	std::deque<vec2d<int>> _order;
	std::vector<std::vector<bool>> _local_vis;


	bool run(vec2d<int> v);
};

class Bfs: public Pathfinder{
public:

	Bfs(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos);
	bool step() override;

private:
	std::queue<vec2d<int>> q;
};

class cmp{
public:
	bool operator () (const std::pair<int, std::pair<int, vec2d<int>>>& a, const std::pair<int, std::pair<int, vec2d<int>>>& b){
		return a.first < b.first;
	}
};

class AStar: public Pathfinder{
public:
	AStar(const std::vector<std::vector<bool>>& wall, const vec2d<int>& size, const vec2d<int>& startPos, const vec2d<int>& endPos);

	bool step() override;
private:
	std::priority_queue<
		std::pair<int, std::pair<int, vec2d<int>>>,
		std::vector<std::pair<int, std::pair<int, vec2d<int>>>>,
		cmp> pq;

	std::vector<std::vector<int>> _dist;
};