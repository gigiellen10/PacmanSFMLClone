/* Programmer name: Genevieve Kochel
Date: April 10th 2024
Last modified: April 21th 2024
File purpose: this file contains the function definitons for the GameMap class */

#include "GameMap.hpp"


// constructor 
GameMap::GameMap(int mapOutline[MAP_HEIGHT][MAP_WIDTH])
{
	float peletSize = 7.f;
	bool isPower = false;

	mGate = RectangleShape((Vector2f(180.f, 15.f))); // init gate size
	mGate.setPosition(900, 360); // set gate position

	mNumPelets = 0; // initialize # pelets on map

	for (int i = 0; i < MAP_HEIGHT; ++i) // controlls rows
	{
		for (int j = 0; j < MAP_WIDTH; ++j) // controlls cols; reset power pelet and size params
		{
			// loop through integer array passed in and construct map according to cell values (0, 1 or 2)

			mTileArr[i][j].setIsPassable(mapOutline[i][j]); // denotes cell as wall or path

			mTileArr[i][j].setTilePos(sf::Vector2f((float)(CELL_SIZE * j), (float)(CELL_SIZE * i))); // calculate and set position of gametile

			// save row and column indexes of tile
			mTileArr[i][j].setRow(i);
			mTileArr[i][j].setCol(j);

			/* evaluate if a cell will contain a pelet */
			if (mTileArr[i][j].getIsPassable() != 1 
				&& mTileArr[i][j].getIsPassable() != 3) // if not a wall and not a prison cell 
			{
				isPower = false; // reset size and status of power pelet
				peletSize = 7.f;

				// set power pelets; static positions
				if ((i == 3 && j == 1) 
					|| (i == 1 && j == 9) || (i == 2 && j == 20))
				{
					peletSize = 20.f;
					isPower = true;
				}


				// add pelet to vector; compute exact midpt of the particular cell 
				mPeletList.push_back(Pelet((float)(CELL_SIZE * j) + (CELL_SIZE / 2), 
					(float)(CELL_SIZE * i) + (CELL_SIZE / 2), peletSize, isPower)); // add pelet to vector 
				++mNumPelets; // increment number of pelets

			}

			if (mTileArr[i][j].getIsPassable() == 1) // set wall color to blue
			{
				mTileArr[i][j].setFillColor(sf::Color::Blue);
			}
			else if (mTileArr[i][j].getIsPassable() == 2) // check for intersection point
			{
				mTileArr[i][j].setIntersection(true); // denote intersection status
				
			}			
		}
	}
}

// display function for rendering to screen 
void GameMap::displayMap(sf::RenderWindow &displayWindow, Font &font, int currScore, int level)
{
	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			displayWindow.draw(mTileArr[i][j]);
		}
	}
	
	// display pelets (i = a pointer to a vector element aka a Pelet)
	for (auto i = begin(mPeletList); i != end(mPeletList); ++i)
	{
		displayWindow.draw(*i);
	}

	// display gate
	displayWindow.draw(mGate);

	// display score 
	scoreAndLevelToScreen(displayWindow, font, currScore, level);
}

// compile-time polymorphism...yay
GameTile* GameMap::operator [](int index)
{
	return mTileArr[index];
} 

// edits the color of pelets displayed and decreases pelets that exist on gameboard
// returns 0 if not eaten, 1 if regular pelet eaten, 2 if power pel eaten
int GameMap::updatePelets(const FloatRect &pacGlobBounds, int frameCounter)
{
	int peletEatenType = 0; // initially pelet not eaten

	// loop through pelet vector to see if pac collides (eats) any pelets
	for (auto i = begin(mPeletList); i != end(mPeletList); ++i)
	{
		if (i->getGlobalBounds().intersects(pacGlobBounds) && !i->getIsEaten()) // pac is overlapping with a pelet and it hasnt been eaten yet
		{
			if (i->IsPower()) 
				peletEatenType = 2; // power pelet eaten
			else 
				peletEatenType = 1; // reg pelet

			i->setIsEaten(true); 
			i->setFillColor(Color::Black); // set to color of gameboard so not visible

			--mNumPelets; // 1 less pelet on board
		}

		// power pelet blinking animation
		if (!i->getIsEaten() && i->IsPower() && frameCounter % 8 < 4)
			i->setFillColor(Color::White);

		else if (!i->getIsEaten() && i->IsPower())
			i->setFillColor(Color::Black);
	}

	return peletEatenType;
}

