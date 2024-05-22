/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#include "Ghost.hpp"

void Ghost::update(Time dt, const Clock& prisonClock, GameMap& theMap, const Vector2i& pacTile, 
	const Vector2i& pacDir, const Vector2i& blinkyPos)
{
	// determine if ghost can leave the prison initially, set speed to move
	if (prisonClock.getElapsedTime().asSeconds() >= mPrisonDelay
		&& mSpeed == 0.f) // prevent unnecessary speed assignments
	{
		mSpeed = GHOST_SPEED; // establish speed = 175.f
	}

		Vector2i ghostPos(getColIndex(getPosition()), getRowIndex(getPosition())),
			newDir; // direction that ghost should travel to get to target tile

		// determine if ghost on an intersection
		bool onIntersection = isOnIntersection(theMap);

		mTarget = findTargetTile(pacTile, pacDir, blinkyPos, theMap); // for pinpointing ghost target tile

		// set new ray bounds 
		this->computeRayBounds();

		// pacman will hit a wall if he continues, so stop and recenter
		if (isWallCollision(theMap))
		{
			// should pick a valid direction before moving further
			this->reCenter();

			// find direction of shortest path to target tile
			newDir = findOptimalPath(theMap); // pass in coordinates to minimize or maximize distance from 

			// mark current tile as evaluated
			mLastTileEval = ghostPos;

			// set ghost direction 
			mDirection = newDir;
		}

		// if haven't already evaluated curr tile and on intersection or prison cell
		if (mLastTileEval != ghostPos && (onIntersection || inPrisonBox(theMap)))
		{
			// find direction of shortest path to target tile
			newDir = findOptimalPath(theMap); // pass in coordinates to minimize or maximize distance from 

			// mark current tile as evaluated
			mLastTileEval = ghostPos;

			// set ghost direction 
			mDirection = newDir;

		}
		else if (ghostPos != mLastTileEval) // if ghost has left the last intersection tile or prison tile
		{
			// reset last tile
			mLastTileEval = Vector2i(0, 0);
		}

		this->travelMiddlePath(); // to ensure stays centered in path

		// apply movement 
		this->move(mSpeed * dt.asSeconds() * static_cast<Vector2f>(mDirection) );
	

}

// accepts vector containing pac's row and col tile, pac's dir and blinky's position vector
Vector2i Ghost::findTargetTile(const Vector2i& pacTile, const Vector2i& pacDir, const Vector2i& blinkyPos, GameMap& theMap)
{
	Vector2i target;

	if (mMode == 1) // chase
	{
		if (inPrisonBox(theMap)) // if in prison
		{
			if (mAIType == 3 || mAIType == 4) // clyde or blinky target to escape prison
				target = Vector2i(10, 3);
			else // inky or pinky target to escape
				target = Vector2i(11, 3);
		}
		else if (mAIType == 1) // 4 tiles in front of pacman, pinky
		{
			target = pacTile + (4 * pacDir); 
		}
		else if (mAIType == 3 || mAIType == 4) // pac's tile, blinky and clyde
		{
			target = pacTile;
		}
		else // AI type = 2, vector through offset tile and through blinky, inky 
		{
			target = static_cast<Vector2i>(calcInkyTarget(pacTile, pacDir, blinkyPos));
		}
	}
	else if (mMode == 2) // mode = scatter, run to corners
	{
		if (mAIType == 1)
		{
			target = Vector2i(2, 2); // upper left corner
		}
		else if (mAIType == 2)
		{
			target = Vector2i(21, 2); // upper right corner
		}
		else if (mAIType == 3)
		{
			target = Vector2i(2, 10); // lower left corner
		}
		else // AI type = 4
		{
			target = Vector2i(21, 10); // lower right corner
		}
	}
	else // frightened mode, maximize distance from pacman
	{
		target = pacTile;
	}

	return target;
	
}

// accepts: pacman's pos in cartesian coords, pacman's direction, blinky the ghost's position in cartesian coords
const Vector2f Ghost::calcInkyTarget(const Vector2i& pacPos, const Vector2i& pacDir, const Vector2i& blinkyPos)
{
	// calculate offset tile - 2 tiles in front of pac
	Vector2i offset((2 * pacDir).x + pacPos.x, (2 * pacDir).y + pacPos.y);

	// compute length of difference of offset vector from blinky's position
	float scaleLength = (float)length(blinkyPos, offset);

	Vector2f unitVect(static_cast<Vector2f>(blinkyPos) / sqrt(pow(blinkyPos.x, 2) + pow(blinkyPos.y, 2)) ); // normalize blinky vector by dividing by length

	return unitVect * scaleLength;
}

// returns direction to minimize or maximize the distance between the target and the ghost
Vector2i Ghost::findOptimalPath(GameMap& theMap)
{
	Vector2i ghostPos(getColIndex(getPosition()), getRowIndex(getPosition())); // calculate ghost pos

	vector<Vector2i> possibleDirs = findValidDirs(theMap); // determine what dirs are not walls

	int currVectLength = 0;

	// determine if maximizing or minimizing distance based on ghost mode - chase, scatter or frightened
	if (mMode == 1 || mMode == 2) // minimizing distance
	{
		Vector2i minDirection;
		int minVectorLength = 1000; // set high initially so first vector length becomes min
			
		for (auto i : possibleDirs) // draw vectors from all valid directions
		{
				// compute distance between next tile from ghost position in given direction and target tile
				currVectLength = (int)length(ghostPos + i, mTarget);

				if (currVectLength < minVectorLength) // found new min dir
				{
					minVectorLength = currVectLength;
					minDirection = i;
				}
		}

		return minDirection;
	}
	else // frightened mode = 3, maximizing distance
	{
		Vector2i maxDirection;
		int maxVectorLength = 0; // set low initially so first vector length becomes max
			
		currVectLength = 1; 

		for (auto i : possibleDirs) // draw vectors from all valid directions
		{
			// compute distance between next tile from ghost position in given direction and target tile
			currVectLength = (int)length(ghostPos + i, mTarget);

			if (currVectLength > maxVectorLength) // found new max dir
			{
				maxVectorLength = currVectLength;
				maxDirection = i;
			}
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

// finds the valid directions that a ghost may travel (based on walls) - returns a std::vector of those directions
vector<Vector2i> Ghost::findValidDirs(GameMap& theMap)
{
	Vector2i directions[4] = { Direction::DOWN, Direction::UP, Direction::LEFT, Direction::RIGHT }, // all possible dirs to move
		ghostPos(getColIndex(getPosition()), getRowIndex(getPosition())), // ghost's current position in cartesian coords
		cellInDirection; // for checking if next cell in a direction is a wall

	vector<Vector2i> validDirs; // contains all possible directions ghost can go

	for (int i = 0; i < 4; ++i)
	{
		cellInDirection = ghostPos + directions[i]; // evaluate cell in direction 

		// conditions for all modes excluding when ghost is eaten
		if (theMap[cellInDirection.y][cellInDirection.x].getIsPassable() != 1 // if next cell is not a wall 
			&& !( theMap[cellInDirection.y][cellInDirection.x].getIsPassable() == 3 && !inPrisonBox(theMap)) // if not trying to enter prison from outside
			&& directions[i] != -mDirection) // not making 180 degree turn
		{
			validDirs.push_back(directions[i]); // include curr direction as a valid dir
		}
	}

	return validDirs;
}

void Ghost::checkModeTimer(int level)
{

	// if in frightened mode NEVER HITS THIS CASE ********************
	if (mMode == 3 
		&& mModeClock.getElapsedTime().asSeconds() >= mModeTimer)
	{
		cout << "switched from frightened to chase at " << mModeClock.getElapsedTime().asSeconds() << " seconds.\n";
		mMode = 1; // set mode back to chase mode

		mModeClock.restart(); // restart the clock each time mode switched

		// set time to chase based on curr level
		if (level < 5) // levels 1-5
		{
			mModeTimer = 7; // ghosts chase pacman for 7 seconds, longer as levels higher
		}
		else if (level >= 5 && level <= 10) // between levels 5-10
		{
			mModeTimer = 15; 
		}
		else // level is > 10
		{
			mModeTimer = 20;
		}

	}
	else if (mMode == 1 // if in chase mode
		&& mModeClock.getElapsedTime().asSeconds() >= mModeTimer)
	{
		cout << "switched from chase to scatter at " << mModeClock.getElapsedTime().asSeconds() << " seconds.\n";
		mMode = 2; // alternate to scatter mode

		mModeClock.restart();

		// set time to scatter based on curr level
		if (level < 5) // levels 1-5
		{
			mModeTimer = 6; 
		}
		else if (level >= 5 && level <= 10) // between levels 5-10
		{
			mModeTimer = 4;
		}
		else // level is > 10
		{
			mModeTimer = 2;
		}
	}
	else if (mMode == 2 // if in scatter mode
		&& mModeClock.getElapsedTime().asSeconds() >= mModeTimer)
	{
		cout << "switched from scatter to chase at " << mModeClock.getElapsedTime().asSeconds() << " seconds.\n";
		mMode = 1; // alternate to chase mode

		mModeClock.restart();

		if (level < 5) // levels 1-5
		{
			mModeTimer = 7; // ghosts chase pacman for 7 seconds, longer as levels higher
		}
		else if (level >= 5 && level <= 10) // between levels 5-10
		{
			mModeTimer = 15;
		}
		else // level is > 10
		{
			mModeTimer = 20;
		}
	}
}

