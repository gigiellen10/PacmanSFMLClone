// programmer: Jacob Kolk

#pragma once
#include "Ghost.hpp"
#include <math.h>
#include "GameState.hpp"

void Ghost::moveGhost(sf::Time deltaTime, GameMap& themap, Pacman pac, bool* ghostsOutOfBox)
{
	int xCoords = floor(this->getPosition().x);
	int yCoords = floor(this->getPosition().y);
	int curRow = (int)(yCoords / CELL_SIZE);
	int curCol = (int)(xCoords / CELL_SIZE);
	if (xCoords >= 1980 || yCoords >= 990) {
		std::cout << "GHOSTS OUT OF RANGE IN MOVE FINCTION!! " << std::endl;
		system("pause");
	}
	if (themap[curRow][curCol].getIsPassable() == 1 && *ghostsOutOfBox == true ) {

		Vector2f backwards = mDirection;
		backwards.x = this->mDirection.x * -1;//reverse direction
		backwards.y = this->mDirection.y * -1;
		int destinationPixX = xCoords; //
		int destinationPixY = yCoords;
		Vector2f ReCenterPos = Vector2f(0, 0);
		while (themap[(destinationPixY / CELL_SIZE)][(destinationPixX / CELL_SIZE)].getIsPassable() == 1) {
			destinationPixX = destinationPixX + backwards.x;
			destinationPixY = destinationPixY + backwards.y;
		}

		ReCenterPos.x = xCoords - destinationPixX;
		ReCenterPos.y = yCoords - destinationPixY;
		this->move(ReCenterPos);
	}

	xCoords = floor(this->getPosition().x);
	yCoords = floor(this->getPosition().y);
	if (ghostId == 1) {
		//std::cout << "XCOORDS " << xCoords << "  YCOORDS " << yCoords << std::endl;
	}
	if (*ghostsOutOfBox == false) {
		if (curRow == 5) {//if in left middle of box, go out
			if (xCoords < 910 &&  xCoords >= 900) {
				this->mDirection = Vector2f(0, -1);
				this->move(Vector2f(xCoords - 900, 0));
				this->move(mSpeed * mDirection * deltaTime.asSeconds());
				return;
			}
		}
		if (curRow == 5){
			if (xCoords > 980 && xCoords < 1000) { //if in middle right of box, go out
				this->mDirection = Vector2f(0, -1);
				this->move(Vector2f(xCoords - 990, 0));
				this->move(mSpeed * mDirection * deltaTime.asSeconds());
				return;
			}
		}
		if (curRow == 4 && curCol == 10) {//if half way out of box, continue
			this->mDirection = Vector2f(0, -1);
			this->move(mSpeed * mDirection * deltaTime.asSeconds());
			return;
		}
		if (curRow == 4 && curCol == 11) {//if half way out of box, continue
			this->mDirection = Vector2f(0, -1);
			this->move(mSpeed * mDirection * deltaTime.asSeconds());
			return;
		}
		if (yCoords > 260 && yCoords < 280 && curCol == 11 && this->mDirection != Vector2f(1,0)) {//if out go left
			this->reCenter();
			this->mDirection = Vector2f(1, 0);
			this->move(mSpeed * mDirection * deltaTime.asSeconds());
			return;
		}
		if (yCoords > 260 && yCoords < 280 && curCol == 10 && this->mDirection != Vector2f(-1, 0)) {//if out go left
			this->reCenter();
			this->mDirection = Vector2f(-1, 0);
			this->move(mSpeed * mDirection * deltaTime.asSeconds());
			return;
		}
	}
	if (themap[curRow][curCol].getIsPassable() == 2 && themap[(yCoords + ghostHitboxHeight) / CELL_SIZE][(xCoords + ghostHitboxWidth)/CELL_SIZE].getIsPassable() == 2) {//if not between cells
		this->reCenter();//sets position to the center of the tile the ghost is currently in
		xCoords = floor(this->getPosition().x);
		yCoords = floor(this->getPosition().y);
		int avalibleDirections = 0; //num of direction ghost can go, excuding backwards
		avalibleDirections = this->getAvalibleDirections(themap);
		if (avalibleDirections == 1) {//go straight
			/*if (this->ghostId == 1) {
				std::cout << "Ghost 1 going straight" << std::endl;
			}*/
			this->move(mSpeed * mDirection * deltaTime.asSeconds());
		}
		if (avalibleDirections == 2) {//if in corridor/corner
			int nextCol = curCol + mDirection.x;
			int nextRow = curRow + mDirection.y;
			if (themap[nextRow][nextCol].getIsPassable() != 1) {//if the next tile in same direction is not a wall, then continue in same direction
				this->move(mSpeed * mDirection * deltaTime.asSeconds());//then continue
				return;
			}
			else {//if in a corner, decide next direction
				if (themap[curRow - 1][curCol].getIsPassable() != 1 && mDirection.y != 1) {//check up
					mDirection = Vector2f(0, -1);
				}
				else if (themap[curRow + 1][curCol].getIsPassable() != 1 && mDirection.y != -1) {//check down
					mDirection = Vector2f(0, 1);
				}
				else if (themap[curRow][curCol + 1].getIsPassable() != 1 && mDirection.x != -11 ) {//check right
					mDirection = Vector2f(1, 0);
				}
				else if (themap[curRow][curCol - 1].getIsPassable() != 1 && mDirection.x != 1) {//check left
					mDirection = Vector2f(-1, 0);
				}
			}
			//std::cout << "Direction = 2, new direction = (" << mDirection.x << "," << mDirection.y << ")" << std::endl;//debugging things
			this->move(mEscapeSpeed * mDirection * deltaTime.asSeconds());//then continue
			return;
		}
		if (avalibleDirections == 3) {//if at 3/4 way intersection
				Vector2f newDir = Vector2f(0, 0);
				findNextDir3(themap, &newDir, pac, avalibleDirections, (int)(xCoords / CELL_SIZE), (int)(yCoords / CELL_SIZE), mDirection);//find bedt direction to go
				mDirection = newDir;
				this->move(mEscapeSpeed* mDirection * deltaTime.asSeconds());//then go
		}
		if (avalibleDirections == 4) {
			*ghostsOutOfBox = true;
			Vector2f newDir = Vector2f(0, 0);
			findNextDir4(themap, &newDir, pac, avalibleDirections, (int)(xCoords / CELL_SIZE), (int)(yCoords / CELL_SIZE), mDirection);//find bedt direction to go
			mDirection = newDir;
			this->move(mEscapeSpeed * mDirection * deltaTime.asSeconds());//then go
		}
	}
	else {//if between cells
		this->move(mSpeed * mDirection * deltaTime.asSeconds());//keep going
		return;
	}
}

int Ghost::getAvalibleDirections(GameMap& themap) //return number of avilible directions ghost can go
{//only called when on grid lines,( x and y coords) % 90 == 0
	//std::cout << "getting avilible directions" << std::endl;
	int avalibleDirections = 0;
	int xCoords = floor(this->getPosition().x);
	int yCoords = floor(this->getPosition().y);
	int dirRow = (int)(yCoords / CELL_SIZE);
	int dirCol = (int)(xCoords / CELL_SIZE);
	if (xCoords >= 1980 || yCoords >= 990 || xCoords < 1 || yCoords < 1) {//checks if ghost is out of map
		std::cout << "Looking outside map in getAvailible directions" << std::endl;
		system("pause");
		return NULL;
	}
	if (themap[dirRow + 1][dirCol].getIsPassable() != 1) {//check left corner
		avalibleDirections++;
	}
	if (themap[dirRow][dirCol - 1].getIsPassable() != 1) {//check left top
		avalibleDirections++;
	}
	if (themap[dirRow][dirCol + 1].getIsPassable() != 1) {//check right
		avalibleDirections++;
	}
	if (themap[dirRow - 1][dirCol].getIsPassable() != 1) {//check down
		avalibleDirections++;
	}
	/*if (this->ghostId == 1) {//debugging things
		std::cout << "Avalible dir" << avalibleDirections << std::endl;
	}*/
	return avalibleDirections;
}

void findNextDir3(GameMap& themap, Vector2f* mnewDir, Pacman pac, int avalibleDirections, int xPos, int yPos, Vector2f mDirection) {//finds valid straght path from intersection, 
	//that at the end will be clostest to pacman, 25% of time its random to prevent grouping
	int random = rand() % 20;
	Vector2f newDir[3] = { Vector2f(0, 0), Vector2f(0, 0)};//array of possible new directions
	Vector2f pacToValidInter[3] = { Vector2f(0, 0), Vector2f(0, 0) };//array of vectors from ghost to pacMan
	int curX, curY = 0;
	int dirI = 0;
	if (xPos >= 1980 || yPos >= 990 || xPos < 1 || yPos < 1) {//checking if ghost is out of bounds
		std::cout << "Looking outside map in getAvailible directions" << std::endl;
		system("pause");
		return;
	}
	int Targetx = floor(pac.getPosition().x);//getting pixel coords of pacman
	int Targety = floor(pac.getPosition().y);
	if (avalibleDirections == 3) {
		if (themap[yPos][xPos + 1].getIsPassable() != 1 && mDirection.x != -1) {//checking right
			curX = xPos + 1;
			curY = yPos;
			for (; themap[curY][curX].getIsPassable() == 0;) { curX += 1; }//if one tile right is a valid move, continute that way until a wall is hit
			pacToValidInter[dirI] = Vector2f(curX-1, curY);//record vector from ghost to pacman
			newDir[dirI] = Vector2f(1, 0);//save direction in list of possible directions
			dirI++;
		}
		if (themap[yPos+1][xPos].getIsPassable() != 1 && mDirection.y != -1) {//checking down
			curX = xPos;
			curY = yPos+1;
			for (; themap[curY][curX].getIsPassable() == 0;) { curY += 1; }
			pacToValidInter[dirI] = Vector2f(curX, curY-1);
			newDir[dirI] = Vector2f(0, 1);
			dirI++;
		}
		if (themap[yPos][xPos - 1].getIsPassable() != 1 && mDirection.x != 1) {//checking left
			curX = xPos - 1;
			curY = yPos;
			for (; themap[curY][curX].getIsPassable() == 0;) { curX -= 1; }
			pacToValidInter[dirI] = Vector2f(curX+1, curY);
			newDir[dirI] = Vector2f(-1, 0);
			dirI++;
		}
	
		if (themap[yPos - 1][xPos].getIsPassable() != 1 &&  mDirection.y != 1) {//checking up
			curX = xPos;
			curY = yPos - 1;
			for (; themap[curY][curX].getIsPassable() == 0;) { curY -= 1; }
			pacToValidInter[dirI] = Vector2f(curX, curY+1);
			newDir[dirI] = Vector2f(0, -1);
			
		}
		if (dirI > 2) {//check if the function think there is more valid options than posible
			//std::cout << "GetDir3 had too many possible directions\n";
			//system("pause");
			dirI--;//this should never trigger, but if it does it wont crash
		}
		int deltaX = 0;
		int deltaY = 0;
		int distance[2] = { 0,0};
		for (dirI = 0; dirI < 2; dirI++){//loop though, calculate distance to pacman and save in array
			deltaX = pacToValidInter[dirI].x *90 - Targetx + 45;
			deltaY = pacToValidInter[dirI].y *90 - Targety + 45;
		distance[dirI] = sqrt(pow(deltaY, 2) + pow(deltaY, 2));
		}
		int minDir = 0;
		int min = 1000000;
		for(dirI = 0; dirI < 2; dirI++){//find minum distance of possible moves
			if(distance[dirI]< min){
				minDir = dirI;
				min = distance[dirI];
			}
		}
		if (random == 0) {//5% of the time go random direction
			random = rand() % 2;
			*mnewDir = newDir[random];
		}
		else {
			*mnewDir = newDir[minDir];//return the best move
		}
		//	std::cout << "decided Vector 3 " << (int)(newDir[minDir].x) << " " << (int)(newDir[minDir].y) << std::endl;
		return;
	}
}

void findNextDir4(GameMap& themap, Vector2f* mnewDir, Pacman pac, int avalibleDirections, int xPos, int yPos, Vector2f mDirection) {

	//this logic is the same as the previous funtion, just with a 4 way intersection.

	int random = rand() % 20;
	Vector2f newDir[4] = { Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0) };
	Vector2f pacToValidInter[4] = { Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0) };
	int curX, curY = 0;
	int dirI = 0;
	int Targetx = pac.getPosition().x;
	int Targety = pac.getPosition().y;
	if (avalibleDirections == 4) {
		if (themap[yPos][xPos + 1].getIsPassable() != 1 && mDirection.x != -1) {
			curX = xPos + 1;
			curY = yPos;
			for (; themap[curY][curX].getIsPassable() == 0;) { curX += 1; }
			pacToValidInter[dirI] = Vector2f(curX-1, curY);
			newDir[dirI] = Vector2f(1, 0);
			dirI++;
		}
		if (themap[yPos + 1][xPos].getIsPassable() != 1 && mDirection.y != -1) {
			curX = xPos;
			curY = yPos + 1;
			for (; themap[curY][curX].getIsPassable() == 0;) { curY += 1; }
			pacToValidInter[dirI] = Vector2f(curX, curY-1);
			newDir[dirI] = Vector2f(0, 1);
			dirI++;
		}
		if (themap[yPos][xPos - 1].getIsPassable() != 1 && mDirection.x != 1) {
			curX = xPos - 1;
			curY = yPos;
			for (; themap[curY][curX].getIsPassable() == 0;) { curX -= 1; }
			pacToValidInter[dirI] = Vector2f(curX+1, curY);
			newDir[dirI] = Vector2f(-1, 0);
			dirI++;
		}
		if (themap[yPos - 1][xPos].getIsPassable() != 1 &&  mDirection.y != 1) {
			curX = xPos;
			curY = yPos - 1;
			for (; themap[curY][curX].getIsPassable() == 0;) { curY -= 1; }
			pacToValidInter[dirI] = Vector2f(curX, curY+1);
			newDir[dirI] = Vector2f(0, -1);
			dirI++;
		}
		if (dirI > 3) {
			dirI--;//this shold never trigger, but if it does the game wont crash
		}
		int deltaX = 0;
		int deltaY = 0;
		int cSquared = 0;
		double distance[3] = { 0,0,0 };
		for (dirI = 0; dirI < 3; dirI++) {
			deltaX = pacToValidInter[dirI].x * 90 - Targetx;
			deltaY = pacToValidInter[dirI].y * 90 - Targety;
			 cSquared = ((pow(deltaY, 2) + pow(deltaX, 2)));
			 distance[dirI] = sqrt(cSquared);
		}
		int minDir = 0;
		int min = 1000000;
		for (dirI = 0; dirI < 3; dirI++) {
			if (distance[dirI] < min) {
				minDir = dirI;
				min = distance[dirI];
			}
		}
		if (random == 0) {//5% of the time go random direction
			random = rand() % 3;
			*mnewDir = newDir[random];
		}
		else {
			*mnewDir = newDir[minDir];
		//	std::cout << "decided Vector 4 " << (int)(newDir[minDir].x) << " " << (int)(newDir[minDir].y) << std::endl;
		}
	}
}
