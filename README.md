# travsales
Implementation of a solution to the Traveling Salesman problem in 3D with visualization using OpenGL

## Supported platforms
* Linux
* Windows

## Dependencies
* OpenGL
* freeglut

OpenGL should be available in most Linux distributions as well as in Windows. freeglut can be installed in Ubuntu by issuing sudo apt install freeglut3-dev. In Windows, freeglut can be downloaded [here](https://www.transmissionzero.co.uk/software/freeglut-devel/).

## Build instructions
* Linux: inside the project directory, run the command `bash build.sh`.
* Windows: change the include commands to reﬂect whatever directory where you put the freeglut headers. Change the build settings to include OpenGL and the downloaded freeglut as linking dependencies.

## Running
* Linux: in console, execute the file `travsales` created by the build script.
* Windows: copy the downloaded freeglut `.DLL` file into the build directory and execute the `.EXE` created in the build process.

You can choose to load a custom map from `custom.txt`. If you do not choose this option a 3D map will be generated randomly. There is a default `cities.txt` included, but the map defined there isn't very interesting; it is meant to indicate the format to input a map: each line of the file must contain the three corrdinates of a city in 3D space separated by blanks. For best visualization the cities should be distributed inside a grid of size 100.0 x 100.0 x 100.0. The algorithm is capable of solving a 2D map -- just set every city's third coordinate to 0.

## Notes
The implementation of the Genetic Algorihm used in this project was based on the corresponding chapter of the book _Genetic Algorithms in Java Basics_ by Lee Jacobson.

The _Traveling Salesman Problem_ consists of the following: given a collection of points, finde the shortest closed polygonal path visiting each point exactly once. The path may have self-intersections as long as they're not in the initial points. Intuitively, each point represents a city, and the traveling salesman must find the shortest path to visit every city once and return home. In the 3D case, the salesman travels in space (think traveling between planets in a science fiction galaxy). This is an optimization problem which for a large number of points is unfeasibly complex to be solved by brute force. Genetic Algorithms give one heuristic approach.

_Genetic Algorithms_ are a class of optimization techniques inspired by natural Evolution. A family of candidate solutions (called a _population_) is generated randomly and the individual solutions "strive to survive"; since only the fittest solutions are able to survive and reproduce, each generation yields better and better solutions. 
