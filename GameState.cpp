/* Programmer: Genevieve Kochel
Date: April 18th 2024
File purpose: contains various general functions that were not methods of any class */

#include "GameState.hpp"

// displays pacman's current score fancily to the upper left of screen 
void scoreAndLevelToScreen(RenderWindow& window, Font& font, int currScore, int level)
{
	Text ScoreText("score " + std::to_string(currScore), font, 60),  // set string to currScore and font passed in
		levelText("level " + std::to_string(level), font, 60);

	levelText.setFillColor(Color::White);
	ScoreText.setFillColor(Color::White);

	levelText.setPosition(1460.f, 13.f);
	ScoreText.setPosition(20.f, 13.f);
	
	window.draw(ScoreText); // display to window
	window.draw(levelText);
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

//// purpose: displays the pacman start screen with retro look; allows player to start or exit
//int displayStartScreen(RenderWindow& window, Texture* titleTexture, Font& pacFont)
//{
//	int frameCounter = 0;
//
//	enum DecisionType { exit, play, undecided };
//	DecisionType userDecision = undecided;
//
//	CircleShape arrow(32.f, 3);
//
//	RectangleShape mainTitle(Vector2f(990, 247));
//	mainTitle.setTexture(titleTexture);
//	mainTitle.setPosition(Vector2f(479, 160));
//
//	Text selectionDirs("use the up, down and enter keys to make a selection.", pacFont, 25),
//		playDir("start", pacFont, 50),
//		exitDir("exit", pacFont, 50);
//
//	selectionDirs.setPosition(Vector2f(310, 450));
//	playDir.setPosition(Vector2f(826, 583));
//	exitDir.setPosition(Vector2f(840, 670));
//	arrow.setPosition(Vector2f(791, 578));
//
//	arrow.setRotation(90);
//		
//
//		while (window.isOpen() && userDecision == undecided)
//		{
//			Event event;
//
//			while (window.pollEvent(event)) // if the game window was selected to be closed
//			{
//				if (event.type == Event::Closed)
//				{
//					userDecision = exit; // so program terminates after exiting function
//					window.close();
//				}
//			}
//
//			// arrow blinking
//			if (frameCounter % 50 < 25)
//				arrow.setFillColor(Color::Black);
//			else
//				arrow.setFillColor(Color::Red);
//
//			// up/down movement of arrow and user selection
//			if (Keyboard::isKeyPressed(Keyboard::Up))
//				arrow.setPosition(Vector2f(791, 578));
//			else if (Keyboard::isKeyPressed(Keyboard::Down))
//				arrow.setPosition(Vector2f(815, 665));
//			else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected play
//				&& arrow.getPosition().y == 578)
//			{
//				arrow.setFillColor(Color::Red);
//				userDecision = play;
//			}
//			else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected exit
//				&& arrow.getPosition().y == 665)
//			{
//				arrow.setFillColor(Color::Red);
//				userDecision = exit;
//			}
//
//			// clear window
//			window.clear();
//
//			// draw objects
//			window.draw(mainTitle);
//			window.draw(selectionDirs);
//			window.draw(playDir);
//			window.draw(exitDir);
//			window.draw(arrow);
//
//
//			window.display();
//
//			++frameCounter;
//		}
//
//		std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep then exit 
//
//		return userDecision;
//}
//
//// purpose: displays a winning message screen with player's score; prompts if player wants to play again
//int displayWonScreen(int score, RenderWindow& window, Font& pacFont)
//{
//	enum DecisionType { exit, play, undecided };
//	DecisionType userDecision = undecided;
//
//	int frameCounter = 0;
//
//	// text to display 
//	Text wonMsg("you won!", pacFont, 75),
//		finalScoreMsg("final score", pacFont, 50),
//		scoreTxt(std::to_string(score), pacFont, 50),
//		playAgainMsg("play again", pacFont, 45),
//		exitMsg("exit", pacFont, 45);
//
//	CircleShape arrow(30.f, 3); // points to yes or no selections
//
//	// position text/objects
//	wonMsg.setPosition(Vector2f(653, 206));
//	finalScoreMsg.setPosition(Vector2f(545, 387));
//	scoreTxt.setPosition(Vector2f(1135, 387));
//	playAgainMsg.setPosition(Vector2f(730, 570));
//	exitMsg.setPosition(Vector2f(840, 660));
//	arrow.setPosition(Vector2f(690, 565));
//
//	arrow.setRotation(90);
//	scoreTxt.setFillColor(Color::Yellow);
//
//
//	while (window.isOpen() && userDecision == undecided)
//	{
//		Event event;
//
//		while (window.pollEvent(event)) // if the game window was selected to be closed
//		{
//			if (event.type == Event::Closed)
//			{
//				userDecision = exit;
//				window.close();
//			}	
//		}
//
//		// arrow blinking
//		if (frameCounter % 50 < 25)
//			arrow.setFillColor(Color::Black);
//		else
//			arrow.setFillColor(Color::Yellow);
//
//		// up/down movement of arrow and user selection
//		if (Keyboard::isKeyPressed(Keyboard::Up))
//			arrow.setPosition(Vector2f(690, 565));
//		else if (Keyboard::isKeyPressed(Keyboard::Down))
//			arrow.setPosition(Vector2f(780, 655));
//		else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected play
//			&& arrow.getPosition().y == 565)
//		{
//			arrow.setFillColor(Color::Yellow);
//			userDecision = play;
//		}
//		else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected exit
//			&& arrow.getPosition().y == 655)
//		{
//			arrow.setFillColor(Color::Yellow);
//			userDecision = exit;
//		}
//
//
//		window.clear();
//
//		// draw and display objects
//		window.draw(wonMsg);
//		window.draw(finalScoreMsg);
//		window.draw(scoreTxt);
//		window.draw(playAgainMsg);
//		window.draw(exitMsg);
//		window.draw(arrow);
//
//
//		window.display();
//
//		++frameCounter;
//	}
//
//	std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep then exit
//
//	return userDecision;
//}
//
//// purpose: displays a loosing message screen; prompts if player wants to play again
//int displayLostScreen(RenderWindow& window, Font& pacFont)
//{
//	enum DecisionType { exit, play, undecided };
//	DecisionType userDecision = undecided;
//
//	int frameCounter = 0;
//
//	// text to display 
//	Text gameOverMsg("game over", pacFont, 75),
//		playAgainMsg("play again?", pacFont, 50),
//		yesMsg("yes", pacFont, 45),
//		noMsg("no", pacFont, 45);
//
//	CircleShape arrow(30.f, 3); // points to yes or no selections
//
//	// position text/objects
//	gameOverMsg.setPosition(Vector2f(603, 206));
//	playAgainMsg.setPosition(Vector2f(666, 437));
//	yesMsg.setPosition(Vector2f(855, 570));
//	noMsg.setPosition(Vector2f(885, 660));
//	arrow.setPosition(Vector2f(820, 565));
//
//	arrow.setRotation(90);
//	
//	while (window.isOpen() && userDecision == undecided)
//	{
//		Event event;
//
//		while (window.pollEvent(event)) // if the game window was selected to be closed
//		{
//			if (event.type == Event::Closed)
//			{
//				userDecision = exit;
//				window.close();
//			}
//		}
//
//		// blinking game over msg
//		if (frameCounter % 30 < 15)
//			gameOverMsg.setFillColor(Color::Red);
//		else if (frameCounter % 30 >= 15)
//			gameOverMsg.setFillColor(Color::Black);
//		
//		// slower blinking arrow
//		if (frameCounter % 50 < 25)
//			arrow.setFillColor(Color::Red);
//		else if (frameCounter % 50 >= 25)
//			arrow.setFillColor(Color::Black);
//
//		// arrow movement and selection
//		if (Keyboard::isKeyPressed(Keyboard::Up))
//			arrow.setPosition(Vector2f(820, 565));
//		else if (Keyboard::isKeyPressed(Keyboard::Down))
//			arrow.setPosition(Vector2f(855, 655));
//		else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected play
//			&& arrow.getPosition().y == 565)
//		{
//			gameOverMsg.setFillColor(Color::Red);
//			arrow.setFillColor(Color::Red);
//			userDecision = play;
//		}
//		else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected exit
//			&& arrow.getPosition().y == 655)
//		{
//			gameOverMsg.setFillColor(Color::Red);
//			arrow.setFillColor(Color::Red);
//			userDecision = exit;
//		}
//
//		// clear window before redrawing frame
//		window.clear();
//
//		// draw and display objs
//		window.draw(gameOverMsg);
//		window.draw(playAgainMsg);
//		window.draw(yesMsg);
//		window.draw(noMsg);
//		window.draw(arrow);
//
//		window.display();
//
//		++frameCounter;
//	}
//
//	std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep then exit
//
//	return userDecision;
//}



