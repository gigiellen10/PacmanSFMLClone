/* Programmer name: Genevieve Kochel
Date created: April 10th, 2024 
Date last modified: May 9th, 2024
File purpose: this file contains main() */

#include "Pacman.hpp"
#include "Ghost.hpp"

int main()
{

    srand((unsigned int)time(NULL));

    RenderWindow window(VideoMode(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS), "Genevieve's Pacman Clone!!");
    window.setFramerateLimit(60); // normalize the framrate to 60 fps

        bool isWon = false; // if game won
        int frameCounter = 0, // incremented at every iteration of game loop (each frame) - used for animation
            isPeletEaten = 0, // if pac eats a pelet
            level = 1; // curr level user is on

        Texture mouthStates, // pac mouth open/closed
       tempGhostText; // 3 different pacman mouth states for animation

        // ghost prison gate
        RectangleShape gate(Vector2f(180.f, 15.f));

        Font scoreFont; // font for displaying score in top left corner
        
        mouthStates.loadFromFile("assets\\PacmanSprites.png"); // load pac mouth states
       
        Pacman pac(&mouthStates); // init pacman with mouth state texture
        
        Ghost clyde(&tempGhostText, (float)GHOST_SPAWN_X_O, (float)GHOST_SPAWN_Y, Color::Yellow, 4),
            pinky(&tempGhostText, (float)GHOST_SPAWN_X_P, (float)GHOST_SPAWN_Y, Color::Magenta, 1),
            inky(&tempGhostText, (float)GHOST_SPAWN_X_B, (float)GHOST_SPAWN_Y, Color::Cyan, 2),
            blinky(&tempGhostText, (float)GHOST_SPAWN_X_R, (float)GHOST_SPAWN_Y, Color::Red, 3);

        Clock deltaClock, // for calculating delta time, time elapsed per this level;
            nonResetClock; // nonResetClock is not reset
          

        Time deltaTime;

        // array to load main game maze
        // 1 = wall, 0 = free path, 2 = intersection point, 3 = shouldn't contain a pelet 
        int myArr[MAP_HEIGHT][MAP_WIDTH]
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

        GameMap map(myArr); // construct the game map

        // load score font from file
        scoreFont.loadFromFile("assets\\emulogic-font\\Emulogic-zrEw.ttf");


        /* LOOP THAT RUNS MAIN GAME */

        while (window.isOpen())
        {

            Event event;

            while (window.pollEvent(event)) // if the game window was selected to be closed
            {
                if (event.type == Event::Closed)
                    window.close();
            }

            // calculate delta time (time since clock started)
            Time deltaTime = deltaClock.restart();

            // update state of characters and/or board
            gate.setPosition(900, 360);

            pac.movement(deltaTime, map);
            pac.animateMouth(frameCounter); // switch between open/closed mouth as pac moves
        
            
            // move ghosts
            blinky.update(deltaTime, nonResetClock, map,
                Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));

            pinky.update(deltaTime, nonResetClock, map,
                Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));

            inky.update(deltaTime, nonResetClock, map,
                Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));

            clyde.update(deltaTime, nonResetClock, map,
                Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));

            // determine if pac collided with a pelet
            isPeletEaten = map.updatePelets(pac.getGlobalBounds()); 

            if (isPeletEaten == 1) // pac ate a regular pelet, add 10 to score count
            {
                pac.setScore(pac.getScore() + 10);
            }
            else if (isPeletEaten == 2)
            {
                pac.setScore(pac.getScore() + 50); // power pelet is 50 pts

                
                // initiate and spend 4 seconds in frightened mode for ghosts if not in prison
                if (!inky.inPrisonBox(map))
                {
                    inky.setMode(3);
                    inky.resetModeClock();
                    inky.setModeTimer(4);
                    cout << "inky frightened\n";
                }
                if (!blinky.inPrisonBox(map))
                {
                    blinky.setMode(3);
                    blinky.resetModeClock();
                    blinky.setModeTimer(4);
                    cout << "blinky frightened\n";
                }
                if (!pinky.inPrisonBox(map))
                {
                    pinky.setMode(3);
                    pinky.resetModeClock();
                    pinky.setModeTimer(4);
                    cout << "pinky frightened\n";
                }
                if (!clyde.inPrisonBox(map))
                {
                    clyde.setMode(3);
                    clyde.resetModeClock();
                    clyde.setModeTimer(4);
                    cout << "lcyde frightened\n";
                }
            }

            // adjust ghost mode if mode timer ran out
            inky.checkModeTimer(level);
            blinky.checkModeTimer(level);
            pinky.checkModeTimer(level);
            clyde.checkModeTimer(level);


            // check if pacman has collided with a ghost
           /* pac.isDeath(redGhost.getGlobalBounds(), blueGhost.getGlobalBounds(), whiteGhost.getGlobalBounds(), greenGhost.getGlobalBounds());*/

            // check if game was won or lost; else, continue
            if (!pac.getIsAlive())
            {
                isWon = false;
            }
            //else if (pac.getScore() == 1090) // winning score, need to change based on number power pels
            //{
            //    isWon = true;
            //}

            /* CLEAR WINDOW AND DRAW NEW GAMESTATE */
            window.clear();

            // display map
            map.displayMap(window, scoreFont, pac.getScore());

            // draw characters
            window.draw(pac);
            window.draw(gate);
            window.draw(pinky);
            window.draw(inky);
            window.draw(clyde);
            window.draw(blinky);

            window.display();

            ++frameCounter; // increment # frames 

            // check if won or lost here and display end screen; get input if want to play again
            

        }
    return 0;
}
