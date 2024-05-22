#pragma once

/* Programmer: Genevieve Kochel
Date: April 18th 2024 
File purpose: this file contains the pelet class definion */

#include "GameState.hpp"

class Pelet : public CircleShape
{
public:
	
	// constructor 
	Pelet(float xPos, float yPos, float size, bool isPowerPel) : CircleShape(size) 
	{
		mIsEaten = false; // not eaten initally
		mIsPower = isPowerPel;
		this->setOrigin(getRadius(), getRadius()); // set the origin to the exact center of the pelet
		this->setFillColor(Color::White); // set the fill color to white
		this->setPosition(xPos, yPos); // set the pelet position
	}

	// setters and getters
	void setIsEaten(bool status) { mIsEaten = status; }
	bool getIsEaten() const { return mIsEaten; }

	bool IsPower() const { return mIsPower; }


private:

	bool mIsEaten; // if pacman has eaten this pelet or not
	bool mIsPower; // if power pelet
};