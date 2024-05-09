#pragma once

/* Programmer name: Genevieve Kochel
Date: April 10th 2024
File purpose: include #define constants and common SFML libraries needed throughout the program */


// global constants - width and height of a cell in pixels (proportional to pacman diameter)
#define CELL_SIZE 90

#define MAP_WIDTH_PIXELS 1980 // = 22 game tiles
#define MAP_HEIGHT_PIXELS 990 // = 11 game tiles

#define MAP_WIDTH 23 // cols of game map array
#define MAP_HEIGHT 11 // rows of game map array

#define COLLISION_RAY_H 60 // universal length of HORISONTAL ray for pacman map collisions

#define NUM_INTERSECTIONS 36 // number of intersections where directional decision required for entities

#define PAC_SPAWN_X 1035
#define PAC_SPAWN_Y 855

#define ghostHitboxWidth 80
#define ghostHitboxHeight 80

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector> // for vector of pelets

using std::string;
using std::cout;
using std::endl;

using std::vector;
using sf::Vector2f;
using sf::CircleShape;
using sf::RenderWindow;
using sf::Event;
using sf::Time;
using sf::Clock;
using sf::VideoMode;
using sf::Keyboard;
using sf::Color;
using sf::Text;
using sf::Font;
using sf::FloatRect;
using sf::Sprite;
using sf::Texture;

// commonly used directions that entities can travel along map
namespace Direction
{
	const Vector2f LEFT(-1, 0);
	const Vector2f RIGHT(1, 0);
	const Vector2f DOWN(0, 1);
	const Vector2f UP(0, -1);
	const Vector2f STOP(0, 0);
}

void scoreToScreen(RenderWindow& window, Font &font, int currScore);

