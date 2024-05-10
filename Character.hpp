/* Programmer: Genevieve Kochel
Date: May 9th 2024
File purpose: contains abstract base class that pacman and ghosts inherit from in game*/

#pragma once

#include "GameMap.hpp"
#include "LineSegment.hpp" // for collision ray casting

// abstract base class - inherits from drawable/transformable rectangle shape
class Character : public RectangleShape
{

public:

	// METHODS
	
	// pure virtual methods - overwridden in derived classes 
	virtual void movement(Time dt, GameMap &theMap) = 0; // executes main movement

	// virtual methods - defined in Character.cpp, not specific to ghost or pacman 
	virtual bool isWallCollision(GameMap& theMap);

	virtual void computeRayBounds();

	virtual void reCenter(Vector2f& direction);

	virtual bool isOnIntersection(GameMap& theMap) const; // checks if position is on an intersection point

	// constructor - takes the size of the character, the texture, and spawn points in x and y coords
	Character(const Texture* charTexture, float spawnX, float spawnY,
		float newSpeed = 0.f, const Vector2f& charSize = Vector2f(90.f, 90.f));
	
	// setters and getters
	bool getIsAlive() const { return mIsAlive; }
	void setIsAlive(bool newAliveStatus) { mIsAlive = newAliveStatus; }

	Vector2f getDirection() const { return mDirection; }
	void setDirection(Vector2f newDir) { mDirection = newDir; }

	LineSegment getRay() const { return mRay; }


	// DATA MEMBERS
	bool mIsAlive; // indicates if character still alive
	float mSpeed; // determines speed of smooth movement
	Vector2f mDirection; // current direction of entity
	LineSegment mRay; // checks collisions in current direction of travel

};

// useful functions for getting row and col of array based on position vector 
// (not passed by ref because required for return value of SFML getPosition() function)
int getRowIndex(Vector2f pos);
int getColIndex(Vector2f pos);