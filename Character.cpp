/* Programmer: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the virtual function defintions for the Character pure virtual base class */

#include "Character.hpp"

Character::Character(const Texture* charTexture, float spawnX, float spawnY,
	float newSpeed, const Vector2f& charSize) : RectangleShape(charSize)
{
	this->setOrigin(getSize().x / (float)2, getSize().y / (float)2); // set to middle of sprite

	this->setTexture(charTexture);

	this->setPosition(spawnX, spawnY);

	mIsAlive = true; // initially alive

	mSpeed = newSpeed; // set speed

	mDirection = Direction::STOP; // initially static/no movement

}

// determines if character about to hit a wall 
bool Character::isWallCollision(GameMap& theMap)
{
	int RayCol = getColIndex(mRay.getEnd()), // get row and col that ray ends in
		RayRow = getRowIndex(mRay.getEnd());

	if (theMap[RayRow][RayCol].getIsPassable() == 1) // we've almost hit a wall
	{
		return true;
	}

	return false;
}

// shoots a ray from the object origin to edge of obj global bounds - 
// computes the two points of ray based on origin and size of object
void Character::computeRayBounds()
{
	Vector2f endPt;
	float xPos = this->getPosition().x,
		yPos = this->getPosition().y;


	// calculate where to shoot ray depending on direction currently travelling

	if (mDirection == Direction::UP) // compute endpt upwards
	{
		endPt = Vector2f(xPos, yPos - COLLISION_RAY);
	}
	else if (mDirection == Direction::RIGHT) // compute endpt right
	{
		endPt = Vector2f(xPos + COLLISION_RAY, yPos);
	}
	else if (mDirection == Direction::DOWN) // compute endpt downwards
	{
		endPt = Vector2f(xPos, yPos + COLLISION_RAY);
	}
	else if (mDirection == Direction::LEFT) // compute endpt left
	{
		endPt = Vector2f(xPos - COLLISION_RAY, yPos);
	}

	// set start and endpt of ray
	mRay.setStart(this->getPosition()); // ray should shoot from pacman origin (center of circle)
	mRay.setEnd(endPt);
}

// recenters character in middle of cell
void Character::reCenter()
{
	Vector2f currPos = this->getPosition();
	int row = getRowIndex(currPos), col = getColIndex(currPos);
	float newX = ((float)col * CELL_SIZE) + (CELL_SIZE / 2), newY = ((float)row * CELL_SIZE) + (CELL_SIZE / 2);

	this->setPosition(newX, newY);
}

// determines if character is currently on an intersection point
bool Character::isOnIntersection(GameMap& theMap) const
{
	// determine current position
	int col = getColIndex(this->getPosition()),
		row = getRowIndex(this->getPosition());

	return theMap[row][col].getIsIntersection();
}

//bool Character::isNearCenter(GameMap& theMap) const
//{
//	// determine current position
//	int col = getColIndex(this->getPosition()),
//		row = getRowIndex(this->getPosition());
//
//	float xCoord = theMap[row][col].getGlobalBounds().left + 30, 
//		yCoord = theMap[row][col].getGlobalBounds().top + 30;
//
//	// create centered sub square close to middle that origin of character coud intersect with
//	FloatRect temp(Vector2f(xCoord, yCoord), Vector2f(45.f, 45.f));
//
//	return temp.contains(this->getPosition()); // if sub rectangle intersects w character position
//	
//}

void Character::travelMiddlePath()
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

// returns the row index of a position vector in the map array
int getRowIndex(Vector2f pos)
{
	return ((int)(pos.y / (float)CELL_SIZE));
}

// returns the col index of a position vector in the map array
int getColIndex(Vector2f pos)
{
	return ((int)(pos.x / (float)CELL_SIZE));
}

