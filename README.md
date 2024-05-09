Pacman SFML Final Project - CPTS 122

Project description: This project is a restricted implementation of Pac-Man. 

Team members: Luka Teodorovic, Genevieve Kochel, Jacob Kolk

Our video is linked here: https://drive.google.com/file/d/1NqDgeUTMEdr7PGn_1Non1RAkQb14Ddba/view?usp=sharing

Task allocations:
Genevieve - maze design and implementation, all Pacman collision checking, animation and functionalities, pelet functionality and score counter/animation
Jacob - all Ghost pathfinding and collision checking
Luka - main menu and game over/game won screens, screen display loops 

Inheritance/polymorphism:
- Inheritance was applied primarily through the Pacman and Ghost classes. Pacman inherits publicly from an sf::CircleShape (which also inherits from
  sf::Drawable and sf::Transformable). Ghosts inherit from an sf::Sprite.
- Compile-time polymorphism was applied through operator overloading ([]) in the GameMap class to access the array contained within the class. Run-time
  polymorphism was not applicable in this project because there were no implementations of virtual functions. 
