/*
 * File: sierpinski.cpp
 * ----------------
 * @author Ayush Raman, Summer 2021
 * This file contains code to recursively create Sierpinski triangles to the n'th order.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;


int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order);

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

void fillWhiteTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("white");
    window.fillPolygon( {one, two, three} );
}

GPoint midPoint(GPoint one, GPoint two) {
    return GPoint((one.x + two.x) / 2, (one.y + two.y) / 2);
}

/* This function takes the arguments 'window', the 'GPoint's 'one', 'two', and 'three', and an integer 'order' and
 * then recursively draws the corresponding Sierpinski triangle on the provided window. It also returns the value of the order.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order < 0) {
        error("Negative order");
    } else if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 0;
    } else {
        GPoint mp1 = midPoint(one, two);
        GPoint mp2 = midPoint(two, three);
        GPoint mp3 = midPoint(one, three);
        drawSierpinskiTriangle(window, mp1, mp3, one, order - 1);
        drawSierpinskiTriangle(window, mp2, mp1, two, order - 1);
        drawSierpinskiTriangle(window, mp2, mp3, three, order - 1);
        return order;
    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

