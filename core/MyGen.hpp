#ifndef MyGen_HPP
#define MyGen_HPP

#include "MazeGenerator.hpp"
#include "Maze.hpp"
#include "Direction.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <stdlib.h>
#include <stack>
using namespace std;

class JasonPadillaGenerator: public MazeGenerator
 {

public:

	JasonPadillaGenerator();
	void generateMaze(Maze& maze);
	void CreateMazeTunnels(int x_coordinate, int y_coordinate, Maze& maze);
	Direction GetNextPossibleDirection();
	bool CanWallsBeRemoved(int cell_walls[5]);
	void UpdateDirectionAndLocation(int x_coordinate,int y_coordinate,Direction possible_direction, Direction& next_direction, int& new_x,int& new_y, bool& checking_cell,int(&cell_walls)[5]);
	bool HasLocationBeenVisited(int x_coordinate, int y_coordinate);
	void GoToPreviousCell(int& new_x, int& new_y,Maze& maze);
	void RemoveWallAndGoForward(int new_x, int new_y, int x_coordinate, int y_coordinate,Direction next_direction, Maze& maze);
	void RecursiveCallToMoveLocation(int x_coordinate, int y_coordinate, Maze& maze);

private:
	int maze_height_;
	int maze_width_;
	int num_of_visited_locations_ = 0;
	vector<pair<int,int>> visited_locations_;
	stack<pair<int,int>> trail_of_locations_;

};

#endif
