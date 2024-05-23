/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#pragma once

#include "Character.hpp" // ghost derived from character

class Ghost : public Character
{
public:
	// empty constructor for now, add animation rectangle
	Ghost(const Texture* ghostTexture, float spawnX, float spawnY, int AI)
		: Character(ghostTexture, spawnX, spawnY) {
		
		mDirection = Direction::UP; // set to up when beginning escape sequence
		mSpeed = 0.f; // stopped initially
		mMode = 1; // chase mode by default
		mAIType = AI; 
		mLastTileEval = Vector2i(0, 0);
		mModeTimer = 10; // chase for 10 seconds initially
		mModeClock = Clock(); 
		mPrisonDelay = 1 * AI; // establish a prison release time delay - may change so red leaves first
		mSpawnTile = Vector2i(getColIndex(Vector2f(spawnX, spawnY)), getRowIndex(Vector2f(spawnX, spawnY)));

		// set texture rectangle based on ghost AI
		if (AI == 1) // blue
		{
			this->setTextureRect(IntRect(619, 4, 192, 171));
		}
		else if (AI == 2) // pink
		{
			this->setTextureRect(IntRect(218, 385, 187, 174));
		}
		else if (AI == 3) // red 
		{
			this->setTextureRect(IntRect(214, 4, 196, 171));
		}
		else // orange, Ai type = 4
		{
			this->setTextureRect(IntRect(619, 384, 192, 170));
		}

		
	}

	// necessary setters and getters
	void setMode(int newMode) { mMode = newMode; }
	int getMode() const { return mMode; }

	void setModeTimer(int timeInMode) { mModeTimer = timeInMode; }

	void resetModeClock() { mModeClock.restart(); }

	void setPrisonDelay(int newDelaySecs) { mPrisonDelay = newDelaySecs; }

	void setTargetTile(Vector2i& newTarget) { mTarget = newTarget; }
	Vector2i getTargetTile() const { return mTarget; }

	// game methods 
	void animate(int frameCounter, GameMap& theMap); 

	void update(Time dt, Clock& prisonClock, GameMap& theMap, const Vector2i& pacTile, const Vector2i& pacDir, const Vector2i& blinkyPos);

	Vector2i findTargetTile(const Vector2i& pacTile, const Vector2i &pacDir, const Vector2i& blinkyPos, GameMap& theMap); 

	const Vector2f calcInkyTarget(const Vector2i& pacPos, const Vector2i& pacDir, const Vector2i& blinkyPos);

	Vector2i findOptimalPath(GameMap& theMap);

	bool inPrisonBox(GameMap& theMap);

	bool onSpawnPoint() const; 

	vector<Vector2i> findValidDirs(GameMap& theMap);

	void checkModeTimer(int level);


private:
	int mMode; // 1 - chase mode, 2 - scatter mode, 3 - frightened mode (run away from pac) 
	int mAIType; // determines chase pattern/personality for ghost (1 = Inky, 2 = Pinky, 3 = Blinky, 4 = Clyde)
	Vector2i mTarget; // target tile at moment in time
	Vector2i mLastTileEval; // so doesn't double evaluate an intersection tile while there
	Vector2i mSpawnTile; // spawn tile
	int mPrisonDelay; // # seconds delay until exits prison box
	int mModeTimer; // controlls how long ghost is in a certain mode based on level or power pelet
	Clock mModeClock; // reset each time a ghost's mode is switched
};





