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
		mLastTileEval = Vector2f(0, 0);

		// establish a prison release time delay - may change so red leaves first
		mPrisonDelay = 2 * AI; 
		
	}

	 
	void update(Time dt, Clock& prisonClock, GameMap& theMap, const Vector2f& pacTile, const Vector2f& pacDir, const Vector2f& blinkyPos);

	Vector2f findTargetTile(const Vector2f& pacTile, const Vector2f &pacDir, const Vector2f& blinkyPos, GameMap& theMap); 

	const Vector2f calcInkyTarget(const Vector2f& pacPos, const Vector2f& pacDir, const Vector2f& blinkyPos);

	Vector2f findOptimalPath(GameMap& theMap);

	bool inPrisonBox(GameMap& theMap);

	/*void escapePrison(GameMap& theMap, Time dt, RenderWindow& window);*/

	vector<Vector2f> findValidDirs(GameMap& theMap);

private:
	int mMode; // 1 - chase mode, 2 - scatter mode, 3 - frightened mode (run away from pac) 
	int mAIType; // determines chase pattern/personality for ghost (1 = Inky, 2 = Pinky, 3 = Blinky, 4 = Clyde)
	Vector2f mTarget; // target tile at moment in time
	Vector2f mLastTileEval; // so doesn't double evaluate an intersection tile while there
	int mPrisonDelay; // # seconds delay until exits prison box
};





