#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <vector>
#include <SDL.h>
#include <ctime>

const int dir = 8; // number of possible directions to go at any position
// if dir==4
//static int dx[dir]={1, 0, -1, 0};
//static int dy[dir]={0, 1, 0, -1};
// if dir==8

class Node;

class AStar
{
public:
	AStar();
	~AStar();

	void RunProgram(int _w, int _h, std::vector<int> _mapData);

	// A-star algorithm, The route returned is a string of direction digits.
	std::string pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish);

	void Present(int _w, int _h);

private:

	int n = 20; // horizontal size of the map
	int m = 20; // vertical size size of the map
	int map[20][20];
	int closed_nodes_map[20][20]; // map of closed (tried-out) nodes
	int open_nodes_map[20][20]; // map of open (not-yet-tried) nodes
	int dir_map[20][20]; // map of directions
	int dx[8];
	int dy[8];

	int xStart;
	int yStart;
	int xFinish;
	int yFinish;
};
#endif