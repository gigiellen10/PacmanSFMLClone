/* Programmer names: Genevieve Kochel, Luka, Jake
Date created: April 10th, 2024 
Date last modified: April 23rd, 2024
File purpose: this file contains main() and any #define constants and sfml includes */


#include "Pacman.hpp"


int main()
{

    
    RenderWindow window(VideoMode(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS), "Pacman!!");
    window.setFramerateLimit(60); // normalize the framrate to 60 fps

        bool isPeletEaten = false, isWon = false; 
        int frameCounter = 0; // incremented at every iteration of game loop (each frame) - used for animation

        Texture mouthStates; // 3 different pacman mouth states for animation
        mouthStates.loadFromFile("assets\\PacmanSprites.png");

        Font scoreFont; // font for displaying score in top left corner

        Pacman pac(&mouthStates); // init pacman with mouth state texture

        // init ghosts
       

        Clock deltaClock; // for calculating delta time
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
        bool ghostsOutOfBox = false;


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
            pac.movePacman(deltaTime, map);
            pac.animateMouth(frameCounter); // switch between open/closed mouth as pac moves

            isPeletEaten = map.updatePelets(pac.getGlobalBounds()); // determine if pac collided with a pelet

            if (isPeletEaten) // pac ate a pelet, add 10 to score count
            {
                pac.setScore(pac.getScore() + 10);
            }

            // update state of ghosts


            // check for valid ghost placement, recenter if not



            // check if pacman has collided with a ghost
           /* pac.isDeath(redGhost.getGlobalBounds(), blueGhost.getGlobalBounds(), whiteGhost.getGlobalBounds(), greenGhost.getGlobalBounds());*/

            // check if game was won or lost; else, continue
            if (!pac.getIsAlive())
            {
                isWon = false;
            }
            else if (pac.getScore() == 1090) // winning score
            {
                isWon = true;
            }

            /* CLEAR WINDOW AND DRAW NEW GAMESTATE */
            window.clear();

            // display map
            map.displayMap(window, scoreFont, pac.getScore());

            // draw characters
            window.draw(pac);

            window.display();

            ++frameCounter; // increment # frames 

            // check if won or lost here and display end screen; get input if want to play again
            if (!pac.getIsAlive() || pac.getScore() == 1090)
            {
                break;
            }

        }

      

    
    return 0;
}
