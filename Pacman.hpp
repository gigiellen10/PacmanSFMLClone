#pragma once

/* Programmer name: Genevieve Kochel 
Date: April 13th 2024
File purpose: This file contains the class definition for the Pacman class */

#include "Character.hpp"

// inherits from Character abstract base class 
class Pacman : public Character
{
public:
	
	// pac constructor
	Pacman(const Texture* pacTexture, float radius = 45.f, float newSpeed = 0.f) 
		: Character(pacTexture, PAC_SPAWN_X, PAC_SPAWN_Y)
	{

		for (int i = 0; i < 5; ++i)
		{
			animationStates[i] = IntRect(i * 16, 0, 16, 17);
		}

		for (int i = 0; i < 12; ++i)
		{
			deathSequence[i] = IntRect(i * 16, 17, 16, 17);
		}

		this->setTextureRect(animationStates[0]); // original state has no open mouth (1st state on sprite file)

		mScore = 0; // no points yet
		mIndex = 0;
	}
	
	// setters and getters

	int getScore() const { return mScore; }
	void setScore(int newScore) { mScore = newScore; }

	/* movement and wall collision checking */

	void movement(Time dt, GameMap &theMap); // executes main movement

	bool isValidDirection(bool onIntersection, const Vector2i desiredDirection, GameMap& theMap) const;

	void animate(int frameCounter); // loops through mouth positions

	void changeDirection(Vector2i& currDir, const Vector2i& newDir); // checks current and new direction and rotates accordingly

private:
	int mScore; // pacman's current score (each pellet = 10 pts)
	int mIndex; // animation states index in array
	IntRect animationStates[5]; // texture rects with regular mouth animation
	IntRect deathSequence[12]; // texture rects with pacman death animation
};

