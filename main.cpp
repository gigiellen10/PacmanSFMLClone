/* Programmer name: Genevieve Kochel
Date created: April 10th, 2024 
Date last modified: May 22nd, 2024
File purpose: this file contains main() */

#include "Pacman.hpp"
#include "Ghost.hpp"

int main()
{

    srand((unsigned int)time(NULL));

    RenderWindow window(VideoMode(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS), "Genevieve's Pacman Clone!!");
    window.setFramerateLimit(60); // normalize the framerate to 60 fps

        bool isWon = false, // if game won
            pacDeath = false; 

        int frameCounter = 0, // incremented at every iteration of game loop (each frame) - used for animation
            isPeletEaten = 0, // if pac eats a pelet
            level = 1; // curr level user is on

        Texture mouthStates, // pac mouth open/closed
       tempGhostText; // 3 different pacman mouth states for animation

        // ghost prison gate
        RectangleShape gate(Vector2f(180.f, 15.f));

        Font scoreFont; // font for displaying score in top left corner
        scoreFont.loadFromFile("assets\\emulogic-font\\Emulogic-zrEw.ttf");
        
        mouthStates.loadFromFile("assets\\PacmanSprites.png"); // load pac mouth states
       
        Pacman pac(&mouthStates); // init pacman with mouth state texture
        
        Ghost clyde(&tempGhostText, (float)GHOST_SPAWN_X_O, (float)GHOST_SPAWN_Y, Color::Yellow, 4),
            pinky(&tempGhostText, (float)GHOST_SPAWN_X_P, (float)GHOST_SPAWN_Y, Color::Magenta, 1),
            inky(&tempGhostText, (float)GHOST_SPAWN_X_B, (float)GHOST_SPAWN_Y, Color::Cyan, 2),
            blinky(&tempGhostText, (float)GHOST_SPAWN_X_R, (float)GHOST_SPAWN_Y, Color::Red, 3);

        Clock deltaClock, // for calculating delta time, time elapsed per this level;
            prisonClock; // for delaying ghost release from prison
          

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

            // update and animate pac
            pac.movement(deltaTime, map);
            //pac.animate(frameCounter); // switch between open/closed mouth as pac moves
        
            
            // update ghosts
            blinky.update(deltaTime, prisonClock, map,
                Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));

            pinky.update(deltaTime, prisonClock, map,
                Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));

            inky.update(deltaTime, prisonClock, map,
                Vector2i(getColIndex(pac.getPosition()), getRowIndex(pac.getPosition())), pac.getDirection(),
                Vector2i(getColIndex(blinky.getPosition()), getRowIndex(blinky.getPosition())));

            clyde.update(deltaTime, prisonClock, map,
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
                    inky.setModeTimer(5);
                    inky.setSpeed(GHOST_FRIGHT_SPEED); 
                    cout << "inky frightened\n";
                }
                if (!blinky.inPrisonBox(map))
                {
                    blinky.setMode(3);
                    blinky.resetModeClock();
                    blinky.setModeTimer(5);
                    inky.setSpeed(GHOST_FRIGHT_SPEED);
                    cout << "blinky frightened\n";
                }
                if (!pinky.inPrisonBox(map))
                {
                    pinky.setMode(3);
                    pinky.resetModeClock();
                    pinky.setModeTimer(5);
                    inky.setSpeed(GHOST_FRIGHT_SPEED);
                    cout << "pinky frightened\n";
                }
                if (!clyde.inPrisonBox(map))
                {
                    clyde.setMode(3);
                    clyde.resetModeClock();
                    clyde.setModeTimer(5);
                    inky.setSpeed(GHOST_FRIGHT_SPEED);
                    cout << "lcyde frightened\n";
                }
            }

            /* CHECK IF CHARACTERS ARE ALIVE */

            if (inky.getMode() == 3 && // if in frightened mode, check if ghost collided with pac
                inky.isDeath(vector<FloatRect>({ pac.getGlobalBounds() }))) 
            {
                inky.setIsAlive(false);
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
                // set speed to double current speed
                inky.setSpeed(300.f);

                // set PrisonDelay to 4 seconds
                inky.setPrisonDelay(4);
            }

            if (pinky.getIsAlive())
            {
                pinky.checkModeTimer(level);
            }
            else // pinky dead
            {
                // set speed to double current speed
                pinky.setSpeed(300.f);

                // set PrisonDelay to 4 seconds
                pinky.setPrisonDelay(4);
            }
          
            if (blinky.getIsAlive())
            {
                blinky.checkModeTimer(level);
            }
            else // blinky dead
            {
                // set speed to double current speed
                blinky.setSpeed(300.f);

                // set PrisonDelay to 4 seconds
                blinky.setPrisonDelay(4);
            }

            if (clyde.getIsAlive())
            {
                clyde.checkModeTimer(level);
            }
            else // clyde dead
            {

                // set speed to double current speed
                clyde.setSpeed(300.f);

                // set PrisonDelay to 4 seconds
                clyde.setPrisonDelay(4);
            }

            /* ANIMATE CHARACTERS */
            pac.animate(frameCounter);

            blinky.animate(frameCounter);
            pinky.animate(frameCounter);
            inky.animate(frameCounter);
            clyde.animate(frameCounter);


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
