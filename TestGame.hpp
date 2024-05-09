#pragma once

/* Programmer: Genevieve Kochel 
Date: April 20th 2024
File purpose: this file contains the test class for various program test functions */

#include "Pacman.hpp" // to test various pacman functions

#include "Ghost.hpp" // to test ghost functions

class TestGame {

	
public:
	void testComputeRayBounds(); // tests function that computes the ray bounds for pac collision checking
	void testGhostRecenter(void);
	void testPacmanTravelMiddlePath(void);
	void testPacmanWallCollision(void);
	void testImportFont(void);
};
