/* Programmer name: Genevieve Kochel
Date created: April 10th, 2024 
Date last modified: May 22nd, 2024
File purpose: this file contains main() */

//#include "Pacman.hpp"
//#include "Ghost.hpp"

#include "GameWrapper.hpp"

int main()
{
    
    // controlls whether user wants to play again or exit program
    enum GameState { exit, inProgress, won, lost, playAgain}; // 0 = exit, 1 = inProgress, 2 = won, 3 = lost, 4 = playAgain
    GameState gameStatus = inProgress; 

    int status = 1; // equivalent to enum inProgress

    // create gameWrapper here 
    GameWrapper theGame;

    Clock timeSinceStart; // used to gauge how long to play start music

    /* DISPLAY PACMAN MENU BEFORE ENTERING MAIN LOOP */
    gameStatus = static_cast<GameState>(theGame.displayStartScreen(timeSinceStart)); 


    while (gameStatus == inProgress || gameStatus == playAgain)
    {
        // if mLevel != 1 || gameStatus == lost, reset game state - de allocate and reallocate Pac, Ghosts, GameMap
        if (theGame.getLevel() != 1 || gameStatus == lost || gameStatus == playAgain)
        {
            // reset the state of the game
            theGame.reset(gameStatus);
            gameStatus = inProgress;
        }

        // run game until player looses 
        theGame.runGame(&status);
        gameStatus = static_cast<GameState>(status);

        if (gameStatus == won) // if player won
         gameStatus = static_cast<GameState>(theGame.displayWonScreen());
        
        else if (gameStatus == lost)
         gameStatus = static_cast<GameState>(theGame.displayLostScreen());

    }

    return 0;
}
