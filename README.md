## Tramway STL project
A C++ STL-library project, which was part of my first-year studies in university. This was a pair project, the other contributor is @Real-bay
The program holds a database which stores different tramway stops and lines. Using following commands, the database can be manipulated and printed:
 * QUIT: Terminates the program.
 * LINES: Prints out all lines in alphabetical order.
 * LINE <line>: Prints out all stops on <line>, in distance order.
 * STOPS: Prints out all stops in alphabetical order.
 * STOP <stop>: Prints out the lines containing <stop>.
 * DISTANCE <line> <stop1> <stop2>: Calculates and prints out the distance between <stop1> and <stop2> on <line>.
 * ADDLINE <line>: Adds <line> to the datastructure.
 * ADDSTOP <line> <new stop> <distance>: Adds <new stop> on <line> in the
 * data structure, with the <distance> given.
 * REMOVE <stop>: Removes <stop> from the data structure.
  
  ![SEestKcW7N](https://user-images.githubusercontent.com/70947133/147092171-e9c9df68-4d81-4cc8-a4e0-a538bc79c882.gif)
  
*A simple example of the project running*
  
This project was done with QT Creator, so it might not run as smoothly on other IDEs. This repository also contains an input file which has commands to initialize the database.
This project got 33/50 on grading due to error messages being inefficient and indenting was bad. These problems have been fixed since the grading. This was also the first time we used STL-library this broadly, so the implementations might not have been the best.
  
