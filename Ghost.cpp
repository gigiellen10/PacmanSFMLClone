/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#include "Ghost.hpp"


void Ghost::movement(Time dt, GameMap& theMap, const Vector2f& pacTile, const Vector2f& pacDir, const Vector2f& blinkyPos)
{
	bool isIntersection = this->isOnIntersection(theMap);
	Vector2f targetCoords, newDir;

	if (this->inPrisonBox(theMap)) // check if in prison cell, if so, execute escape sequence
	{

	}

	if (isIntersection)
	{
		// set new ray bounds 
		this->computeRayBounds();

		// evaluate target tile
		targetCoords = findTargetTile(pacTile, pacDir, blinkyPos);

		//  will hit a wall if continues, so recenter
		if (isWallCollision(theMap))
		{
			// should pick a valid direction before moving further
			this->reCenter();
			mSpeed = 0.f; /*see what this looks like, if necessary IF ON TARGET TILE????? */
		}

		// find direction of shortest path to target tile
		newDir = findOptimalPath(targetCoords, theMap); // pass in coordinates to minimize or maximize distance from 

		// set ghost direction 
		mDirection = newDir;
	}

	/*this->travelMiddlePath(); may need this? */
	
	this->move(mSpeed * mDirection * dt.asSeconds());
	
}

// accepts vector containing pac's row and col tile, pac's dir and blinky's position vector
Vector2f Ghost::findTargetTile(const Vector2f& pacTile, const Vector2f& pacDir, const Vector2f& blinkyPos)
{
	Vector2f target;

	if (mMode == 1) // chase
	{
		if (mAIType == 1) // 4 tiles in front of pacman
		{
			target = pacTile + (4.f * pacDir); 
		}
		else if (mAIType == 3 || mAIType == 2) // pac's tile
		{
			target = pacTile;
		}
		else // AI type = 4, vector through offset tile and through blinky 
		{
			target = calcInkyTarget(pacTile, pacDir, blinkyPos);
		}
	}
	else // mode = scatter
	{
		if (mAIType == 1)
		{
			target = Vector2f(2, 2); // upper left corner
		}
		else if (mAIType == 2)
		{
			target = Vector2f(21, 2); // upper right corner
		}
		else if (mAIType == 3)
		{
			target = Vector2f(2, 10); // lower left corner
		}
		else // AI type = 4
		{
			target = Vector2f(21, 10); // lower right corner
		}
	}

	return target;
	
}

const Vector2f Ghost::calcInkyTarget(const Vector2f& pacPos, const Vector2f& pacDir, const Vector2f& blinkyPos)
{
	// calculate offset tile - 2 tiles in front of pac
	Vector2f offset((2.f * pacDir).x + pacPos.x, (2.f * pacDir).y + pacPos.y);

	// compute length of difference of offset vector from blinky's position
	float scaleLength = length(blinkyPos, offset);

	Vector2f unitVect(blinkyPos / sqrt(pow(blinkyPos.x, 2) + pow(blinkyPos.y, 2)) ); // normalize blinky vector by dividing by length

	return unitVect * scaleLength;
}

// returns direction to minimize or maximize the distance between the target and the ghost
Vector2f Ghost::findOptimalPath(Vector2f& targetPos, GameMap& theMap)
{
	// calculate x and y of ghost tile
	int ghostRow = getRowIndex(getPosition()),
		ghostCol = getColIndex(getPosition());

	Vector2f directions[4] = { Direction::DOWN, Direction::UP, Direction::LEFT, Direction::RIGHT }; // easily evaluate each direction
		

	// determine if maximizing or minimizing distance based on ghost mode - chase, scatter or frightened
	if (mMode == 1 || mMode == 2) // minimizing distance
	{
		Vector2f minDirection = Direction::STOP;
		float minVectorLength = 1000.f, currVectLength = 0.f; // set high initially so first vector length becomes min

		for (int i = 0; i < 4; ++i)
		{
			// compute distance between next tile from ghost position in given direction and target tile
			currVectLength = length(Vector2f( (float)ghostCol, (float)ghostRow ) + directions[i], targetPos);

			if (currVectLength < minVectorLength) // found new min dir
				minDirection = directions[i];

		}

		return minDirection;
	}
	else // frightened mode = 3, maximizing distance
	{
		Vector2f maxDirection = Direction::STOP;
		float maxVectorLength = 0.f, currVectLength = 0.f; // set low initially so first vector length becomes new max

		for (int i = 0; i < 4; ++i)
		{
			// compute distance between next tile from ghost position in given direction and target tile
			currVectLength = length(Vector2f( (float)ghostCol, (float)ghostRow ) + directions[i], targetPos);

			if (currVectLength > maxVectorLength) // found new max 
				maxDirection = directions[i];

		}

		return maxDirection;
	}
	
}

// determines if the ghost is currently in the starting prison box
bool Ghost::inPrisonBox(GameMap& theMap)
{
	int ghostRow = getRowIndex(this->getPosition()),
		ghostCol = getColIndex(this->getPosition());

	return theMap[ghostRow][ghostCol].getIsPassable() == 3; // if the passable value of the cell is 3
}
