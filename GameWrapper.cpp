/* Programmer name: Genevieve Kochel
Date: May 27th 2024
Last modified: May 27th 2024
File purpose: this file contains the method implementations for the GameWrapper class */

#include "GameWrapper.hpp"

GameWrapper::GameWrapper()
{
    // for init ghosts
    int AI = 1,
        ghostXCoord = 855;

    // allocate and init heap mem
    mWindow = new RenderWindow(VideoMode(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS), "Genevieve's Pacman Clone!!");
    mWindow->setFramerateLimit(60); // normalize the framerate to 60 fps

    mPacmanAnimation = new Texture;
    mGhostAnimation = new Texture;
    mLogoHeader = new Texture;
    mFont = new Font; // pacman font used for score and text display

    // load textures and fonts from files
    mPacmanAnimation->loadFromFile("assets\\pacAnimationStates.png");
    mGhostAnimation->loadFromFile("assets\\GhostStates.png");
    mLogoHeader->loadFromFile("assets\\pacmanHeader.png");
    mFont->loadFromFile("assets\\emulogic-font\\Emulogic-zrEw.ttf");

    mPac = new Pacman(mPacmanAnimation);
    
    // init 4 ghosts
    for (int i = 0; i < 4; ++i, ++AI)
    {
        mGhosts[i] = new Ghost(mGhostAnimation, (float)ghostXCoord, (float)GHOST_SPAWN_Y, AI);

        ghostXCoord += 90; // increment by 90 pix so ghosts spaced correctly
    }

    // array to load main game maze
    // 1 = wall, 0 = free path, 2 = intersection point, 3 = prison cell/shouldn't contain a pelet 
        int mapOutline[MAP_HEIGHT][MAP_WIDTH]
            = {
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 2, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 2, 1},
                {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
                {1, 0, 1, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 1, 0, 1},
                {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 3, 3, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
                {1, 2, 0, 2, 0, 0, 0, 2, 1, 3, 3, 3, 3, 1, 2, 0, 0, 0, 2, 0, 2, 1},
                {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
                {1, 0, 1, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 1, 0, 1},
                {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
                {1, 2, 0, 0, 0, 2, 1, 2, 0, 0, 0, 3, 0, 0, 2, 1, 2, 0, 0, 0, 2, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      };

      mMap = new GameMap(mapOutline);

      mLevel = 1; // player starts on level 1
      mScore = 0;
      mFrameCounter = 0; 
}

GameWrapper::~GameWrapper()
{
    // deallocate all vars
    delete mWindow;
    delete mPacmanAnimation;
    delete mGhostAnimation;
    delete mLogoHeader;
    delete mFont;
    delete mPac;
    delete mMap;

    for (int i = 4; i < 4; ++i)
    {
        delete mGhosts[i];
    }

}

// purpose: resets the state of the game per each level or if player chooses play again after loosing
void GameWrapper::reset()
{
    int AI = 1,
        ghostXCoord = 855;

    // reset by deleting and re allocating pacman, the map, and the ghosts
    delete mPac;
    delete mMap;

    // refill ghost vector
    for (int i = 0; i < 4; ++i, ++AI)
    {
        // delete and init new ghost to reset all attributes
        delete mGhosts[i];

        mGhosts[i] = new Ghost(mGhostAnimation, (float)ghostXCoord, (float)GHOST_SPAWN_Y, AI);

        ghostXCoord += 90; // increment by 90 pix so ghosts spaced correctly
    }

    mPac = new Pacman(mPacmanAnimation);

    int mapOutline[MAP_HEIGHT][MAP_WIDTH]
        = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 2, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 2, 1},
            {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
            {1, 0, 1, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 1, 0, 1},
            {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 3, 3, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
            {1, 2, 0, 2, 0, 0, 0, 2, 1, 3, 3, 3, 3, 1, 2, 0, 0, 0, 2, 0, 2, 1},
            {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
            {1, 0, 1, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 1, 0, 1},
            {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
            {1, 2, 0, 0, 0, 2, 1, 2, 0, 0, 0, 3, 0, 0, 2, 1, 2, 0, 0, 0, 2, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    mMap = new GameMap(mapOutline);

    // may need to reset frame counter?
}

void GameWrapper::runGame(int* gameWonOrLoss)
{
    int typePeletEaten = 0; // if pac eats a pelet
        
    bool pacAnimationDone = false, 
        playing = true; // controlls main game loop, when runGame() returns

    /* INIT MAIN GAME VARS */

    Clock deltaClock, // for calculating delta time
        prisonClock, // for delaying ghost release from prison
        deathTimer;

    Time deltaTime;

    /* LOOP THAT RUNS MAIN GAME */

    while (mWindow->isOpen() && playing /*&& pacAnimationDone*/)
    {

        vector<FloatRect> ghostPositions; // used to check pacman death

        Event event;

        while (mWindow->pollEvent(event)) // if the game window was selected to be closed
        {
            if (event.type == Event::Closed)
            {
                *gameWonOrLoss = 0; // exit game loop
                mWindow->close();
            }
        }

        // calculate delta time (time since clock started)
        deltaTime = deltaClock.restart();

        /* UPDATE STATE OF CHARACTERS AND/OR BOARD */
        
        // update pac
        if (mPac->getIsAlive()) // move if he's alive
            mPac->movement(deltaTime, *mMap);

        // reset justDied variable for this frame if true
        if (mPac->getJustDied())
            mPac->setJustDied(false);

        // update ghosts - if alive and pac isn't dead, move as normal; if dead, initiate death animation/sequence
        for (auto i : mGhosts)
        {
            
            if (i->getIsAlive())
            {
                i->update(deltaTime, prisonClock, *mMap,
                    Vector2i(getColIndex(mPac->getPosition()), getRowIndex(mPac->getPosition())), mPac->getDirection(),
                    Vector2i(getColIndex(getBlinkyPosition()), getRowIndex(getBlinkyPosition())));
            }
            else
            {
                i->dead(deltaTime, *mMap);
                i->setJustDied(false);
            }

            // get ghost global bounds
            ghostPositions.push_back(i->getGlobalBounds());
        }

        // determine if pac collided with a pelet
        typePeletEaten = mMap->updatePelets(mPac->getGlobalBounds(), mFrameCounter);

        if (typePeletEaten == 1) // pac ate a regular pelet, add 10 to score count
            mScore += 10;
           

        else if (typePeletEaten == 2)
        {
            mScore += 50; // power pelet is 50 pts


            // initiate frightened mode for ghosts if not on spawn point
            for (auto i : mGhosts)
            {
                if (!i->onSpawnPoint())
                    i->frightened(mLevel);
            }

        }

        /* CHECK IF CHARACTERS ARE ALIVE */
       
        for (auto i : mGhosts)
        {
            
            if (i->getMode() == 3 && // if in frightened mode, check if ghost collided with pac
                i->isDeath(vector<FloatRect>({ mPac->getGlobalBounds() })))
            {
                i->setIsAlive(false);
                i->setJustDied(true);
                mScore += 100; // update pac score, eating ghost = +100 pts
            }
            else if (i->getIsAlive() && i->getMode() != 3
                && mPac->isDeath(vector<FloatRect>(ghostPositions)))
            {
                mPac->setIsAlive(false);
                mPac->setSpeed(0.f); // execute death sequence/animation
                mPac->setJustDied(true);
                mPac->setRotation(0);

                // stop ghosts in their tracks
                mGhosts[0]->setSpeed(0.f);
                mGhosts[1]->setSpeed(0.f);
                mGhosts[2]->setSpeed(0.f);
                mGhosts[3]->setSpeed(0.f);

                *gameWonOrLoss = 3; // return loss value outside of function

            }

        }



        // if ghost alive, adjust ghost mode and reset speed if mode timer ran out
        // if ghost dead, initiate respawn sequence/animation
        for (auto i : mGhosts)
        {
            

            if (i->getIsAlive() && playing != false)
            {
               
                i->checkModeTimer(mLevel, Vector2i(getColIndex(mPac->getPosition()), getRowIndex(mPac->getPosition())), *mMap);
               
            }
            else if (!i->getIsAlive())
            {
                // increase speed
                i->setSpeed(400.f);

                // set mode back to default mode - chase
                i->setMode(1);
            }
        }

        // is pac ate all the pelets, won the game!
        if (mMap->getNumPelets() == 0)
        {
            ++mLevel; // increment level 
            playing = false; // exit main loop 
        }

        if (mLevel > 10) // 10th level is last level, player wins after level 10
        {
            playing = false; // break out of game loop
            *gameWonOrLoss = 2;
        }

        /* ANIMATE CHARACTERS */
        pacAnimationDone = mPac->animate(mFrameCounter);

        if (pacAnimationDone)
            playing = false; // pac death sequence completed, break out of gameloop
        for (auto i : mGhosts)
        {
            
            i->animate(mFrameCounter, *mMap);
            
        }

        /* CLEAR WINDOW AND DRAW NEW GAMESTATE */
        mWindow->clear();

        // display map
        mMap->displayMap(*mWindow, *mFont, mScore);

        // draw characters
        mWindow->draw(*mPac);

        for (auto i : mGhosts)
        {
            mWindow->draw(*i);
        }

        mWindow->display();

        ++mFrameCounter; // increment # frames 
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep for 3 seconds before displaying won/loss screen
}

// purpose: displays the pacman start screen with retro look; allows player to start or exit
int GameWrapper::displayStartScreen()
{
    int frameCounter = 0;

    enum DecisionType { exit, play, undecided };
    DecisionType userDecision = undecided;

    CircleShape arrow(32.f, 3);

    RectangleShape mainTitle(Vector2f(990, 247));
    mainTitle.setTexture(mLogoHeader);
    mainTitle.setPosition(Vector2f(479, 160));

    Text selectionDirs("use the up, down and enter keys to make a selection.", *mFont, 25),
        playDir("start", *mFont, 50),
        exitDir("exit", *mFont, 50);

    selectionDirs.setPosition(Vector2f(310, 450));
    playDir.setPosition(Vector2f(826, 583));
    exitDir.setPosition(Vector2f(840, 670));
    arrow.setPosition(Vector2f(791, 578));

    arrow.setRotation(90);


    while (mWindow->isOpen() && userDecision == undecided)
    {
        Event event;

        while (mWindow->pollEvent(event)) // if the game window was selected to be closed
        {
            if (event.type == Event::Closed)
            {
                userDecision = exit; // so program terminates after exiting function
                mWindow->close();
            }
        }

        // arrow blinking
        if (frameCounter % 50 < 25)
            arrow.setFillColor(Color::Black);
        else
            arrow.setFillColor(Color::Red);

        // up/down movement of arrow and user selection
        if (Keyboard::isKeyPressed(Keyboard::Up))
            arrow.setPosition(Vector2f(791, 578));
        else if (Keyboard::isKeyPressed(Keyboard::Down))
            arrow.setPosition(Vector2f(815, 665));
        else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected play
            && arrow.getPosition().y == 578)
        {
            arrow.setFillColor(Color::Red);
            userDecision = play;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected exit
            && arrow.getPosition().y == 665)
        {
            arrow.setFillColor(Color::Red);
            userDecision = exit;
        }

        // clear window
        mWindow->clear();

        // draw objects
        mWindow->draw(mainTitle);
        mWindow->draw(selectionDirs);
        mWindow->draw(playDir);
        mWindow->draw(exitDir);
        mWindow->draw(arrow);


        mWindow->display();

        ++frameCounter;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep then exit 

    return userDecision;
}

// purpose: displays a winning message screen with player's score; prompts if player wants to play again
int GameWrapper::displayWonScreen()
{
    enum DecisionType { exit, play, undecided };
    DecisionType userDecision = undecided;

    int frameCounter = 0;

    // text to display 
    Text wonMsg("you won!", *mFont, 75),
        finalScoreMsg("final score", *mFont, 50),
        scoreTxt(std::to_string(mScore), *mFont, 50),
        playAgainMsg("play again", *mFont, 45),
        exitMsg("exit", *mFont, 45);

    CircleShape arrow(30.f, 3); // points to yes or no selections

    // position text/objects
    wonMsg.setPosition(Vector2f(653, 206));
    finalScoreMsg.setPosition(Vector2f(545, 387));
    scoreTxt.setPosition(Vector2f(1135, 387));
    playAgainMsg.setPosition(Vector2f(730, 570));
    exitMsg.setPosition(Vector2f(840, 660));
    arrow.setPosition(Vector2f(690, 565));

    arrow.setRotation(90);
    scoreTxt.setFillColor(Color::Yellow);


    while (mWindow->isOpen() && userDecision == undecided)
    {
        Event event;

        while (mWindow->pollEvent(event)) // if the game window was selected to be closed
        {
            if (event.type == Event::Closed)
            {
                userDecision = exit;
                mWindow->close();
            }
        }

        // arrow blinking
        if (frameCounter % 50 < 25)
            arrow.setFillColor(Color::Black);
        else
            arrow.setFillColor(Color::Yellow);

        // up/down movement of arrow and user selection
        if (Keyboard::isKeyPressed(Keyboard::Up))
            arrow.setPosition(Vector2f(690, 565));
        else if (Keyboard::isKeyPressed(Keyboard::Down))
            arrow.setPosition(Vector2f(780, 655));
        else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected play
            && arrow.getPosition().y == 565)
        {
            arrow.setFillColor(Color::Yellow);
            userDecision = play;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected exit
            && arrow.getPosition().y == 655)
        {
            arrow.setFillColor(Color::Yellow);
            userDecision = exit;
        }


        mWindow->clear();

        // draw and display objects
        mWindow->draw(wonMsg);
        mWindow->draw(finalScoreMsg);
        mWindow->draw(scoreTxt);
        mWindow->draw(playAgainMsg);
        mWindow->draw(exitMsg);
        mWindow->draw(arrow);


        mWindow->display();

        ++frameCounter;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep then exit

    return userDecision;
}

// purpose: displays a loosing message screen; prompts if player wants to play again
int GameWrapper::displayLostScreen()
{
    enum DecisionType { exit, play, undecided };
    DecisionType userDecision = undecided;

    int frameCounter = 0;

    // text to display 
    Text gameOverMsg("game over", *mFont, 75),
        playAgainMsg("play again?", *mFont, 50),
        yesMsg("yes", *mFont, 45),
        noMsg("no", *mFont, 45);

    CircleShape arrow(30.f, 3); // points to yes or no selections

    // position text/objects
    gameOverMsg.setPosition(Vector2f(603, 206));
    playAgainMsg.setPosition(Vector2f(666, 437));
    yesMsg.setPosition(Vector2f(855, 570));
    noMsg.setPosition(Vector2f(885, 660));
    arrow.setPosition(Vector2f(820, 565));

    arrow.setRotation(90);

    while (mWindow->isOpen() && userDecision == undecided)
    {
        Event event;

        while (mWindow->pollEvent(event)) // if the game window was selected to be closed
        {
            if (event.type == Event::Closed)
            {
                userDecision = exit;
                mWindow->close();
            }
        }

        // blinking game over msg
        if (frameCounter % 30 < 15)
            gameOverMsg.setFillColor(Color::Red);
        else if (frameCounter % 30 >= 15)
            gameOverMsg.setFillColor(Color::Black);

        // slower blinking arrow
        if (frameCounter % 50 < 25)
            arrow.setFillColor(Color::Red);
        else if (frameCounter % 50 >= 25)
            arrow.setFillColor(Color::Black);

        // arrow movement and selection
        if (Keyboard::isKeyPressed(Keyboard::Up))
            arrow.setPosition(Vector2f(820, 565));
        else if (Keyboard::isKeyPressed(Keyboard::Down))
            arrow.setPosition(Vector2f(855, 655));
        else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected play
            && arrow.getPosition().y == 565)
        {
            gameOverMsg.setFillColor(Color::Red);
            arrow.setFillColor(Color::Red);
            userDecision = play;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Enter) // if enter was pressed and selected exit
            && arrow.getPosition().y == 655)
        {
            gameOverMsg.setFillColor(Color::Red);
            arrow.setFillColor(Color::Red);
            userDecision = exit;
        }

        // clear window before redrawing frame
        mWindow->clear();

        // draw and display objs
        mWindow->draw(gameOverMsg);
        mWindow->draw(playAgainMsg);
        mWindow->draw(yesMsg);
        mWindow->draw(noMsg);
        mWindow->draw(arrow);

        mWindow->display();

        ++frameCounter;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep then exit

    return userDecision;
}