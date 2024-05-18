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
		mDirection = Direction::UP; // set to up when beginning escape sequence
		mSpeed = 0.f; // stopped initially
		mMode = 1; // chase mode by default
		mAIType = AI; 
		mLastTileEval = Vector2i(0, 0);

		// establish a prison release time delay - may change so red leaves first
		mPrisonDelay = 3 * AI; 
		
	}

	 
	void update(Time dt, Clock& prisonClock, GameMap& theMap, const Vector2i& pacTile, const Vector2i& pacDir, const Vector2i& blinkyPos);

	Vector2i findTargetTile(const Vector2i& pacTile, const Vector2i &pacDir, const Vector2i& blinkyPos, GameMap& theMap); 

	const Vector2f calcInkyTarget(const Vector2i& pacPos, const Vector2i& pacDir, const Vector2i& blinkyPos);

	Vector2i findOptimalPath(GameMap& theMap);

	bool inPrisonBox(GameMap& theMap);

	vector<Vector2i> findValidDirs(GameMap& theMap);

private:
	int mMode; // 1 - chase mode, 2 - scatter mode, 3 - frightened mode (run away from pac) 
	int mAIType; // determines chase pattern/personality for ghost (1 = Inky, 2 = Pinky, 3 = Blinky, 4 = Clyde)
	Vector2i mTarget; // target tile at moment in time
	Vector2i mLastTileEval; // so doesn't double evaluate an intersection tile while there
	int mPrisonDelay; // # seconds delay until exits prison box
};





