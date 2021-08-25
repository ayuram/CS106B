/*This file contains the functions necessary to find Euclid's perfect numbers
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush; // progress bar
        }
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
long smarterSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor + (divisor != 1 ? (n/divisor) : 0);
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 */
bool isPerfectSmarter(long n) {
    return (n > 0) && (n == smarterSum(n));
}

/* This function is an improved version of 'findPerfects()',
 * which takes one argument 'stop' and effeciently iterates
 * through the range 1 to 'stop' checking to see if the number is perfect
 * and printing to the console if it is.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush; // progress bar
        }
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes one argument 'n'
 * and returns whether the argument 'n' is a prime number
 */
bool isPrime(long n){
    if(n < 2) return false;
    for (int i = 2; i < n; i++){
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

/* This function takes one argument 'n' and returns the nth perfect Euclid number
 * Note: the implementation is system confined and may have varying limitations based
 * on the system ( as per the assignment details and through testing of my own :) )
 */
long findNthPerfectEuclid(long n) {
    if(n <= 0) return 0;
    long k = 1;
    long m = pow(2, k) - 1;
    int perfectNumbersFound = 0;
    while(true) {
        if(isPrime(m)){
            perfectNumbersFound ++;
            if(perfectNumbersFound == n){
                return pow(2, (k - 1)) * (pow(2, k) - 1);
            }
        }
        k++;
        m = pow(2, k) - 1;
    }
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Oddballs, zero, one, negative") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
    EXPECT(!isPerfect(-6));
}

PROVIDED_TEST("33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time multiple trials of findPerfects on doubling input sizes") {
    cout << endl;
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
//    TIME_OPERATION(80000, findPerfects(80000));
//    TIME_OPERATION(160000, findPerfects(160000));
//    TIME_OPERATION(320000, findPerfects(320000));
//    TIME_OPERATION(640000, findPerfects(640000));
}


STUDENT_TEST("Custom Negative Test") {
    cout << endl;
    EXPECT(!isPerfect(-6));
    EXPECT(!isPerfect(-3));
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-28));
}

STUDENT_TEST("Positive non-perfect number test"){
    EXPECT(!isPerfect(5));
}

STUDENT_TEST("Smarter Sum"){
    EXPECT(isPerfectSmarter(28));
    EXPECT(isPerfectSmarter(33550336));
    EXPECT(!isPerfectSmarter(-6));
    EXPECT(!isPerfect(1));
    EXPECT(!isPerfectSmarter(0));
}

STUDENT_TEST("Find nth"){
    EXPECT(isPerfect(findNthPerfectEuclid(5)));
    EXPECT(isPerfect(findNthPerfectEuclid(4)));
    EXPECT(isPerfect(findNthPerfectEuclid(3)));
    EXPECT(isPerfect(findNthPerfectEuclid(2)));
    EXPECT(isPerfect(findNthPerfectEuclid(1)));
}

STUDENT_TEST("Time multiple trials of findPerfects on doubling input sizes") {
    cout << endl;
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(40000, findPerfectsSmarter(40000));
    TIME_OPERATION(80000, findPerfectsSmarter(80000));
    TIME_OPERATION(160000, findPerfectsSmarter(160000));
    TIME_OPERATION(320000, findPerfectsSmarter(320000));
    TIME_OPERATION(640000, findPerfectsSmarter(640000));
}
