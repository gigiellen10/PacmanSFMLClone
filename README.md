#Pacman Clone
This project was my first ever graphical application--a remake of the classic Pacman game! With a slightly smaller game maze, this implementation emphisises 
the various Ghost AI algorithms, backend collision checking, and efficient handling of asychronous events triggered by user input.

###Link to Demo Video: (link will be posted here).

##How it's made:
###Tech Used: C++, Graphics: SFML (Simple and Fast Multi-Media Library)

Behind the scenes, the game maze is implemented with a simple 2D array that contains GameTiles, each of which contain attributes denoting if a 
wall should be drawn, if entities may make decisions about thier direction of travel, and whether a pelet should be placed in the cell. Ultimately, 
this implementation of the GameBoard allowed for smoother Ghost and Pacman collision checking and pathfinding. 

Each main entity--both Pacman and the Ghosts--inherited from an abstract Character class that contained intuitive attributes like speed, direction of travel, 
and whether the entity was alive. A Character itself inherited from the RectangleShape SFML class which permited smooth animation and graphics. Pacman's methods
of travel were governed by user input, which required a collision checking algorithm to ensure smooth movement while also making sure Pacman couldn't run into 
walls or go into the Ghost prison box. An attribute of the Pacman class, mRay used simple raycasting from Pacman's origin to detect upcoming walls in his path of 
travel. This method of collision checking ended up being the most robust, and was the final result of many refactorings for his movement (explained more thoroughly
below).

The Ghost pathfinding followed the same general algorithm; during game play, the Ghosts could exist in 3 modes: chase, scatter or frightened.
Depending on the Ghost's assigned identity--aka "AI type"--and current mode the Ghost would then select a target tile. The red ghost (blinky), 
targets Pacman directly in chase mode. The orange ghost (clyde) also targets Pacman directly, until it is 8 tiles away from pac in either the horisontal or 
vertical directions. The pink ghost (pinky) always targets the cell 4 tiles in front of Pacman, in an attempt to corner him between blinky and itself. Lastly, 
the blue ghost (inky), chooses the tile 2 cells in front of pacman, draws a vector from Pinky's current position and doubles that vector to land on his final 
target tile. After determining the target tile, the Ghost will evaluate the 4 adjacent cells in each direction and find all valid directions of travel--aka 
not hitting a wall or going back into the prison box. From here, the algorithm draws a vector from each of the adjacent cells in all valid directions and determines 
which vector has the shortest length. The Ghost then travels in that direction. The target tiles vary depending on mode, as the ghosts will run towards the corners 
of the maze in scatter mode and maximize the distance between them and Pacman if they enter frightened mode (when Pacman eats a power pelet). It is also worth noting 
that the Ghosts generally are not aloud to turn around. The one exeption to this rule if if they are switching modes. 

In the instance of Pacman's death, a death animation sequence is initiated and the game is over. If a ghost dies during frightened mode, the target tile becomes the 
spawn tile for that particular ghost within the prison box. Once reached, the ghost will idle before exiting and continuing to persue Pacman. 

##Optimizations
#Pacman Collision Checking
The code that determines Pacman's collision checking was refactored 3 times while developing this game. Originally, Pacman was allowed to go any direction on each
iteration of the game loop and would be transported back to the center of its previous cell if trying to enter a wall. This proved to be very inefficient as
Pacman was allowed to change directions multiple times per each cell, and required checking of all surrounding cells. The usage of "intersection cells" was my next 
attempt at refactoring these checks. An intersection cell was essentially a vertex within the map at which pacman could change directions perpendicularly. This meant
that Pacman was only allowed to change directions on these cells, otherwise he could only flip around 180 degrees to reverse his current direction. This significantly
reduced the amount of position checks needed throughout gameplay. The last addition to Pacman's collision checking was a line segment/ray that would be cast out to detect
if Pacman would hit a wall in his current direction of travel. This check was only necessary if a cell was both an intersection cell and a corner, as his speed would 
need to be reduced to 0 if he were about to run into a wall/corner of the map. 

#Ghost AI
The Ghost pathfinding AI was originally implemented where the Ghost would select a random route to its target cell. This code was optimized through following
a "greedy" approach where the shortest path to a particular cell was calculated and selected. As the Ghosts were often targeting Pacman's position, which changes each 
iteration of the game loop, a Ghost was permitted to make a new direction decision per each new tile it occupied. This code cleaned up the original algorithm, 
which allowed a ghost to make multiple new directional choices per each cell. Ultimately, these changes allowed the game to run faster and with a smoother graphics
display.

##What I Learned
Being my first ever game, the process of handling the backend implementation while considering how to aesthetically present the application was a great learning 
opportunity. This was my first exposure to principles of game development; I learned how to use deltaTime to normalize movement across different environments 
independent of framerate, the basics of rendering graphics and handling asynchronous events throughout gameplay. I found it particularly challenging to track down 
bugs since the environment was less controlled than a typical application. This prompted me to take a different approach to debugging by singling out different components
of the game to narrow down where errors were occuring (ex: focusing on the Ghosts and ommiting Pacman code). This project also provided the opportunity to research
common pathfinding algorithms and weigh the benefits of using one over the other for efficiency. Lastly, this project allowed me to familiarize myself with version
controll, particularly Git. This project started out as a team project where each member was in charge of one element of the project, which required me to learn how 
to efficiently collaborate throughout the bulding process and resolve merge conflicts. (Note: all code in this repo is not from the team project portion and was solely
developed by me). 



