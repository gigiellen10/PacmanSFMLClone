/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#pragma once

#include "Character.hpp" // ghost derived from character

class Ghost : public Character
{
public:
	// empty constructor for now, add animation rectangle
	Ghost(const Texture* ghostTexture, float spawnX, float spawnY, const Color& temp /* temp parameter */ )
		: Character(ghostTexture, spawnX, spawnY) {
		this->setFillColor(temp);
	}

	// override to make ghost concrete 
	void movement(Time dt, GameMap& theMap);

private:
	// no data members yet!

};