/* Programmer name: Genevieve Kochel
Date: May 27th 2024
Last modified: May 27th 2024
File purpose: this file contains the class definition for the GameWrapper class */

#include "Pacman.hpp"
#include "Ghost.hpp"

class GameWrapper {

public:

	// constructor and destructor - allocate/clean up heap vars
	GameWrapper();
	~GameWrapper();

	// setters and getters used in main()
	RenderWindow* getWindow() { return mWindow; }
	int getScore() const { return mScore; }
	int getLevel() const { return mLevel; }
	const Vector2f getBlinkyPosition() const { return mGhosts[2].getPosition(); } // blinky ghost is at index 2 (AI type = 3)
	/*int getFrameCounter() const { return mFrameCounter; }*/

	// main game methods
	int runGame(int* gameWonOrLoss);
	void reset();
	int displayStartScreen();
	int displayLostScreen();
	int displayWonScreen();


private:

	// allocated on heap
	RenderWindow* mWindow; 
	Pacman* mPac;
	vector<Ghost> mGhosts;
	GameMap* mMap;
	Texture* mPacmanAnimation;
	Texture* mGhostAnimation;
	Texture* mLogoHeader;
	Font* mFont;


	// stack vars
	int mLevel;
	int mScore;
	int mFrameCounter;
};