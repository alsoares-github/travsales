# travsales
Implementation of a solution to the Traveling Salesman problem in 3D with visualization using OpenGL

## Supported platforms
* Linux
* Windows

## Dependencies
* OpenGL
* freeglut

OpenGL should be available in most Linux distributions as well as in Windows. freeglut can be installed in Ubuntu by issuing sudo apt install freeglut3-dev. In Windows, freeglut can be downloaded [here](https://www.transmissionzero.co.uk/software/freeglut-devel/).

## Building instructions
* Linux: inside the project directory, run the command `bash build.sh`.
* Windows: change the include commands to reﬂect whatever directory where you put the freeglut headers. Change the build settings to include OpenGL and the downloaded freeglut as linking dependencies.
