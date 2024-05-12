/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#include "Ghost.hpp"


void Ghost::movement(Time dt, GameMap& theMap, Vector2f& pacTile, Vector2f& pacDir, Vector2f& blinkyPos)
{
	bool isIntersection = this->isOnIntersection(theMap);
	Vector2f targetCoords;

	if (isIntersection)
	{
		// set new ray bounds 
		this->computeRayBounds();

		//  will hit a wall if continues, so recenter
		if (isWallCollision(theMap))
		{
			// should pick a valid direction before moving further
			this->reCenter();
			/*mSpeed = 0.f; see what this looks like, if necessary */
		}

		// evaluate target tile
		targetCoords = findTargetTile(pacTile, pacDir, blinkyPos);

		// find direction of shortest path to target tile

		// set ghost direction 
	}
	

	

	// move ghost to target tile 
	// may need condition for if ghost already on target tile or has hit wall before reaching it, ex: pacman is not moving  

	
}

// accepts vector containing pac's row and col tile, pac's dir and blinky's position vector
Vector2f& Ghost::findTargetTile(Vector2f& pacTile, Vector2f& pacDir, Vector2f& blinkyPos)
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
			target = calcDirVectorTile(pacTile, pacDir, blinkyPos);
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

const Vector2f& Ghost::calcDirVectorTile(const Vector2f& pacPos, const Vector2f& pacDir, const Vector2f& blinkyPos)
{
	// calculate offset tile - 2 tiles in front of pac
	Vector2f offset((2.f * pacDir).x + pacPos.x, (2.f * pacDir).y + pacPos.y);

	// compute length of difference of offset vector from blinky's position
	float scaleLength = length(blinkyPos - offset);

	Vector2f unitVect(blinkyPos / length(blinkyPos)); // normalize blinky vector into unit vector

	return unitVect * scaleLength;
}
