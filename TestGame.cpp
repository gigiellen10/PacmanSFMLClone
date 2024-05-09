/* programmer: Genevieve Kochel
Date: April 20th 2024
File purpose: This file contains the function definitions for the test game class */

#include "TestGame.hpp"



// this test function was implemented by Genevieve Kochel
void TestGame::testComputeRayBounds()
{
	Texture blankText; // no default argument in pac constructor for texture, so this is necessary
	Pacman tempPac(&blankText); // construct temporary pac

	tempPac.setPosition(0, 0); // set pac's position to (0, 0) for easy expected ray computations

	/* WE WILL TEST UP, DOWN, LEFT AND RIGHT RAYS */

	// note: universal length of a ray is 60 pixels - a constant macro in GameState.hpp

	// left
	tempPac.setDirection(Direction::LEFT);
	tempPac.computeRayBounds(); // computes bounds based on left dir

	// expected: start of ray @ origin (0, 0) and end @ 60 pixels to the left in x direction (- negative)
	if (tempPac.getRay().getStart() == Vector2f(0, 0)
		&& tempPac.getRay().getEnd() == Vector2f(-60, 0))
	{
		std::cout << "Left ray test passed!" << std::endl;
	}
	else
	{
		std::cout << "Left ray test FAILED." << std::endl;
	}

	// right
	tempPac.setDirection(Direction::RIGHT);
	tempPac.computeRayBounds(); // computes bounds based on RIGHT dir

	// expected: start of ray @ origin (0, 0) and end @ 60 pixels to the RIGHT in x direction (+ positive)
	if (tempPac.getRay().getStart() == Vector2f(0, 0)
		&& tempPac.getRay().getEnd() == Vector2f(60, 0))
	{
		std::cout << "Right ray test passed!" << std::endl;
	}
	else
	{
		std::cout << "Right ray test FAILED." << std::endl;
	}

	// up
	tempPac.setDirection(Direction::UP);
	tempPac.computeRayBounds(); // computes bounds based on UP dir

	// expected: start of ray @ origin (0, 0) and end @ 60 pixels to the UP in y direction (- negative)
	if (tempPac.getRay().getStart() == Vector2f(0, 0)
		&& tempPac.getRay().getEnd() == Vector2f(0, -60))
	{
		std::cout << "Up ray test passed!" << std::endl;
	}
	else
	{
		std::cout << "Up ray test FAILED." << std::endl;
	}

	// down
	tempPac.setDirection(Direction::DOWN);
	tempPac.computeRayBounds(); // computes bounds based on DOWN dir

	// expected: start of ray @ origin (0, 0) and end @ 60 pixels to the DOWN in y direction (+ positive)
	if (tempPac.getRay().getStart() == Vector2f(0, 0)
		&& tempPac.getRay().getEnd() == Vector2f(0, 60))
	{
		std::cout << "Down ray test passed!" << std::endl;
	}
	else
	{
		std::cout << "Down ray test FAILED." << std::endl;
	}
}

void TestGame::testGhostRecenter(void) {
	Ghost testGhost(250.f, "assets/redGhost.png", CELL_SIZE * 9, CELL_SIZE * 5, 5, Vector2f(1, 0));
	int casesPassed = 0;
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			testGhost.setPosition(Vector2f(10 * i, 10 * j));
			testGhost.reCenter();
			if (testGhost.getPosition().x == 0 && testGhost.getPosition().y == 0) {
				casesPassed++;
			}
			else {
				int a = 0;
			}
		}
	}
	if (casesPassed == 64) {
		std::cout << "Test case 1 passed!\n";
	}
	else {
		std::cout << "Test case 2 failed!\n";
	}
	return;
}

void TestGame::testPacmanTravelMiddlePath(void) {
	Texture mouthStates; // 3 different pacman mouth states for animation
	mouthStates.loadFromFile("assets\\PacmanSprites.png");
	Pacman pac(&mouthStates);
	pac.setPosition(Vector2f(45,45));
	pac.setDirection(Direction::LEFT);
	int casesPassed = 0;
		for (int j = 1; j < 8; j++) {
			pac.setPosition(Vector2f(45, 10 * j));
			pac.travelMiddlePath();
			if (pac.getPosition().x == 45 && pac.getPosition().y == 45) {
				casesPassed++;
			}
		}
	pac.setDirection(Direction::DOWN);
	pac.setPosition(Vector2f(45, 45));
	for (int i = 1; i < 8; i++) {
			pac.setPosition(Vector2f(10 * i, 45));
			pac.travelMiddlePath();
			if (pac.getPosition().x == 45 && pac.getPosition().y == 45) {
				casesPassed++;
			}
	}
	if (casesPassed == 14) {
		std::cout << "Test case 2 passed!\n";
	}
	else {
		std::cout << "Test case 2 failed!\n";
	}
	return;

}

void TestGame::testPacmanWallCollision(void)
{
	int arr[MAP_HEIGHT][MAP_WIDTH] = { {1} };
	for (int i = 0; i < MAP_HEIGHT; i++) { // Sets every element in the map array to 1.
		for (int j = 0; j < MAP_WIDTH; j++) {
			arr[i][j] = 1;
		}
	}

	GameMap map(arr);
	Texture t;
	t.loadFromFile("assets\\PacmanSprites.png");
	Pacman pac(&t);

	if (pac.isWallCollision(map) == 1)
		cout << "Pacman wall collision test successful." << endl;
	else
		cout << "Pacman wall collision test failed." << endl;
}

void TestGame::testImportFont(void)
{
	Font mFont;
	if (mFont.loadFromFile("assets\\pacfont.TTF")) // Tests if the font was successfully loaded
		cout << "Import font test successful." << endl;
	else
		cout << "Import font test failed." << endl;
}