#pragma once

/* Programmer name: Genevieve Kochel 
Date: April 13th 2024
File purpose: This file contains the class definition for the Pacman class */

#include "GameMap.hpp"
#include "LineSegment.hpp" // need for ray casting and collision detection

// inherits from circle shape - drawable, transformable
class Pacman : public CircleShape
{
public:
	
	// constructor
	Pacman(const Texture* pacTexture, float radius = 45.f, float newSpeed = 0.f) : CircleShape(radius)
	{

		this->setOrigin(getRadius(), getRadius()); // set to middle of sprite

		this->setTexture(pacTexture); 

		this->setTextureRect(sf::IntRect(20, 0, 220, 200)); // original state has no open mouth (1st state on sprite file)

		this->setPosition(PAC_SPAWN_X, PAC_SPAWN_Y);

		mIsAlive = true; // initially alive

		mSpeed = newSpeed;

		mDirection = Direction::STOP; 

		mScore = 0; // no points yet
	
	}
	
	// setters and getters

	bool getIsAlive() const { return mIsAlive; }
	void setIsAlive(bool newAliveStatus) { mIsAlive = newAliveStatus; }

	Vector2f getDirection() const { return mDirection; }
	void setDirection(Vector2f newDir) { mDirection = newDir; }

	LineSegment getRay() const { return mRay;}

	int getScore() const { return mScore; }
	void setScore(int newScore) { mScore = newScore; }

	/* movement and wall collision checking */

	void movePacman(Time dt, GameMap &theMap); // executes main movement

	bool isWallCollision(GameMap& theMap); 

	void travelMiddlePath(); // recenters in middle of path when turning corners

	void computeRayBounds();

	void reCenter(Vector2f& direction);

	bool isOnIntersection(GameMap& theMap) const; // checks if pacman's position is on an intersection point

	bool isValidDirection(bool onIntersection, const Vector2f desiredDirection, GameMap& theMap) const;

	void animateMouth(int frameCounter); // loops through mouth positions

	void changeDirection(Vector2f& currDir, const Vector2f& newDir); // checks current and new direction and rotates accordingly

	void isDeath(const sf::FloatRect& ghost1Pos, const sf::FloatRect& ghost2Pos, 
		const sf::FloatRect& ghost3Pos, const sf::FloatRect& ghost4Pos); // checks if pacman collides with any ghosts, adjusts pacman alive status accordingly
	

private:

	bool mIsAlive; // indicates if pacman still alive, game continues as long as true
	float mSpeed; // determines speed of smooth movement
	Vector2f mDirection; // current direction of entity
	LineSegment mRay; // checks collisions in current direction of travel
	int mScore; // pacman's current score (each pellet = 10 pts)

};

// useful functions for getting row and col of array based on position vector 
// (not passed by ref because required for return value of SFML getPosition() function)
int getRowIndex(Vector2f pos);
int getColIndex(Vector2f pos);