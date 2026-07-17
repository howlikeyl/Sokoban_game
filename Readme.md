# PS4: Sokoban

## Contact
Name: Senghak Heng


## Description
Explain what the project does.

the project is to create a sokoban game in this part we're just creating a structure or an image of the game and make the game playable. In order to win the game the player need to move the box to the assigned location.


### Features
Describe what your major decisions were and why you did things that way.

#### Appraoch
One of the biggest decisions I made was to put all the game logic and rendering into a single Sokoban class that inherits from sf::Drawable. I did this to keep things organized and modular. Instead of cluttering main.cpp with all the logic for movement, drawing, and texture loading, I wanted main.cpp to just handle the window and user input, and let the Sokoban class take care of everything else.

Since i decided to put all the file inside sokoban folder so the command to run the code i use (./Sokoban sokoban/level2.lvl).

I added undo/redo using move history stacks, and reset clears all state to avoid bugs. The player image updates based on direction, and a victory sound plays once when the game is won — both for extra credit. I wrote unit tests to verify core mechanics like pushing, winning, and edge behavior.

### Memory
Describe how you decided to store the level data including whether you used smart pointers.

To support undo and redo, I designed a `Move` struct to track each player action, including optional box movement, and stored these in a `moveHistory` stack. When the player presses undo (`Z`), the last move is popped from `moveHistory` and pushed onto a `redoHistory` stack, allowing it to be reapplied with redo (`Y`). I clear the redo stack whenever a new move is made or the game is reset to prevent invalid replays. This structure keeps the logic modular, avoids bugs from crossing game states, and makes it easy to extend or debug.

### Lambdas
Describe what <algorithm> functions you used and what lambda expressions you wrote.

I used some functions from `<algorithm>` like `count_if`, `find_if`, and `any_of`. These help me check things in the grid more easily. For example, I used `count_if` with a lambda to count how many boxes are on targets. I also used `find_if` to find the player or a box in the grid. Lambdas are small functions I can write inside the code — they make it shorter and easier to read. I used them to check conditions like “is this tile a box?” or “is this tile a target?” without writing a full separate function.

### Issues

Everything work. In this project i learn how to load image from the file and know how to use the command to run the code more effectively unlike previous project wherearea i had trouble with these stuff. 

I also had to debug some tricky issues like undoing into a previous game state after reset. Fixing that taught me how important it is to clear history and manage game state carefully. I learned to think more about what happens behind the scenes when players press keys, and how to make the game feel smooth and bug-free.

## Acknowledgements

SFML
Youtube
StackOverFlow
W3School

freesound (Achievement.wav): https://freesound.org/
Google Fonts (Roboto.ttf): https://fonts.google.com/
Kenney Sokoban Pack (CC0): https://kenney.nl/assets/sokoban
