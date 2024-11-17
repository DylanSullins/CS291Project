# CS291 Project
### Dylan Sullins
## Course Prerequisite Graph
This project is a graphical representation of a directed graph between courses and their prerequisites. 
## Dependencies
This project requires Qt version 6.8.0, a graphical library for C++. This project was created on Windows 11 Home version 24H2 and compiled with mingw. 
## Installation
After cloning the repository, open a terminal in the Build folder and run the following commands:
### Windows
*qmake*                     : Prepares makefile

*mingw32-make*              : Builds project

*cd release*                : Change directory into the release directory

*windeployqt [executable]*  : Installs necessary .dll files

Run the executable created by mingw32-make. 
### Linux
*qmake*                     : Prepares makefile

*make*                      : Builds project

Run the executable created by make

## Data
The repo contains a file called *Catalog.dat*. This file is formatted in the following manner:
    CATEGORY;NUM;Course Title;Prerequisites sepaparted by commas
The current state of the project allows for the following categories: 
    NONE, COMP-SCI, MATH, STATS, TEST, DEPT, JUNIOR
    TEST, DEPT, and JUNIOR are specific categories for "Test required", "Department consent required", and "Junior status required" respectively.
NUM must be an integer.
Course Title is a string
Prereqs are formatted in the following manner:
    CATEGORY NUM (i.e. MATH 210, COMP-SCI 101)

## Functionality
Each line is read from Catalog.dat and converted to a course object. This course object is then used to create a graphical node on a grid. The course object is then mapped to the node. Edges are then created and added to each course object based on the prerequisites. The directed graph is then displayed on a grid. On hover, an info box appears with course information and the immediate prerequisite edges are highlighted.
Prerequisites are recursively counted for each object, and the minimum and maximum are recorded in the console. (One consideration: Certain prerequisites are intended to be or, rather than and, but in this simplified representation, they are represented by an and logical relationship. Therefore, the maximum prerequisites in the simplified model is slightly higher than its real-life counterpart.)
