/* Programmer names: Genevieve Kochel, Luka, Jake
Date created: April 10th, 2024 
Date last modified: April 23rd, 2024
File purpose: this file contains main() and any #define constants and sfml includes */


#include "Pacman.hpp"
//#include "Ghost.hpp"
#include "TestGame.hpp"
#include "Menus.hpp"
#include "TestGame.hpp"

int main()
{
   //test cases:
    TestGame game;
    //1: Test Ghost Recenter Function
    game.testGhostRecenter();
    //2. Test Pacman TravelMiddlePath function
    game.testPacmanTravelMiddlePath();
    //3. Test Pacman font import
    game.testImportFont();
    //4. Test Pacman wall collision function
    game.testPacmanWallCollision();


    srand((unsigned int)time(0));
    RenderWindow window(VideoMode(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS), "Pacman!!");
    window.setFramerateLimit(60); // normalize the framrate to 60 fps

    MenuScreen mainMenu(window); // init main menu

    // window loop for starting menu screen
    while (window.isOpen())
    {
        int selection = mainMenu.handleEvents();
        mainMenu.render();
        if (selection == 1) break;
        else if (selection == -1) return 0;
    }

    while (true) // main game loop, sends back to main menu
    {

        bool isPeletEaten = false, isWon = false; 
        int frameCounter = 0; // incremented at every iteration of game loop (each frame) - used for animation

        Texture mouthStates; // 3 different pacman mouth states for animation
        mouthStates.loadFromFile("assets\\PacmanSprites.png");

        Font scoreFont; // font for displaying score in top left corner

        Pacman pac(&mouthStates); // init pacman with mouth state texture

        // init ghosts
       /* Ghost redGhost(250.f, "assets/redGhost.png", CELL_SIZE * 9, CELL_SIZE * 5, 1, Direction::RIGHT);
        Ghost blueGhost(250.f, "assets/blueGhost.png", CELL_SIZE * 10, CELL_SIZE * 5, 2, Direction::UP);
        Ghost whiteGhost(250.f, "assets/whiteGhost.png", CELL_SIZE * 11, CELL_SIZE * 5, 3, Direction::UP);
        Ghost greenGhost(250.f, "assets/greenGhost.png", CELL_SIZE * 12, CELL_SIZE * 5, 4, Direction::LEFT);*/

        Clock deltaClock; // for calculating delta time

        Time deltaTime;

        //load gate sprites
        Texture* gateTexture = new Texture;
        gateTexture->loadFromFile("assets/Gate.png");
        Sprite* gateSpriteLeft = new Sprite;
        Sprite* gateSpriteRight = new Sprite;
        gateSpriteLeft->setTexture(*gateTexture);
        gateSpriteRight->setTexture(*gateTexture);
        gateSpriteLeft->setPosition(900, 360);
        gateSpriteRight->setPosition(990, 360);

        // array to load main game maze
        // 1 = wall, 0 = free path, 2 = intersection point, 3 = shouldn't contain a pelet 
        int myArr[MAP_HEIGHT][MAP_WIDTH]
            = {
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 2, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 2, 1},
                {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
                {1, 0, 1, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 1, 0, 1},
                {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
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
            /*redGhost.moveGhost(deltaTime, map, pac, &ghostsOutOfBox);
            blueGhost.moveGhost(deltaTime, map, pac, &ghostsOutOfBox);
            whiteGhost.moveGhost(deltaTime, map, pac, &ghostsOutOfBox);
            greenGhost.moveGhost(deltaTime, map, pac, &ghostsOutOfBox);*/

            // check for valid ghost placement, recenter if not
            /*if (redGhost.isPositionValid() == false) { redGhost.reCenter(); }
            if (greenGhost.isPositionValid() == false) { greenGhost.reCenter(); }
            if (blueGhost.isPositionValid() == false) { blueGhost.reCenter(); }
            if (whiteGhost.isPositionValid() == false) { whiteGhost.reCenter(); }*/


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
           /* window.draw(redGhost);
            window.draw(blueGhost);
            window.draw(whiteGhost);
            window.draw(greenGhost);*/
            window.draw(*gateSpriteLeft);
            window.draw(*gateSpriteRight);
           
            window.display();

            ++frameCounter; // increment # frames 

            // check if won or lost here and display end screen; get input if want to play again
            if (!pac.getIsAlive() || pac.getScore() == 1090)
            {
                break;
            }
           
        }

        // free heap variables
        delete gateSpriteLeft;
        delete gateSpriteRight;
        delete gateTexture;
            
        EndScreen endScreen(window, isWon); // create end screen based on win or loss
        while (window.isOpen())
        {
            int selection = endScreen.handleEvents(); // determine if player clicked play again or wants to exit (1 or -1)
            endScreen.render(); // display end screen

            if (selection == 1) break;
            else if (selection == -1) return 0;
        }

    }

    
    return 0;
}
