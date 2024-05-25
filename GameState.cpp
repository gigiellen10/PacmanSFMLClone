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


const Vector2i operator *(Vector2i& lhs, float rhs)
{
	return Vector2i(lhs.x * rhs, lhs.y * rhs);
}

// for normalizing a vector to compute unit vector (dividing by length)
Vector2f operator /(const Vector2f& lhs, double rhs)
{
	return Vector2f(lhs.x / rhs, lhs.y / rhs);
}

// calculates the length between two points 
double length(const Vector2i& vector1, const Vector2i& vector2)
{
	return sqrt(pow(vector1.x - vector2.x, 2) + pow(vector2.y - vector1.y, 2));
}

// purpose: displays a winning message screen with player's score; prompts if player wants to play again
bool displayWonScreen(int score, RenderWindow& window)
{
	bool userDecision = false; 

	while (window.isOpen())
	{
		window.clear();

		// draw

		window.display();
	}
}

// purpose: displays a loosing message screen; prompts if player wants to play again
bool displayLostScreen(RenderWindow& window)
{
	while (window.isOpen())
	{
		window.clear();

		// draw

		window.display();
	}
}

