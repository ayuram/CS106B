/* 
 * File: boggle.cpp
 * ----------------
 * @author Ayush Raman, Summer 2021
 * This file contains code to recursively solve the game 'Boggle'
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/* 
 * This function takes one argument 'str' and returns its point value as an integer
 */
int points(string str) {
    if (str.length() < 3) {
        return 0;
    } else {
        return str.length() - 3;
    }
}

/*
 * This function takes an argument 'board', a GridLocation 'cur', and a set of seen locations 'seenLocs' and returns a set of valid moves
 */
Set<GridLocation> validMoves(Grid<char>& board, GridLocation cur, Set<GridLocation> seenLocs) {
    Set<GridLocation> neighbors = {
        GridLocation(cur.row + 1, cur.col),
        GridLocation(cur.row - 1, cur.col),
        GridLocation(cur.row, cur.col + 1),
        GridLocation(cur.row, cur.col - 1),

        GridLocation(cur.row + 1, cur.col + 1),
        GridLocation(cur.row - 1, cur.col - 1),
        GridLocation(cur.row - 1, cur.col + 1),
        GridLocation(cur.row + 1, cur.col - 1)
    };
    Set<GridLocation> validMoves;
    for(GridLocation neighbor : neighbors) {
        if(board.inBounds(neighbor.row, neighbor.col) && !seenLocs.contains(neighbor)) {
            validMoves.add(neighbor);
        }
    }
    return validMoves;
}

// Helper function to 'scoreBoard()'
int scoreBoardHelper(Grid<char>& board, Lexicon& lex, Set<string>& seenWords, Set<GridLocation> pathLocs, GridLocation currLoc, string currWord, int scoreTotal) {
    if (!lex.containsPrefix(currWord)) {
        return 0; // dead end base case
    } else if (lex.contains(currWord) && !seenWords.contains(currWord) && points(currWord) != 0) {
        seenWords.add(currWord); // add to the set of seen words
        return scoreBoardHelper(board, lex, seenWords, pathLocs, currLoc, currWord, points(currWord)); // recurse for potential words further down the search
    } else {
        Set<GridLocation> moves = validMoves(board, currLoc, pathLocs);
        int tempScore = 0;
        for (GridLocation move : moves) {
            string copy = currWord;
            copy.push_back(board.get(move.row, move.col));
            Set<GridLocation> pathCopy = pathLocs;
            pathCopy.add(move);
            tempScore += scoreBoardHelper(board, lex, seenWords, pathCopy, move, copy, 0); // run a depth search for each valid move
        }
        return tempScore + scoreTotal; // sum up the scores found in the depth searches and the total score
    }
}

/* 
 * This function takes 2 arguments 'board' and a lexicon 'lex' by reference,
 * and using DFS (Depth-First-Search) scores the total points on the board by all valid words contained in it.
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    Set<string> seenWords = {};
    int scoreCount = 0;
    for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            string initial = "";
            Set<GridLocation> seenLocs = {GridLocation(row, col)};
            initial.push_back(board.get(row, col));
            scoreCount += scoreBoardHelper(board, lex, seenWords, seenLocs, GridLocation(row, col), initial, 0);
        }
    }
    return scoreCount;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
