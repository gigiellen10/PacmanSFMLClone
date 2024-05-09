/* Programmer name: Luka Teodorovic
Date: April 17th 2024
Last modified: April 19th 2024
File purpose: this file contains the function definitons for the MenuScreen and EndScreen classes */

#include "Menus.hpp"

MenuScreen::MenuScreen(RenderWindow& window) : mWindow(window)
{
    // Loads the font from the TrueType file
    mFont.loadFromFile("assets\\pacfont.TTF");

    // Sets the font, size, and position of the title
    mTitle.setFont(mFont);
    mTitle.setString("PAC-MAN");
    mTitle.setCharacterSize(100);
    mTitle.setFillColor(sf::Color::Yellow);
    mTitle.setPosition(630, 150);

    // Sets the font, size and position of the play and exit buttons
    mPlayButton.setFont(mFont);
    mPlayButton.setString("PLAY");
    mPlayButton.setCharacterSize(60);
    mPlayButton.setFillColor(sf::Color::White);
    mPlayButton.setPosition(800, 350);

    mExitButton.setFont(mFont);
    mExitButton.setString("EXIT");
    mExitButton.setCharacterSize(60);
    mExitButton.setFillColor(sf::Color::White);
    mExitButton.setPosition(800, 450);
}

int MenuScreen::handleEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                if (mPlayButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return 1; // Returns true if the play button is clicked
                }
                else if (mExitButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return -1; // Returns false if the exit button is clicked
                    mWindow.close();
                }
            }
        }
    }
}

void MenuScreen::render()
{
    // Renders the menu screen
    mWindow.clear(sf::Color::Blue);
    mWindow.draw(mTitle);
    mWindow.draw(mPlayButton);
    mWindow.draw(mExitButton);
    mWindow.display();
}

EndScreen::EndScreen(sf::RenderWindow& window, bool win) : mWindow(window), mWin(win)
{
    // Loads the font from the TrueType file
    mFont.loadFromFile("assets\\pacfont.TTF");

    if (mWin)
        mText.setString("You Win!");
    else
        mText.setString("Game Over");

    // Sets the font, size, and position of the end screen title
    int xPosition = (win) ? 600 : 500;
    mText.setFont(mFont);
    mText.setCharacterSize(100);
    mText.setFillColor(sf::Color::White);
    mText.setPosition(xPosition, 150);


    // Sets the font, size and position of the play again and exit buttons
    mPlayButton.setFont(mFont);
    mPlayButton.setString("PLAY AGAIN");
    mPlayButton.setCharacterSize(60);
    mPlayButton.setFillColor(sf::Color::White);
    mPlayButton.setPosition(650, 350);

    mExitButton.setFont(mFont);
    mExitButton.setString("EXIT");
    mExitButton.setCharacterSize(60);
    mExitButton.setFillColor(sf::Color::White);
    mExitButton.setPosition(800, 450);
}

int EndScreen::handleEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                if (mPlayButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return 1; // Returns true if the play again button is clicked
                }
                else if (mExitButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    return -1; // Returns false if the exit button is clicked
                    mWindow.close();
                }
            }
        }
    }

}

void EndScreen::render()
{
    // Renders the end screen
    mWindow.clear(sf::Color::Blue);
    mWindow.draw(mText);
    mWindow.draw(mPlayButton);
    mWindow.draw(mExitButton);
    mWindow.display();
}