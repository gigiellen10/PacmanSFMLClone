#pragma once

/* Programmer name: Genevieve Kochel
Date: April 10th
Last modified: April 10th, by Genevieve Kochel
File purpose: This file contains the class declaration for the game tile class */

#include "Pelet.hpp" // include for spawnPelet() function

// tiles make up the main gameboard - must be proportional to diameter of pacman
class GameTile : public sf::RectangleShape
{
public:

	// default constructor
	GameTile(const sf::Vector2f& size = sf::Vector2f(CELL_SIZE, CELL_SIZE), const sf::Color color = sf::Color::Black,
		const int isPassable = 0) : RectangleShape(size)
	{

		this->setFillColor(color);
		mIsPassable = isPassable;
		mIsIntersection = false; // set intersection status to false initally

		// init row and col members to 0 for now, will be updated when gameboard is constructed
		mRow = 0;
		mCol = 0;

	}

	// setters and getters
	int getIsPassable() { return mIsPassable;} 
	void setIsPassable(int newValue) { mIsPassable = newValue; }

	bool getIsIntersection() const { return mIsIntersection; }

	int getRow() const { return mRow; }
	void setRow(int newRow) { mRow = newRow; }
	
	int getCol() const { return mCol; }
	void setCol(int newCol) { mRow = newCol; }


	// public version to set fill color of tile
	void setTileColor(const sf::Color newColor)
	{
		this->setFillColor(newColor);
	}

	// public set position for constructing map
	void setTilePos(const sf::Vector2f newPos)
	{
		this->setPosition(newPos);
	}
	
	//set intersection status
	void setIntersection(bool isIntersection) { mIsIntersection = isIntersection; }

	//// returns center coordinates of the tile in pixels
	//Vector2f computeCenter() const
	//{
	//	return Vector2f( (mCol * CELL_SIZE) + (CELL_SIZE / 2), (mRow * CELL_SIZE) + (CELL_SIZE / 2) );
	//}


private:

	bool mIsIntersection; // true if its an intersection, false if it is not
	int mIsPassable; // contains a 1 for wall, 0 for empty 

	// row and column of array that cell exists in
	int mRow; 
	int mCol;
	
};