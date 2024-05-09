#pragma once
/* programmer: Jacob Kolk */

#include "GameMap.hpp"
#include "Pacman.hpp"

class Ghost : public sf::Sprite {
private:
	int ghostId; // 1 = red, 2 = blue, 3 = white, 4 = green
	float mSpeed = 200.f; //determines the speed of movment
	float mEscapeSpeed = 800.f;
	Vector2f mDirection; // current direction of entity
	sf::Texture ghostTexture;
public:
	//setters
	void setDirection(Vector2f newDir) { mDirection = newDir; }
	//getters
	Vector2f getDirection(void) { return mDirection; }
	//constructor
	Ghost(float newSpeed, std::string textureFileName, float initialPositionX, float initialPositionY, int ghostNum, Vector2f initialDir)
	{
		mDirection = initialDir;
		ghostId = ghostNum;
		if (!ghostTexture.loadFromFile(textureFileName)) {//checks if file texture is loaded
			std::cout << "Failed to load ghost texture" << textureFileName << std::endl;
		}
		this->setTexture(ghostTexture);
		this->setPosition(initialPositionX, initialPositionY);
		mSpeed = newSpeed;
		mSpeed = newSpeed;
	}

	void moveGhost(sf::Time deltaTime, GameMap& themap, Pacman pac, bool* ghostOutOfBox);
	int getAvalibleDirections(GameMap& themap);	//find number of valid directions

	void reCenter(){//moves ghost to center of tile
		Vector2f currPos = this->getPosition();
		int row = getRowIndex(currPos), col = getColIndex(currPos);
		float newX = ((float)col * 90), newY = ((float)row * 90);
		this->setPosition(newX, newY);
		return;
	}
	void setToMiddle(void) {//sets position to validposition if in wall or outside of maze
		this->setPosition(Vector2f(90, 90));
	}
	bool isPositionValid(void) {
		int xCoords = this->getPosition().x;
		int yCoords = this->getPosition().y;
		if (xCoords > 1980 || yCoords > 990 || xCoords < 1 || yCoords < 1) {
			return false;
		}		
	}

};
void findNextDir3(GameMap& themap, Vector2f* mnewDir, Pacman pac, int avalibleDirections, int xPos, int yPos, Vector2f direction);
//finds direction that would get ghost closest to pacman in 3 way intersection
void findNextDir4(GameMap& themap, Vector2f* mnewDir, Pacman pac, int avalibleDirections, int xPos, int yPos, Vector2f Direction);
