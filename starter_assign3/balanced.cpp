/*
 * File: balanced.cpp
 * ----------------
 * @author Ayush Raman, Summer 2021
 * This file contains code to recursively check the balance of a given string of operators.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

// This function return 'true' if the inputted character 'c' is an operator.
bool isOperator(char c) {
    switch (c) {
        case '{':
        case '}':
        case '(':
        case ')':
        case '[':
        case ']':
            return true;
        default:
            return false;
    }
}

//This function takes 2 arguments 'start' and 'end' and returns whether they are corresponding operators
bool areOpposite(char start, char end) {
    if (start == '{' && end == '}') {
        return true;
    } else if (start == '[' && end == ']') {
        return true;
    } else if (start == '(' && end == ')') {
        return true;
    } else {
        return false;
    }
}

/*
 * This function takes one argument 'str' and using recursion, returns a new string with the only the operators from 'str'.
 */
string operatorsFrom(string str) {
    if (str.length() == 0) {
        return ""; // base case
    } else if (!isOperator(str[0])) {
        return operatorsFrom(str.substr(1)); // if a character is not an operator we toss it out here
    }
    return str[0] + operatorsFrom(str.substr(1)); // recursively append new operators to the string

}

// Helper function to 'operatorsAreMatched()',
// This function takes two arguments 'index' and a string 'clean' and returns the index at which the operand directly meets its closing bracket through recursion.
int indexOfRemoval(int index, string clean) {
        int length = clean.length();
        if (index == length) {
            return -1; // couldn't find anything
        }
        if (length <= 2 && !areOpposite(clean[index], clean[index + 1])) {
            return -1; // if we are on the last 2 characters and they are not opposites, we return -1 to indicate we couldn't find anything
        }
        if (areOpposite(clean[index], clean[index +1])) {
            return index; // return the index if we found the opposite matching pair
        }
        return indexOfRemoval(index + 1, clean); // recursive case
}

/*
 * This function takes one argument 'ops' of operators and returns true if the operators in the string are all matched
 */
bool operatorsAreMatched(string ops) {
    if (ops.length() == 0) {
        return true; // reached the end of the string succesfully (base case)
    } else {
        int index = indexOfRemoval(0, ops);
        if(index == -1) {
            return false; // if no matching pairs could be found, return false
        }
        ops.erase(index, 2); // otherwise, erase the 2 characters starting at the found index
        return operatorsAreMatched(ops); // recursive case
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
