/* Programmer name: Genevieve Kochel 
Date: April 13th 2024
File: this file contains the function definitions for the pacman class */

#include "Pacman.hpp"

void Pacman::movement(Time dt, GameMap &theMap)
{

	// determine if pacman on an intersection
	bool onIntersection = isOnIntersection(theMap);

	// set speed back to standard speed if not currently at spawn point

		mSpeed = 300.f;
	
	/* Wall collision detection */

	// if pacman is on intersection = close to a wall, so compute ray bounds to see when he hits the wall
	if (onIntersection)
	{
		// set new ray bounds 
		this->computeRayBounds();

		// pacman will hit a wall if he continues, so stop and recenter
		if (isWallCollision(theMap))
		{
			// pac should pick a valid direction before moving further
			this->reCenter();
			mSpeed = 0.f;
		}

	}

	/* User keyboard input movement */

	// validate that no collisions will occur if player selected new direction
	if (sf::Keyboard::isKeyPressed(Keyboard::Left) 
		|| Keyboard::isKeyPressed(Keyboard::A))
	{
		

		if (this->isValidDirection(onIntersection, Direction::LEFT, theMap))
		{
			// pacman can go this direction! 
			this->changeDirection(mDirection, Direction::LEFT); // change direction facing
			mDirection = Direction::LEFT;
		}

		
		// else: he will continue moving in same direction he currently is moving! 
		
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right)
			|| Keyboard::isKeyPressed(Keyboard::D))
	{

		if (this->isValidDirection(onIntersection, Direction::RIGHT, theMap))
		{
			this->changeDirection(mDirection, Direction::RIGHT); // change direction facing
			// pacman can go this direction! 
			mDirection = Direction::RIGHT;
		}
		

	}
	else if (Keyboard::isKeyPressed(Keyboard::Up) 
			|| Keyboard::isKeyPressed(Keyboard::W))
	{

		if (this->isValidDirection(onIntersection, Direction::UP, theMap))
		{
			this->changeDirection(mDirection, Direction::UP); // change direction facing
			// pacman can go this direction!
			mDirection = Direction::UP;
		}
		

	}
	else if (Keyboard::isKeyPressed(Keyboard::Down) 
			|| Keyboard::isKeyPressed(Keyboard::S))
	{

		if (this->isValidDirection(onIntersection, Direction::DOWN, theMap))
		{
			this->changeDirection(mDirection, Direction::DOWN); // change direction facing
			// pacman can go this direction! 
			mDirection = Direction::DOWN;
		}

	}

	
	this->travelMiddlePath(); // ensure that pacman is traveling in the middle of a path/can't cut corners 

	// apply movement 
	this->move(mSpeed * static_cast<Vector2f>(mDirection) * dt.asSeconds());
	
}


// evaluates possible paths that can be travelled depending on if the entity is 
// on an intersection point. returns true for valid direction, false otherwise
bool Pacman::isValidDirection(bool onIntersection, const Vector2i desiredDirection, GameMap &theMap) const
{
	int currRow = getRowIndex(this->getPosition()), // compute pacman's current row/col
		currCol = getColIndex(this->getPosition());


	if (onIntersection && 
		!(currRow == 3 && (currCol == 10 || currCol == 11))) // check surrounding valid paths and compare with desiredDirection
	{
		// note: could take all these if statements and add to one large compound conditional statement since they all return true. 
		// I thought this organization was more readable this way, as each direction scenario is separated

		if (desiredDirection == Direction::LEFT && // if col to left is not a wall 
			theMap[currRow][currCol - 1].getIsPassable() != 1)
		{
			return true;
		}
		else if (desiredDirection == Direction::RIGHT && // if col to right is not a wall 
			theMap[currRow][currCol + 1].getIsPassable() != 1)
		{
			return true;
		}
		else if (desiredDirection == Direction::DOWN && // if col down is not a wall or prison cell
			theMap[currRow + 1][currCol].getIsPassable() != 1)
		{
			return true;
		}
		else if (desiredDirection == Direction::UP && // if col up is not a wall 
			theMap[currRow - 1][currCol].getIsPassable() != 1)
		{
			return true;
		}

	}
	else // not on an intersection point, the only two valid directions are forward/back or up/down, respectively
	{
		// can only make a complete 180 turn/opposite direction of current direction
		
		if (mDirection == Vector2i(-desiredDirection.x, -desiredDirection.y) || // adding a (-) reverses the direction vector
			(( this->getPosition() == Vector2f(PAC_SPAWN_X, PAC_SPAWN_Y) )
			&& (desiredDirection == Direction::LEFT || desiredDirection == Direction::RIGHT) ) )  // if at spawn point, can only move left or right!!
		{
			return true;
		}
	}

	return false;

}


// OVERRIDING CHARACTER PURE VIRTUAL FUNCT!! 
// purpose: alternates between 2 mouth states based on how many frames elapsed; adjusts texture to display accordingly
void Pacman::animate(int frameCounter)
{
	if (mSpeed != 0.f && this->getPosition() != Vector2f(PAC_SPAWN_X, PAC_SPAWN_Y)) // if pacman is actively moving and not at the spawn point
	{
		if (frameCounter % 8 < 4) // alternate between open and closed mouth states
		{
			this->setTextureRect(sf::IntRect(250, 0, 200, 200));
		}
		else
		{
			this->setTextureRect(sf::IntRect(460, 0, 180, 200));
		}
	}
	
}

// computes the degrees to rotate pacman depending on current and desired direction 
void Pacman::changeDirection(Vector2i& currDir, const Vector2i& newDir)
{

	// if changing direction
	if (currDir != newDir)
	{
		// 180 degree turn
		if (currDir == -newDir || (currDir == Direction::STOP && newDir == Direction::LEFT))
		{
			this->rotate(180); // rotate 180 deg
		}
		else if ( (currDir == Direction::LEFT && newDir == Direction::DOWN) || // all cases of 270 degree turns
			(currDir == Direction::DOWN && newDir == Direction::RIGHT) ||
			(currDir == Direction::RIGHT && newDir == Direction::UP) ||
			(currDir == Direction::UP && newDir == Direction::LEFT) )
		{
			this->rotate(270);
		}
		else if (currDir != Direction::STOP) // 90 degree rotation
		{
			this->rotate(90);
		}

	}
	
}




