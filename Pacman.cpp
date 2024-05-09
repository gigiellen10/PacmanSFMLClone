/* Programmer name: Genevieve Kochel 
Date: April 13th 2024
File: this file contains the function definitions for the pacman class */

#include "Pacman.hpp"

// checks for up, down, left, right arrows pressed...or WASD equivalently for left handed peeps, #inclusion !!
// moves in corresponding direction as long as no collisions detected

void Pacman::movePacman(Time dt, GameMap &theMap)
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
			this->reCenter(mDirection);
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
	this->move(mSpeed * mDirection * dt.asSeconds());
	
}

bool Pacman::isWallCollision(GameMap& theMap)
{
	int RayCol = getColIndex(mRay.getEnd()), // get row and col that ray ends in
		RayRow = getRowIndex(mRay.getEnd());

	if (theMap[RayRow][RayCol].getIsPassable() == 1) // we've almost hit a wall
	{
		return true;
	}

	return false;
}


void Pacman::travelMiddlePath()
{
	float xPos = this->getPosition().x, // get current position
		  yPos = this->getPosition().y;

	int PacCol = getColIndex(this->getPosition()), // get current row and col
		PacRow = getRowIndex(this->getPosition());

	Vector2f centeredPos(xPos, yPos); // set to current position, initially

	if (mDirection == Direction::LEFT || mDirection == Direction::RIGHT) // if travelling left or right, compute center of current row
	{
		centeredPos.y = ((float)PacRow * CELL_SIZE) + (CELL_SIZE / 2); // midpt of col
		this->setPosition(centeredPos);
	}
	else if (mDirection == Direction::UP || mDirection == Direction::DOWN) // if travelling left or right, compute center of current col
	{
		centeredPos.x = ((float)PacCol * CELL_SIZE) + (CELL_SIZE / 2); // midpt of col
		this->setPosition(centeredPos);
	}

}

// shoots a ray from the object origin to edge of obj global bounds - computes the two points of ray based on origin and size of object
void Pacman::computeRayBounds()
{
	Vector2f endPt;
	float xPos = this->getPosition().x,
		yPos = this->getPosition().y;


	// calculate where to shoot ray depending on direction currently travelling

	if (mDirection == Direction::UP) // compute endpt upwards
	{
		endPt = Vector2f(xPos, yPos - COLLISION_RAY_H);
	}
	else if (mDirection == Direction::RIGHT) // compute endpt right
	{
		endPt = Vector2f(xPos + COLLISION_RAY_H, yPos);
	}
	else if (mDirection == Direction::DOWN) // compute endpt downwards
	{
		endPt = Vector2f(xPos, yPos + COLLISION_RAY_H);
	}
	else if (mDirection == Direction::LEFT) // compute endpt left
	{
		endPt = Vector2f(xPos - COLLISION_RAY_H, yPos);
	}
	
	// set start and endpt of ray
	mRay.setStart(this->getPosition()); // ray should shoot from pacman origin (center of circle)
	mRay.setEnd(endPt);

	
}

// determines if pacman is currently on an intersection point
bool Pacman::isOnIntersection(GameMap& theMap) const
{
	// determine pacman current position
	int PacCol = getColIndex(this->getPosition()),
		PacRow = getRowIndex(this->getPosition());
		
	// if pacman is in an intersection cell 
	if (theMap[PacRow][PacCol].getIsIntersection() == true)
	{
		return true;
	}

	return false;


}


// evaluates possible paths that can be travelled depending on if the entity is 
// on an intersection point. returns true for valid direction, false otherwise
bool Pacman::isValidDirection(bool onIntersection, const Vector2f desiredDirection, GameMap &theMap) const
{
	int currRow = getRowIndex(this->getPosition()), // compute pacman's current row/col
		currCol = getColIndex(this->getPosition());


	if (onIntersection) // check surrounding valid paths and compare with desiredDirection
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
		else if (desiredDirection == Direction::DOWN && // if col down is not a wall
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
		
		if (mDirection == Vector2f(-desiredDirection.x, -desiredDirection.y) || // adding a (-) reverses the direction vector
			(( this->getPosition() == Vector2f(PAC_SPAWN_X, PAC_SPAWN_Y) )
			&& (desiredDirection == Direction::LEFT || desiredDirection == Direction::RIGHT) ) )  // if at spawn point, can only move left or right!!
		{
			return true;
		}
	}

	return false;

}

// returns the row index of a position vector in the map array
int getRowIndex(Vector2f pos)
{
	return ((int)(pos.y / CELL_SIZE));
}

// returns the col index of a position vector in the map array
int getColIndex(Vector2f pos)
{
	return ((int)(pos.x / CELL_SIZE));
}

// used to recenter when he hits a corner/wall cell
void Pacman::reCenter(Vector2f& direction)
{
	Vector2f currPos = this->getPosition();
	int row = getRowIndex(currPos), col = getColIndex(currPos);
	float newX = ((float)col * 90) + 45, newY = ((float)row * 90) + 45;

	this->setPosition(newX, newY);

}

// alternates between 2 mouth states based on how many frames elapsed; adjusts texture to display accordingly
void Pacman::animateMouth(int frameCounter)
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
void Pacman::changeDirection(Vector2f& currDir, const Vector2f& newDir)
{
	Vector2f copyNewDir = newDir;

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


// checks if pacman has collided with a ghost, returns true if so, false otherwise
void Pacman::isDeath(const sf::FloatRect& ghost1Pos, const sf::FloatRect& ghost2Pos,
	const sf::FloatRect& ghost3Pos, const sf::FloatRect& ghost4Pos)
{
	if (this->getGlobalBounds().intersects(ghost1Pos) || this->getGlobalBounds().intersects(ghost2Pos) // check collisions with all 4 ghosts
		|| this->getGlobalBounds().intersects(ghost3Pos) || this->getGlobalBounds().intersects(ghost4Pos))
	{
		mIsAlive = false; // denote pacman as dead
		
	}
}



