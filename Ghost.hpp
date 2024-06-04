/* Programmer name: Genevieve Kochel
Date created: May 9th 2024
File purpose: contains the function definitions for the ghost class */

#pragma once

#include "Character.hpp" // ghost derived from character

class Ghost : public Character
{
public:

	~Ghost()
	{
		delete mGhostSounds;
		delete mChaseBuff;
		delete mFrightenedBuff;
		delete mRespawnBuff;
	}

	// constructor
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
		mIsSwitchingModes = false; 

		mGhostSounds = new Sound;
		mChaseBuff = new SoundBuffer;
		mFrightenedBuff = new SoundBuffer;
		mRespawnBuff = new SoundBuffer;

		mChaseBuff->loadFromFile("assets\\pacManSounds\\ghostChaseSound.wav");
		mFrightenedBuff->loadFromFile("assets\\pacManSounds\\ghostFrightened.wav");
		mRespawnBuff->loadFromFile("assets\\pacManSounds\\ghostEyesBackToPrison.wav");

		mGhostSounds->setBuffer(*mChaseBuff); // play chase sounds initially
		mGhostSounds->setLoop(true); // should loop
		mGhostSounds->setVolume(50);

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

	void setAIType(int AI) { mAIType = AI; }

	void setModeTimer(int timeInMode) { mModeTimer = timeInMode; }

	void resetModeClock() { mModeClock.restart(); }

	void setPrisonDelay(int newDelaySecs) { mPrisonDelay = newDelaySecs; }

	void setTargetTile(Vector2i& newTarget) { mTarget = newTarget; }
	Vector2i getTargetTile() const { return mTarget; }

	bool getIsModeSwitch() const { return mIsSwitchingModes; }
	void setIsModeSwitch(bool isSwitch) { mIsSwitchingModes = isSwitch; }

	// game methods 
	void animate(int frameCounter, GameMap& theMap); 

	void update(Time dt, Clock& prisonClock, GameMap& theMap, const Vector2i& pacTile, 
		const Vector2i& pacDir, const Vector2i& blinkyPos);

	Vector2i findTargetTile(const Vector2i& pacTile, const Vector2i &pacDir, const Vector2i& blinkyPos, GameMap& theMap); 

	const Vector2f calcInkyTarget(const Vector2i& pacPos, const Vector2i& pacDir, const Vector2i& blinkyPos);

	Vector2i findOptimalPath(GameMap& theMap);

	bool inPrisonBox(GameMap& theMap);

	bool onSpawnPoint() const; 

	vector<Vector2i> findValidDirs(GameMap& theMap);

	void checkModeTimer(int level, const Vector2i& pacPos, GameMap& theMap);

	void dead(Time dt, GameMap& theMap); 

	bool closeToPac(const Vector2i& pacPos, int buffer);
	
	void frightened(int level);

	void playSounds(bool pacIsAlive, GameMap& theMap);

	void stopGhostSounds() { mGhostSounds->stop(); }


private:
	int mMode; // 1 - chase mode, 2 - scatter mode, 3 - frightened mode (run away from pac) 
	int mAIType; // determines chase pattern/personality for ghost (1 = Inky, 2 = Pinky, 3 = Blinky, 4 = Clyde)
	Vector2i mTarget; // target tile at moment in time
	Vector2i mLastTileEval; // so doesn't double evaluate an intersection tile while there
	Vector2i mSpawnTile; // spawn tile
	int mPrisonDelay; // # seconds delay until exits prison box
	int mModeTimer; // controlls how long ghost is in a certain mode based on level or power pelet
	Clock mModeClock; // reset each time a ghost's mode is switched
	bool mIsSwitchingModes; // if the ghost switches modes on an iteration of game loop

	// audio controlls
	Sound* mGhostSounds; // switches between chase, frightened and eaten sounds; only stops looping when game won or over

	SoundBuffer* mChaseBuff;
	SoundBuffer* mFrightenedBuff;
	SoundBuffer* mRespawnBuff;
};





