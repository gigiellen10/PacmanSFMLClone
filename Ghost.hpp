/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#pragma once

#include "Character.hpp" // ghost derived from character

class Ghost : public Character
{
public:
	// empty constructor for now, add animation rectangle
	Ghost(const Texture* ghostTexture, float spawnX, float spawnY, const Color& temp, /* temp parameter */ int AI)
		: Character(ghostTexture, spawnX, spawnY) {
		this->setFillColor(temp);
		mSpeed = 200.f; // speed = 200 initally
		mMode = 1; // chase mode by default
		mAIType = AI; 
	}

	 
	void movement(Time dt, GameMap& theMap, Vector2f& pacTile, Vector2f& pacDir, Vector2f& blinkyPos);

	Vector2f& findTargetTile(Vector2f& pacTile, Vector2f &pacDir, Vector2f& blinkyPos); 

	const Vector2f& calcDirVectorTile(const Vector2f& pacPos, const Vector2f& pacDir, const Vector2f& blinkyPos);

private:
	int mMode; // 1 - chase mode, 2 - scatter mode 
	int mAIType; // determines chase pattern/personality for ghost (1 = Inky, 2 = Pinky, 3 = Blinky, 4 = Clyde)

};