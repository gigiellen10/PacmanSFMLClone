/* Programmer name: Genevieve Kochel
Date created: April 10th, 2024 
Date last modified: May 22nd, 2024
File purpose: this file contains main() */

#include "Pacman.hpp"
#include "Ghost.hpp"

int main()
{

    RenderWindow window(VideoMode(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS), "Genevieve's Pacman Clone!!");
    window.setFramerateLimit(60); // normalize the framerate to 60 fps

    bool isWon = false, // if game won
        pacDeath = false, // if pac dies
        justDied = false;

        int frameCounter = 0, // incremented at every iteration of game loop (each frame) - used for animation
            isPeletEaten = 0, // if pac eats a pelet
            level = 1; // curr level user is on

        Texture mouthStates, 
        ghostStates; 
       
        // load pacman and ghost states from file for animation
        mouthStates.loadFromFile("assets\\pacAnimationStates.png"); 
        ghostStates.loadFromFile("assets\\GhostStates.png");

        Font scoreFont; // font for displaying score in top left corner
        scoreFont.loadFromFile("assets\\emulogic-font\\Emulogic-zrEw.ttf");
        
        // ghost prison gate
        RectangleShape gate(Vector2f(180.f, 15.f));

        Pacman pac(&mouthStates); // init pacman with mouth state texture
        
        Ghost clyde(&ghostStates, (float)GHOST_SPAWN_X_O, (float)GHOST_SPAWN_Y, 4),
            pinky(&ghostStates, (float)GHOST_SPAWN_X_P, (float)GHOST_SPAWN_Y, 1),
            inky(&ghostStates, (float)GHOST_SPAWN_X_B, (float)GHOST_SPAWN_Y, 2),
            blinky(&ghostStates, (float)GHOST_SPAWN_X_R, (float)GHOST_SPAWN_Y, 3);

        Clock deltaClock, // for calculating delta time, time elapsed per this level;
            prisonClock, // for delaying ghost release from prison
            deathTimer;

        Time deltaTime;

        // array to load main game maze
        // 1 = wall, 0 = free path, 2 = intersection point, 3 = prison cell/shouldn't contain a pelet 
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

            /* UPDATE STATE OF CHARACTERS AND/OR BOARD */
            gate.setPosition(900, 360);

            // update pac
            pac.movement(deltaTime, map);
        
            // update ghosts - if alive, move as normal; if dead, initiate death animation/sequence
            if (blinky.getIsAlive())
            {
                blinky.update(deltaTime, prisonClock, map,
                    Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                    Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));
            }
            else
            {
                blinky.dead(deltaTime, map);
                blinky.setJustDied(false);
            }

            if (pinky.getIsAlive())
            {
                pinky.update(deltaTime, prisonClock, map,
                    Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                    Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));
            }
            else
            {
                pinky.dead(deltaTime, map);
                pinky.setJustDied(false);
            }

            if (inky.getIsAlive())
            {
                inky.update(deltaTime, prisonClock, map,
                    Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                    Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));
            }
            else
            {
                inky.dead(deltaTime, map);
                inky.setJustDied(false);
            }

            if (clyde.getIsAlive())
            {
                clyde.update(deltaTime, prisonClock, map,
                    Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                    Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));
            }
            else
            {
                clyde.dead(deltaTime, map);
                clyde.setJustDied(false);
            }

            // determine if pac collided with a pelet
            isPeletEaten = map.updatePelets(pac.getGlobalBounds()); 

            if (isPeletEaten == 1) // pac ate a regular pelet, add 10 to score count
            {
                pac.setScore(pac.getScore() + 10);
            }
            else if (isPeletEaten == 2)
            {
                pac.setScore(pac.getScore() + 50); // power pelet is 50 pts

                
                // initiate frightened mode for ghosts if not on spawn point
                if (!inky.onSpawnPoint())
                    inky.frightened(level);

                if (!blinky.onSpawnPoint())
                    blinky.frightened(level);

                if (!pinky.onSpawnPoint())
                    pinky.frightened(level);

                if (!clyde.onSpawnPoint())
                    clyde.frightened(level);
            }

            /* CHECK IF CHARACTERS ARE ALIVE */

            if (inky.getMode() == 3 && // if in frightened mode, check if ghost collided with pac
                inky.isDeath(vector<FloatRect>({ pac.getGlobalBounds() }))) 
            {
                inky.setIsAlive(false);
                inky.setJustDied(true);
                pac.setScore(pac.getScore() + 100); // update pac score, eating ghost = +100 pts
            }
            else if (inky.getMode() != 3
                && pac.isDeath(vector<FloatRect>({ inky.getGlobalBounds(), pinky.getGlobalBounds(),
                    blinky.getGlobalBounds(), clyde.getGlobalBounds() }))) // check if pac collided with any ghosts) // if not frightened, pac can die
            {
                pac.setIsAlive(false);
            }

            if (pinky.getMode() == 3 
                && pinky.isDeath(vector<FloatRect>({ pac.getGlobalBounds() })))
            {
                pinky.setIsAlive(false);
                pinky.setJustDied(true);
                pac.setScore(pac.getScore() + 100);
            }
            else if (pinky.getMode() != 3
                && pac.isDeath(vector<FloatRect>({ inky.getGlobalBounds(), pinky.getGlobalBounds(),
                    blinky.getGlobalBounds(), clyde.getGlobalBounds() }))) // check if pac collided with any ghosts) // if not frightened, pac can die
            {
                pac.setIsAlive(false);
            }

            if (blinky.getMode() == 3 
                && blinky.isDeath(vector<FloatRect>({ pac.getGlobalBounds() })))
            {
                blinky.setIsAlive(false);
                blinky.setJustDied(true);
                pac.setScore(pac.getScore() + 100);
            }
            else if (blinky.getMode() != 3 
                && pac.isDeath(vector<FloatRect>({ inky.getGlobalBounds(), pinky.getGlobalBounds(),
                    blinky.getGlobalBounds(), clyde.getGlobalBounds() }))) // check if pac collided with any ghosts) // if not frightened, pac can die
            {
                pac.setIsAlive(false);
            }

            if (clyde.getMode() == 3 
                && clyde.isDeath(vector<FloatRect>({ pac.getGlobalBounds() })))
            {
                clyde.setIsAlive(false);
                clyde.setJustDied(true);
                pac.setScore(pac.getScore() + 100);
            }
            else if (clyde.getMode() != 3
                && pac.isDeath(vector<FloatRect>({ inky.getGlobalBounds(), pinky.getGlobalBounds(),
                    blinky.getGlobalBounds(), clyde.getGlobalBounds() }))) // check if pac collided with any ghosts) // if not frightened, pac can die
            {
                pac.setIsAlive(false);
            }

            
            // if ghost alive, adjust ghost mode and reset speed if mode timer ran out
            // if ghost dead, initiate respawn sequence/animation

            if (inky.getIsAlive())
            {
                inky.checkModeTimer(level);
            }
            else // inky dead
            {
                // increase speed
                inky.setSpeed(400.f);

                // set mode back to default mode - chase
                inky.setMode(1);
            }

            if (pinky.getIsAlive())
            {
                pinky.checkModeTimer(level);
            }
            else // pinky dead
            {
                // increase speed
                pinky.setSpeed(400.f);

                // set mode back to default mode - chase
                pinky.setMode(1);
            }
          
            if (blinky.getIsAlive())
            {
                blinky.checkModeTimer(level);
            }
            else // blinky dead
            {
                // increase speed
                blinky.setSpeed(400.f);

                // set mode back to default mode - chase
                blinky.setMode(1);
            }

            if (clyde.getIsAlive())
            {
                clyde.checkModeTimer(level);
            }
            else // clyde dead
            {

                // increase speed
                clyde.setSpeed(400.f);

                // set mode back to default mode - chase
                clyde.setMode(1);
            }

            /* ANIMATE CHARACTERS */
            pac.animate(frameCounter);

            blinky.animate(frameCounter, map);
            pinky.animate(frameCounter, map);
            inky.animate(frameCounter, map);
            clyde.animate(frameCounter, map);


            /* CHECK IF GAME WON OR LOST */
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
