#pragma once

/* Programmer name: Luka Teodorovic
Date: April 17th
Last modified: April 19th, by Luka Teodorovic
File purpose: This file contains the class declaration for the MenuScreen and EndScreen classes */

#include "GameState.hpp"

class MenuScreen
{
public:

    // Constructor
    MenuScreen(RenderWindow& window);

    // Event handler
    int handleEvents();

    void render();

private:

    RenderWindow& mWindow;
    Font mFont;
    Text mTitle;
    Text mPlayButton;
    Text mExitButton;

};

class EndScreen
{
public:

    // Constructor
    EndScreen(RenderWindow& window, bool win);

    // Event handler
    int handleEvents();
    
    void render();

private:

    RenderWindow& mWindow;
    Font mFont;
    Text mText;
    Text mPlayButton;
    Text mExitButton;
    bool mWin;

};