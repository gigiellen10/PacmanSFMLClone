/* Programmer name: Genevieve Kochel 
Date: April 14th 2024
File: this file contains the line segment class definition - line segments used for collision checking */

#pragma once

#include "GameState.hpp"

class LineSegment {

public:
	
	// constructor - default is pacman spawn pt
	LineSegment(const Vector2f& rayStart = Vector2f(PAC_SPAWN_X, PAC_SPAWN_Y), const Vector2f& rayEnd = Vector2f(PAC_SPAWN_X, PAC_SPAWN_Y))
	{
		mRayStart = rayStart;
		mRayEnd = rayEnd;
	}

	// getters
	Vector2f getStart() const { return mRayStart; }
	Vector2f getEnd() const { return mRayEnd; }

	// setters
	void setStart(const Vector2f &newStart) { mRayStart = newStart; }
	void setEnd(const Vector2f &newEnd) { mRayEnd = newEnd; }

private:
	Vector2f mRayStart; // pt at start of ray
	Vector2f mRayEnd; // pt at end of ray
};