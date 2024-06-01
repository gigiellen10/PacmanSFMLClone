/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#include "Ghost.hpp"

// purpose: main movement function; updates ghost position when alive (death() function updates move. for dead)
// accepts: deltaTime (time between frames), time ghosts have been in prison at begin of game, the gamemap, 
// pacman's tile coordinates, pacman's direction, blinky's position coords, and (true/false) if ghost is switching modes on this frame

void Ghost::update(Time dt, Clock& prisonClock, GameMap& theMap, const Vector2i& pacTile, 
	const Vector2i& pacDir, const Vector2i& blinkyPos)
{

		Vector2i ghostPos(getColIndex(getPosition()), getRowIndex(getPosition())),
			newDir; // direction that ghost should travel to get to target tile

		// determine if ghost can leave the prison initially, set speed to move
		if (prisonClock.getElapsedTime().asSeconds() >= mPrisonDelay
			&& this->inPrisonBox(theMap)) // prevent unnecessary speed assignments
		{
			mSpeed = GHOST_CHASE_SPEED; 
		}


			// determine if ghost on an intersection
			bool onIntersection = isOnIntersection(theMap);

			mTarget = findTargetTile(pacTile, pacDir, blinkyPos, theMap); // for pinpointing ghost target tile

			// set new ray bounds 
			this->computeRayBounds();

			
			//  will hit a wall if ghost continues, so stop and recenter
			if (isWallCollision(theMap))
			{
				// should pick a valid direction before moving further
				this->reCenter();

				// find direction of shortest path to target tile
				mDirection = findOptimalPath(theMap); // pass in coordinates to minimize or maximize distance from 

				// mark current tile as evaluated
				mLastTileEval = ghostPos;
			}

			// if (haven't already evaluated curr tile and on intersection or prison cell)
			if ((mLastTileEval != ghostPos
					&& (onIntersection || inPrisonBox(theMap))) || mIsSwitchingModes)
			{
					// find direction of shortest path to target tile
					mDirection = findOptimalPath(theMap); // pass in coordinates to minimize or maximize distance from 

					// mark current tile as evaluated
					mLastTileEval = ghostPos;

			}

			if (ghostPos != mLastTileEval) // if ghost has left the last intersection tile or prison tile
			{
					// reset last tile
					mLastTileEval = Vector2i(0, 0);
			}

				

			this->travelMiddlePath(); // to ensure stays centered in path

			// apply movement 
			this->move(mSpeed * dt.asSeconds() * static_cast<Vector2f>(mDirection));

}

// purpose: sends ghost back to prison cell with a slight delay before respawns
void Ghost::dead(Time dt, GameMap& theMap)
{
	Vector2i ghostPos(getColIndex(getPosition()), getRowIndex(getPosition())); // ghost's curr x and y coords

	if (mJustDied)
	{
		mPrisonDelay = 50; // ghost will idle in prison for 50 calls of this function

		if (mAIType == 3 || mAIType == 4) // clyde or blinky target
			mTarget = Vector2i(10, 3);
		else // inky or pinky target
			mTarget = Vector2i(11, 3);
	}

	
	if (ghostPos == mSpawnTile) // if on spawn tile target
	{
		this->reCenter(); 

		mIsSwitchingModes = false; // reset so cannot change directions/ turn 180 deg
		
		// if delay elapsed in prison
		if (mPrisonDelay == 0)
		{
			mIsAlive = true; // set ghost back to alive status so can leave prison
			mSpeed = 200.f;
		}

		--mPrisonDelay; // decrease the prisonDelay value each call of dead()
	}
	else if (ghostPos == mTarget) // if on tile above prison
	{
		mTarget = mSpawnTile; // new target is the spawn tile inside prison
	}
	else // ghost is travelling path to prison spawn pt.
	{
		if (ghostPos != mLastTileEval) // if haven't already evaluated a direction for that tile
		{
			mDirection = findOptimalPath(theMap);
			mLastTileEval = ghostPos;
		}

		this->travelMiddlePath();
		this->move(mSpeed * dt.asSeconds() * static_cast<Vector2f>(mDirection));
	}

}

// accepts vector containing pac's row and col tile, pac's dir and blinky's position vector
Vector2i Ghost::findTargetTile(const Vector2i& pacTile, const Vector2i& pacDir, const Vector2i& blinkyPos, GameMap& theMap)
{
	Vector2i target;

	if (mMode == 1) // chase
	{
		if (inPrisonBox(theMap)) // if in the prison
		{
			if (mAIType == 3 || mAIType == 4) // clyde or blinky target to escape prison
				target = Vector2i(10, 3);
			else // inky or pinky target to escape
				target = Vector2i(11, 3);
		}
		else if (mAIType == 1) // 4 tiles in front of pacman, pinky
		{
			target = static_cast<Vector2i>(calcInkyTarget(pacTile, pacDir, blinkyPos));
		}
		else if (mAIType == 3 || mAIType == 4) // pac's tile, blinky and clyde
		{
			target = pacTile;
		}
		else // AI type = 2, pinky 
		{
			target = pacTile + (4 * pacDir);
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

// purpose: returns true or false depending if ghost is on it's spawn point
bool Ghost::onSpawnPoint() const
{
	Vector2i currPos(getColIndex(getPosition()), getRowIndex(getPosition())),
		spawnCell(0, 0); // init vars

	spawnCell.y = GHOST_SPAWN_Y / CELL_SIZE;

	// determine spawn cell coords based on ghost type (AI)
	if (mAIType == 1) // blue 
		spawnCell.x = (GHOST_SPAWN_X_B / CELL_SIZE);
	else if (mAIType == 2) // pink
		spawnCell.x = (GHOST_SPAWN_X_P / CELL_SIZE);
	else if (mAIType == 3) // red 
		spawnCell.x = GHOST_SPAWN_X_R / CELL_SIZE;
	else // orange, Ai type = 4 
		spawnCell.x = GHOST_SPAWN_X_O / CELL_SIZE;

		return currPos == spawnCell; // is ghost's curr position == to it's spawn cell?
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

		
		// if ghost is eaten - may enter prison but not hit wall or turn around
		if (!mIsAlive 
			&& theMap[cellInDirection.y][cellInDirection.x].getIsPassable() != 1 
			&& directions[i] != -mDirection)
		{
			validDirs.push_back(directions[i]); // include curr direction as a valid dir
		}
		// in all other cases independent of mode or other factors - cannot go back into prison [from outside], turn around or hit a wall 
		else if (theMap[cellInDirection.y][cellInDirection.x].getIsPassable() != 1
			&& !(theMap[cellInDirection.y][cellInDirection.x].getIsPassable() == 3 && !inPrisonBox(theMap))
			&& directions[i] != -mDirection)
		{
			validDirs.push_back(directions[i]);
		}
		
	}

	return validDirs;
}

void Ghost::checkModeTimer(int level, const Vector2i& pacPos, GameMap& theMap)
{
	// if in frightened mode 
	if (mMode == 3
		&& mModeClock.getElapsedTime().asSeconds() >= mModeTimer)
	{
		mMode = 1; // set mode back to chase mode
		mIsSwitchingModes = true;

		mSpeed = GHOST_FRIGHT_SPEED;

		mModeClock.restart(); // restart the clock each time mode switched

		// set time to chase based on curr level
		if (level < 5) // levels 1-5
			mModeTimer = 7; // ghosts chase pacman for 7 seconds, longer as levels higher

		else if (level >= 5 && level <= 10) // between levels 5-10
			mModeTimer = 15;

		else // level is > 10
			mModeTimer = 20;

	}
	else if (mMode == 1 // if in chase mode and clyde is within 8 cells of pac or the mode clock has run out
		&& ((mAIType == 4 && !inPrisonBox(theMap) && this->closeToPac(pacPos, 8)) || mModeClock.getElapsedTime().asSeconds() >= mModeTimer))
	{
		mMode = 2; // alternate to scatter mode
		mIsSwitchingModes = true;

		mSpeed = GHOST_CHASE_SPEED;

		mModeClock.restart();

		// set time to scatter based on curr level
		if (level < 5) // levels 1-5
			mModeTimer = 6;

		else if (level >= 5 && level <= 10) // between levels 5-10
			mModeTimer = 4;

		else // level is > 10
			mModeTimer = 2;
	}
	else if (mMode == 2 // if in scatter mode
		&& mModeClock.getElapsedTime().asSeconds() >= mModeTimer)
	{
		mMode = 1; // alternate to chase mode
		mIsSwitchingModes = true;

		mSpeed = GHOST_CHASE_SPEED;

		mModeClock.restart();

		if (level < 5) // levels 1-5
			mModeTimer = 7; // ghosts chase pacman for 7 seconds, longer as levels higher

		else if (level >= 5 && level <= 10) // between levels 5-10
			mModeTimer = 15;

		else // level is > 10
			mModeTimer = 20;
	}
	else
		mIsSwitchingModes = false;

}


// purpose: animates ghost based on mode (frightened/not frightenened), alive state, and current direction of travel
void Ghost::animate(int frameCounter, GameMap& theMap)
{

	if (onSpawnPoint()) // if ghost on spawn point
	{
		
		// loop between bouncing up and down within the cell
		if (frameCounter % 8 < 4) // upwards orientation
		{
			if (mAIType == 1) // blue 
				this->setTextureRect(IntRect(619, 4, 192, 171));
			else if (mAIType == 2) // pink
				this->setTextureRect(IntRect(218, 385, 187, 174));
			else if (mAIType == 3) // red 
				this->setTextureRect(IntRect(214, 4, 196, 171));
			else // orange, Ai type = 4 
				this->setTextureRect(IntRect(619, 384, 192, 170));
		}
		else // downwards orientation
		{
			if (mAIType == 1) // blue 
				this->setTextureRect(IntRect(423, 186, 192, 174));

			else if (mAIType == 2) // pink 
				this->setTextureRect(IntRect(24, 560, 192, 189));

			else if (mAIType == 3) // red 
				this->setTextureRect(IntRect(28, 184, 192, 183));

			else // orange, Ai type = 4 
				this->setTextureRect(IntRect(422, 565, 197, 179));
		}
	}
	else if (!mIsAlive) // if ghost is dead
	{
		if (mDirection == Direction::DOWN)
			this->setTextureRect(IntRect(841, 320, 118, 125));
		else if (mDirection == Direction::UP)
			this->setTextureRect(IntRect(837, 10, 128, 127));
		else if (mDirection == Direction::LEFT)
			this->setTextureRect(IntRect(832, 224, 137, 110));
		else // right
			this->setTextureRect(IntRect(838, 119, 130, 117));

	}
	else if (mMode == 3)
	{
		this->setTextureRect(IntRect(832, 456, 193, 169));

		// loop between blue/white frightened face if <0.5 second of frightened mode left
		if ((float)mModeTimer - mModeClock.getElapsedTime().asSeconds() <= 2 && frameCounter % 8 > 4) // white face
			this->setTextureRect(IntRect(807, 626, 187, 164));
			
	}
	else if (mDirection == Direction::UP) // animation for moving up
	{
			if (mAIType == 1) // blue 
				this->setTextureRect(IntRect(619, 4, 192, 171));
			else if (mAIType == 2) // pink
				this->setTextureRect(IntRect(218, 385, 187, 174));
			else if (mAIType == 3) // red 
				this->setTextureRect(IntRect(214, 4, 196, 171));
			else // orange, Ai type = 4 
				this->setTextureRect(IntRect(619, 384, 192, 170));
	}
	else if (mDirection == Direction::DOWN) // animation moving down
	{
		if (mAIType == 1) // blue 
			this->setTextureRect(IntRect(423, 186, 192, 174));
		else if (mAIType == 2) // pink 
			this->setTextureRect(IntRect(24, 560, 192, 189));
		else if (mAIType == 3) // red 
			this->setTextureRect(IntRect(28, 184, 192, 183));
		else // orange, Ai type = 4
			this->setTextureRect(IntRect(422, 565, 197, 179));
	}
	else if (mDirection == Direction::LEFT) // animation moving down
	{
		if (mAIType == 1) // blue 
			this->setTextureRect(IntRect(611, 194, 193, 170));
		else if (mAIType == 2) // pink 
			this->setTextureRect(IntRect(217, 568, 188, 169));
		else if (mAIType == 3) // red 
			this->setTextureRect(IntRect(213, 193, 191, 170));
		else // orange, Ai type = 4
			this->setTextureRect(IntRect(619, 578, 180, 165));
	}
	else if (mDirection == Direction::RIGHT) // animation moving down
	{
		if (mAIType == 1) // blue 
			this->setTextureRect(IntRect(422, 10, 189, 166));
		else if (mAIType == 2) // pink 
			this->setTextureRect(IntRect(24, 383, 197, 172));
		else if (mAIType == 3) // red 
			this->setTextureRect(IntRect(21, 9, 201, 162));
		else // orange, Ai type = 4
			this->setTextureRect(IntRect(421, 390, 201, 166));
	}

}

// purpose: determines if the ghost is within <buffer> number of tiles of pacman in current dir of travel
// returns: true if within buffer range, false otherwise
bool Ghost::closeToPac(const Vector2i& pacPos, int buffer)
{
	Vector2i ghostPos(getColIndex(getPosition()), getRowIndex(getPosition()));

	if ((mDirection == Direction::UP || mDirection == Direction::DOWN) &&
		ghostPos.x == pacPos.x)
	{
		return abs(ghostPos.y - pacPos.y) >= buffer; // if difference in y coords (vertical) is >= buffer of tiles
	}
	else if ((mDirection == Direction::LEFT || mDirection == Direction::RIGHT) &&
		ghostPos.y == pacPos.y)
	{
		return abs(ghostPos.x - pacPos.x) >= buffer; // if difference in x coords (lateral) is >= buffer of tiles
	}
	else 
		return false;
}

// purpose: adjusts ghost settings to properly initiate frightened mode
void Ghost::frightened(int level)
{
	mMode = 3; // frightened mode

	mIsSwitchingModes = true;

	mModeClock.restart();

	mSpeed = GHOST_FRIGHT_SPEED;

	// set frightened mode timer - higher levels = shorter time available to be eaten
	if (level < 5) // levels 1-5
		mModeTimer = 7; // ghosts chase pacman for 7 seconds, longer as levels higher

	else if (level >= 5 && level <= 10) // between levels 5-10
		mModeTimer = 5;

	else // level is > 10
		mModeTimer = 3;
}




