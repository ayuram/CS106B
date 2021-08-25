#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This function takes in 2 arguments : a Grid<bool> 'maze' and a GridLocation 'cur'.
 * Given the arguments, it returns a set of grid locations containing valid moves.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors = {
        GridLocation(cur.row + 1, cur.col),
        GridLocation(cur.row - 1, cur.col),
        GridLocation(cur.row, cur.col + 1),
        GridLocation(cur.row, cur.col - 1)
    };
    Set<GridLocation> validMoves;
    for(GridLocation neighbor : neighbors) {
        if(maze.inBounds(neighbor.row, neighbor.col) && maze[neighbor.row][neighbor.col]) {
            validMoves.add(neighbor);
        }
    }
    return validMoves;
}

/* This function takes in 2 arguments 'maze' and 'path', and throws an error
 * if the path contains a loop, does not start or end at valid points on the maze, or contains invalid moves
 * with respect to the maze.
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeStart = {0, 0};
    Set<GridLocation> seenLocations;
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    GridLocation prev;
    int size = path.size();
    for(int i = 0; i < size; i++) {
        GridLocation curr = path.pop();
        if(i != 0){
            if(!generateValidMoves(maze, prev).contains(curr)){
                error("Invalid move");
            }
        }
        if(seenLocations.contains(curr)) {
            error("Path contains loop");
        }
        seenLocations.add(curr);
        prev = curr;
    }
    if (prev != mazeStart) {
        error("Path does not start at maze start");
    }
}

/* This helper function simply checks whether a given path and maze
 * has valid start and end points in the maze.
 */
bool isValidPath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeStart = {0, 0};
    if(path.peek() != mazeExit) {
        return false;
    }
    int size = path.size();
    GridLocation prev;
    for(int i = 0; i < size; i++) {
        prev = path.pop();
    }
    if(prev != mazeStart) return false;
    return true;
}

/* This helper function checks a path and
 * returns a set of all GridLocations in that path
 */
Set<GridLocation> pathLocations(Stack<GridLocation> path) {
    Set<GridLocation> set;
    int size = path.size();
    for(int i = 0; i < size; i++) {
        set.add(path.pop());
    }
    return set;
}

/* This function takes one argument 'maze' and runs a breadth first search to solve the maze,
 * and return the solved path.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path = {{0, 0}};
    Queue<Stack<GridLocation>> paths;
    paths.enqueue(path);
    while (paths.size() != 0) { // break out if queue is empty
        Stack<GridLocation> curr = paths.dequeue(); // take next path from queue
        MazeGraphics::highlightPath(curr, "blue");
        if (isValidPath(maze, curr)) return curr; // return path if it is valid
        for (GridLocation valid : generateValidMoves(maze, curr.peek())) {
            if(!pathLocations(curr).contains(valid)) { // if the valid move is not inside a grid location already in the path
                Stack<GridLocation> copy = curr; // create a copy of the path
                copy.push(valid); // update the copied path with new move
                paths.enqueue(copy); // add the updated copied path to the queue
            }
        }
    }
    return path; // return empty value (should never reach this ideally)
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("solveMaze on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("maze solver equals actual") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    Stack<GridLocation> actualSol;
    readSolutionFile("res/13x39.soln", actualSol);
    EXPECT_EQUAL(soln, actualSol);
}


