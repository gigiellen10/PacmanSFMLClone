
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

float length(const Vector2f& vector)
{
	return pow(vector.x, 2) + pow(vector.y, 2);
}
