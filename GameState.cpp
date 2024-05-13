
/* Programmer: Genevieve Kochel
Date: April 18th 2024
File purpose: contains various general functions that were not methods of any class */

#include "GameState.hpp"

// displays pacman's current score fancily to the upper left of screen 
void scoreToScreen(RenderWindow& window, Font& font, int currScore)
{
	Text myText(std::to_string(currScore), font, 60); // set string to currScore and font passed in
	myText.setFillColor(Color::White);
	myText.setPosition(20.f, 13.f);
	
	window.draw(myText); // display to window
}

// for scaling vectors by a constant (float value)
const Vector2f operator *(float lhs, Vector2f& rhs)
{
	return Vector2f(rhs.x * lhs, rhs.y * lhs);
}

// for normalizing a vector to compute unit vector (dividing by length)
Vector2f operator /(const Vector2f& lhs, double rhs)
{
	return Vector2f(lhs.x / rhs, lhs.y / rhs);
}

// calculates the length between two points (for our purposes, vector1 and vector2 will be expressed in pixels, not cartesian row/col 
// coordinates
float length(const Vector2f& vector1, const Vector2f& vector2)
{
	return sqrt(pow(vector1.x - vector2.x, 2) + pow(vector2.y - vector1.y, 2));
}


