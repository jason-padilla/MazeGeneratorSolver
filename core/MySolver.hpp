#ifndef JasonPadillaSolver_HPP
#define JasonPadillaSolver_HPP

#include "MazeSolver.hpp"
#include "Maze.hpp"
#include "MazeSolution.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <stdlib.h>
#include <stack>
using namespace std;

class JasonPadillaSolver: public MazeSolver
{
	public:
		JasonPadillaSolver();
		void solveMaze(const Maze& maze, MazeSolution& mazeSolution);
		void SearchThroughMaze(int x_coordinate, int y_coordinate, const Maze& maze, MazeSolution& mazeSolution);
		bool AllDirectionsHaveBeenExplored(int cell_direction[5]);
		void ChooseNextDirection(int x_coordinate, int y_coordinate, Direction& next_direction, int& new_x, int& new_y, bool& checking_directions, int(&cell_directions)[5],const Maze& maze);
		bool HasBeenVisited(int x_coordinate, int y_coordinate);
		void GoToPreviousCell(int& new_x, int& new_y,const Maze& maze,MazeSolution& mazeSolution);
		void GoForward(int new_x,int new_y,int x_coordinate,int y_coordinate,Direction next_direction,const Maze& maze,MazeSolution& mazeSolution);
		void RecursiveCallToExtendSolution(int x_coordinate, int y_coordinate, const Maze& maze, MazeSolution& mazeSolution);
	
	private:
		int maze_height_;
		int maze_width_;
		vector<pair<int,int>> visited_locations_;
		stack<pair<int,int>> trail_of_locations_;                                                                      
};

#endif