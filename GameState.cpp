
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

