#pragma once

/* Programmer name: Genevieve Kochel
Date: April 10th, 2024
Last modified: April 10th 2024
File purpose: this file contains the class declaration for the game map class */

#include "GameTile.hpp"


class GameMap {

public:

	
	// constructor
	GameMap(int mapOutline[MAP_HEIGHT][MAP_WIDTH]);

	// necessary setters and getters 
	vector<Pelet> getPeletList() const { return mPeletList; }

	void setNumPelets(int newPelets) { mNumPelets = newPelets; }
	int getNumPelets() const { return mNumPelets; }


	// display function for rendering to screen
	void displayMap(RenderWindow& displayWindow, Font& font, int currScore);

	// adjusting which pelets displayed on screen if pac eats them; returns true if a pelet was eaten
	int updatePelets(const FloatRect& pacGlobBounds, int frameCounter);

	// overloaded []
	GameTile* operator [](int index);


private:

	GameTile mTileArr[MAP_HEIGHT][MAP_WIDTH]; // array to create paths, walls etc.
	vector<Pelet> mPeletList; // list of pelets contained on the map
	int mNumPelets; // number of pelets still on map (when = 0, pacman won game)
	RectangleShape mGate; // ghost prison gate

};