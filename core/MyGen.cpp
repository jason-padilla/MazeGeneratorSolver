#include <ics46/factory/DynamicFactory.hpp>
#include "MyGen.hpp"
#include <list>

using namespace std;
ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator,JasonPadillaGenerator,"JasonPadillaGenerator(Required)");


JasonPadillaGenerator::JasonPadillaGenerator()
{

}

void JasonPadillaGenerator::generateMaze(Maze& maze)
{
	//Creates the GUI walls
	maze.addAllWalls();
	maze_height_ = maze.getHeight();
	maze_width_ = maze.getWidth();

	//Starts the location count at 1		
	num_of_visited_locations_ = 1;
	//Adds the starting location to the locations visited
	visited_locations_.push_back(make_pair(0,0));

	//Begins to make the tunnels that make up the maze
	CreateMazeTunnels(0,0,maze);
	
}

void JasonPadillaGenerator::CreateMazeTunnels(int x_coordinate, int y_coordinate, Maze& maze)
{
	/*
		This is the main function that creates the tunnels that make up the maze. It will start by 
		checking to see if the current cell can remove any cells. It will then choose a wall to remove,
		if 3 or less walls have been removed than it will update the location and move forward. If 3 walls 
		have been removed than it will choose to move back one cell. The function calls itself recursively
		at the end within another function and checks to see if the all the cells have been visited, if not 
		then it will keeping moving forward or backward.
	*/

	int new_x;
	int new_y;
	Direction next_direction;
	bool checking_cell = true;
	int cell_walls[5] = {0,0,0,0};
	bool go_back = false;

	//Checks each wall of a cell; once 1 or all walls have been checked than it will loop out
	while (checking_cell)
	{
		//Gets a random direction to go to
		Direction possible_direction = GetNextPossibleDirection();
		//Walls can be removed as long as no more than 3 have already been removed
		if (CanWallsBeRemoved(cell_walls))
			UpdateDirectionAndLocation(x_coordinate,y_coordinate,possible_direction,next_direction,new_x,new_y,checking_cell,cell_walls);
		//This indicates that 3 walls have already been removed and therefore we should go back by one cell
		else
		{
			go_back = true;
			break;
		}
	}

	if (go_back)
	{
		//Goes back one cell and then calls a function that will recursively call CreateMazeTunnels
		GoToPreviousCell(new_x,new_y,maze);
	}
	else
	{
		//Goes forward one cell and then calls a function that will recursively call CreateMazeTunnels
		RemoveWallAndGoForward(new_x,new_y,x_coordinate,y_coordinate,next_direction,maze);
	}

}

Direction JasonPadillaGenerator::GetNextPossibleDirection()
{
	/*
		This function returns the next Direction to go to by first assigning a value (0-3) 
		to a certain Direction and then randomly selecting a number.
		up = 0, right = 1, down = 2, left = 3 
	*/

	Direction next_direction;
	int random_num = rand()%4;

	if (random_num == 0)
	{
		next_direction = Direction::up;
	}
	
	if (random_num == 1)
	{
		next_direction = Direction::right;
	}

	if (random_num == 2)
	{
		
		next_direction = Direction::down;
	}

	if (random_num == 3)
	{
		
		next_direction = Direction::left;
	}

	return next_direction;	
}

void JasonPadillaGenerator::UpdateDirectionAndLocation(int x_coordinate,int y_coordinate,Direction possible_direction, Direction& next_direction, int& new_x,int& new_y, bool& checking_cell,int(&cell_walls)[5])
{
	/*
		This function takes in a possible direction that indicates what wall to remove from the current location.
		If the next direction has not already been visited than that wall can be removed.
	*/
		if (possible_direction == Direction::up)
		{
			if(HasLocationBeenVisited(x_coordinate,y_coordinate-1) == false and y_coordinate != 0)
			{
				checking_cell = false;
				next_direction = Direction::up;
				new_x = x_coordinate;
				new_y = y_coordinate - 1;
				num_of_visited_locations_++;
			}

			cell_walls[0] = 1;
		}
		else if (possible_direction == Direction::right)
		{
			if(HasLocationBeenVisited(x_coordinate+1,y_coordinate) == false and x_coordinate != (maze_width_ - 1))
			{
				checking_cell = false;
				next_direction = Direction::right;
				new_x = x_coordinate + 1;
				new_y = y_coordinate;
				num_of_visited_locations_++;
			}

			cell_walls[1] = 1;
		}
		else if (possible_direction == Direction::down)
		{
			if(HasLocationBeenVisited(x_coordinate,y_coordinate + 1) == false and y_coordinate != (maze_height_ - 1) )
			{
				checking_cell = false;
				next_direction = Direction::down;
				new_y = y_coordinate + 1;
				new_x = x_coordinate;
				num_of_visited_locations_++;
			}

			cell_walls[2] = 1;
		}
		else if (possible_direction == Direction::left)
		{
			if(HasLocationBeenVisited(x_coordinate - 1,y_coordinate) == false and x_coordinate != 0)
			{
				checking_cell = false;
				next_direction = Direction::left;
				new_x = x_coordinate - 1;
				new_y = y_coordinate;
				num_of_visited_locations_++;
			}

			cell_walls[3] = 1;
		}
}

bool JasonPadillaGenerator::HasLocationBeenVisited(int x_coordinate, int y_coordinate)
{
	/*
		This function takes in the coordinates of the next location and returns if it has
		or has not been visited. It does so by looking through the vector of visited locations 
		and making sure that the current pair<int,int> of coordinates is not in the vector.
		If location has been visited than the wall in that direction will not be removed.
	*/

	pair<int,int> next_location;
	next_location.first = x_coordinate;
	next_location.second = y_coordinate;

	if(find(visited_locations_.begin(),visited_locations_.end(),next_location) != visited_locations_.end())
		return true;

	else
		return false;
}

bool JasonPadillaGenerator::CanWallsBeRemoved(int cell_walls[5])
{
	/*
		This function checks to see if the current cell still has walls that can be removed.
		The function takes in an array of ints {0(up),0(right),0(down),0(left)}, each zero 
		indicates that the wall has not been removed. No more than 3 walls can be removed 
		from each cell. Once it tries to remove the fourth wall the value of all the walls 
		will be 1 which will then indicate that no more walls can be removed.
	*/

		//Will only equal 1 if all the values are 1 else it will be 0
		if((cell_walls[0] *cell_walls[1] *cell_walls[2] *cell_walls[3]) == 1)
			return false;
		else
			return true;
}

void JasonPadillaGenerator::RemoveWallAndGoForward(int new_x, int new_y, int x_coordinate,int y_coordinate,Direction next_direction, Maze& maze)
{
	/*
		This function will remove the wall from the chosen wall from the current location. 
		It will then update the locations that it has visited and the location it is currently at.
		Then it will make the call to check if there are more locations to visit or is the maze complete.
	*/

	maze.removeWall(x_coordinate,y_coordinate,next_direction);
	
	visited_locations_.push_back(make_pair(new_x,new_y));
	trail_of_locations_.push(make_pair(new_x,new_y));

	
	RecursiveCallToMoveLocation(new_x,new_y,maze);		
	
}

void JasonPadillaGenerator::GoToPreviousCell(int& new_x, int& new_y,Maze& maze)
{
	/*
		This function will make the previous location the new location by looking at
		the trail of locations that have been visited. It will then remove the current 
		location from the trail as to indicate that the path has been fully explored.  
	*/

	new_x = trail_of_locations_.top().first;
	new_y = trail_of_locations_.top().second;
	trail_of_locations_.pop();
	RecursiveCallToMoveLocation(new_x,new_y,maze);
	
}

void JasonPadillaGenerator::RecursiveCallToMoveLocation(int x_coordinate, int y_coordinate, Maze& maze)
{
	/*
		This function checks to see if the number of visited locations is equal to the amount of cells
		are in the maze. If they are equal than the square is done. Else call the making tunnel function 
		again keep making tunnels either by moving forward or backward.
	*/
	
	if(num_of_visited_locations_ == maze_width_*maze_width_)
		return;	
	else
		CreateMazeTunnels(x_coordinate,y_coordinate,maze);
}