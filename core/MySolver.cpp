
#include "MySolver.hpp"
#include "Direction.hpp"
#include "MazeSolution.hpp"
#include "Maze.hpp"
#include <ics46/factory/DynamicFactory.hpp> 

ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver,JasonPadillaSolver,"JasonPadillaMazeSolver(Required)");

JasonPadillaSolver::JasonPadillaSolver()
{

}

void JasonPadillaSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
	//Creates the GUI walls
	maze_height_ = maze.getHeight();
	maze_width_ = maze.getWidth();
	//adds the current cell to the visited locations
	visited_locations_.push_back(make_pair(0,0));
	//Begins the searching through the maze
	SearchThroughMaze(0, 0, maze,mazeSolution);
}

void JasonPadillaSolver::SearchThroughMaze(int x_coordinate, int y_coordinate, const Maze& maze, MazeSolution& mazeSolution)
{
	/*
		This the main function that searches through the maze to find the solution from the
		top left corner to the bottom right corner. It does so by checking all the directions of the
		cell that it is currently. If the cell has a direction that can be explored than it will 
		go towards that direction else the solution finder will back up until it finds a new path.
	*/

	int new_x;
	int new_y;
	Direction next_direction;
	bool checking_directions = true;
	int cell_directions[5] = {0,0,0,0};
	bool go_back = false;

	//Checks each direction; if 1 direction can be explored than it will end the loop
	while (checking_directions)
	{
		//This means that it has reached a dead end in the maze and has to go back
		if(AllDirectionsHaveBeenExplored(cell_directions))
		{
			go_back = true;
			break;
		}
		else
		{
			//Will check each direction and fully explore it until all directions have beenn explored
			ChooseNextDirection(x_coordinate, y_coordinate,next_direction,new_x,new_y,checking_directions,cell_directions,maze);
		}
	}

	if (go_back)
	{
		//Goes back one cell and then calls a function that will recursively call SearchThroughMaze
		GoToPreviousCell(new_x,new_y,maze,mazeSolution);
	}
	else
	{
		//Goes forward one cell and then calls a function that will recursively call SearchThroughMaze
		GoForward(new_x,new_y,x_coordinate,y_coordinate,next_direction,maze,mazeSolution);
	}
}

bool JasonPadillaSolver::AllDirectionsHaveBeenExplored(int cell_direction[5])
{
	/*
		This function checks to see if the current cell still has directions that can be explored.
		The function takes in an array of ints {0(up),0(right),0(down),0(left)}, each zero 
		indicates that the direction has not been explored. Once explores all four directions 
		the value of all the directions will be 1 which will then indicate that no more directions can be explored.
	*/

	if((cell_direction[0] *cell_direction[1] *cell_direction[2] *cell_direction[3]) == 1)
		return true;
	else
		return false;
}

void JasonPadillaSolver::ChooseNextDirection(int x_coordinate, int y_coordinate, Direction& next_direction, int& new_x, int& new_y, bool& checking_directions, int(&cell_directions)[5],const Maze& maze)
{
	/*
		This function checks the surrounding directions to see if there is a wall. If there is not a wall than 
		it will proceed to make that its next direction.If all the directions have been visited than that means 
		that the solution isnt in that direction and it will have to go back to find anmother path. 
	*/

	if(maze.wallExists(x_coordinate,y_coordinate,Direction::up) == 0 and HasBeenVisited(x_coordinate,y_coordinate-1) == false)
	{
		checking_directions = false;
		next_direction = Direction::up;
		new_x = x_coordinate;
		new_y = y_coordinate -1;
	}

	else if(maze.wallExists(x_coordinate,y_coordinate,Direction::right) == 0 and HasBeenVisited(x_coordinate+1,y_coordinate) == false)
	{
		checking_directions = false;
		next_direction = Direction::right;
		new_x = x_coordinate+1;
		new_y = y_coordinate;
	}

	else if(maze.wallExists(x_coordinate,y_coordinate,Direction::down) == 0 and HasBeenVisited(x_coordinate,y_coordinate+1) == false)
	{
		checking_directions = false;
		next_direction = Direction::down;
		new_y = y_coordinate+1;
		new_x = x_coordinate;	
	}

	else if(maze.wallExists(x_coordinate,y_coordinate,Direction::left) == 0 and HasBeenVisited(x_coordinate-1,y_coordinate) == false)
	{
		checking_directions = false;
		next_direction = Direction::left;
		new_x = x_coordinate -1;
		new_y = y_coordinate;			
	}
	
	else
	{
		cell_directions[0] = 1;
	    cell_directions[1] = 1;
		cell_directions[2] = 1;
		cell_directions[3] = 1;
	}
}

bool JasonPadillaSolver::HasBeenVisited(int x_coordinate, int y_coordinate)
{
	/*
		This function takes in the coordinates of the next location and returns if it has
		or has not been visited. It does so by looking through the vector of visited locations 
		and making sure that the current pair<int,int> of coordinates is not in the vector.
		If location has been visited than the wall in that direction will not be removed.
	*/

	pair<int,int> next_direction;
	next_direction.first = x_coordinate;
	next_direction.second = y_coordinate;
	if(find(visited_locations_.begin(),visited_locations_.end(),next_direction)!=visited_locations_.end())
	{
		return true;
	}
	else
		return false;
}

void JasonPadillaSolver::GoToPreviousCell(int& new_x, int& new_y,const Maze& maze, MazeSolution& mazeSolution)
{
	/*
		This function will make the previous location the new location by looking at
		the trail of locations that have been visited. It will then remove the current 
		location from the trail as to indicate that the path has been fully explored.
		And then it will have the line backup.  
	*/

	trail_of_locations_.pop();
	new_x = trail_of_locations_.top().first;
	new_y = trail_of_locations_.top().second;
	mazeSolution.backUp();

	RecursiveCallToExtendSolution(new_x,new_y,maze,mazeSolution);
}

void JasonPadillaSolver::GoForward(int new_x,int new_y,int x_coordinate,int y_coordinate,Direction next_direction,const Maze& maze, MazeSolution& mazeSolution)
{
	/*
		This function will extend the line displaying the path that leads to the solution of the maze. 
		It also adds the current location to the array of visited locations and to the trail of current locations. 
	*/

	visited_locations_.push_back(make_pair(new_x,new_y));
	trail_of_locations_.push(make_pair(new_x,new_y));

	mazeSolution.extend(next_direction);
	RecursiveCallToExtendSolution(new_x,new_y,maze,mazeSolution);
}

void JasonPadillaSolver::RecursiveCallToExtendSolution(int x_coordinate, int y_coordinate, const Maze& maze, MazeSolution& mazeSolution)
{
	/*
		This function is the recursive call to keep searching throughout the maze. It also checks to see if 
		the current location is at the bottom right corner of the maze.If it is then that means it has found 
		the solution to the maze and therefore can end.
	*/

	//End of maze can be found at [(maze_width_ -1),(maze_height_-1)]
	if(x_coordinate == (maze_width_ -1) and y_coordinate == (maze_height_-1))
	{
		return;	
	}
	else
	{
		SearchThroughMaze(x_coordinate,y_coordinate, maze, mazeSolution);
	}

}