/*
 * File: merge.cpp
 * ----------------
 * @author Ayush Raman, Summer 2021
 * This file contains code to merge queues of integers using binary merge.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function takes 2 sorted Queues of integers 'a' and 'b' and returns the sorted merged Queue.
 */
Queue<int> merge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    int prevA = 0;
    if(a.size() != 0) prevA = a.peek();
    int prevB = 0;
    if(b.size() != 0) prevB = b.peek();
    while (a.size() != 0 || b.size() != 0) {
        if(a.size() == 0 && b.size() != 0) {
            if(prevB > b.peek()) {
                error("Queue B is not sorted");
            }
            result.enqueue(b.dequeue());
        } else if (a.size() != 0 && b.size() == 0) {
            if(prevA > a.peek()) {
                error("Queue A is not sorted");
            }
            result.enqueue(a.dequeue());
        } else {
            if(prevA > a.peek()) {
                error("Queue A is not sorted");
            }
            if(prevB > b.peek()) {
                error("Queue B is not sorted");
            }
            if (a.peek() < b.peek()) {
                result.enqueue(a.dequeue());
            } else {
                result.enqueue(b.dequeue());
            }
        }
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined merge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> multiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = merge(q, result);
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined merge function and makes use of this function to
 * recursively merge a collection of sequences through divide and conquer.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Vector<Queue<int>> left = all.subList(0, all.size() / 2);
    Vector<Queue<int>> right;
    if (all.size() % 2 == 0) {
        right = all.subList(all.size() / 2, all.size() / 2);
    } else {
        right = all.subList((all.size() / 2), all.size() / 2 + 1);
    }
    if (all.size() == 1) { // base case
        if(left.size() > 0) {
            return left[0];
        } else {
            return right[0];
        }
    } else {
        return merge(recMultiMerge(left), recMultiMerge(right)); // recursive case
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("Test binary merge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

PROVIDED_TEST("Test multiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(multiMerge(all), expected);
}

PROVIDED_TEST("Test recMultiMerge by compare to multiMerge") {
    int n = 1000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(multiMerge(all), recMultiMerge(all));
}

PROVIDED_TEST("Time binary merge operation") {
    int n = 1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

PROVIDED_TEST("Time multiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

STUDENT_TEST("Merge timing") {
    int n = 1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));

    n = 3000000;
    a = createSequence(n);
    b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));

    n = 6000000;
    a = createSequence(n);
    b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));

    n = 12000000;
    a = createSequence(n);
    b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));

    n = 24000000;
    a = createSequence(n);
    b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

STUDENT_TEST("Merge 0 Queues") {
    Vector<Queue<int>> empty = {};
    EXPECT_EQUAL(multiMerge(empty).size(), 0);
}

STUDENT_TEST("Merge many empty queues") {
    Vector<Queue<int>> manyEmpty = {};
    manyEmpty.add({});
    manyEmpty.add({});
    manyEmpty.add({});
    manyEmpty.add({});
    manyEmpty.add({});
    manyEmpty.add({});
    manyEmpty.add({});
    EXPECT_EQUAL(multiMerge(manyEmpty).size(), 0);
}

STUDENT_TEST("Multi Merge Timing (Fixed N)") {
    for (int i = 1100; i < 10000; i*=2) {
        int n = 11000;
        int k = i;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(k, multiMerge(all));
    }
}

STUDENT_TEST("Multi Merge Timing (Fixed K)") {
    for (int i = 11000; i < 100000; i*=2) {
        int n = i;
        int k = 1100;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), multiMerge(all));
    }
}

STUDENT_TEST("Multi Merge Timing both variable") {
    for (int i = 11000; i < 88000; i*=2) {
        int n = i;
        int k = n/10;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), multiMerge(all));
    }
}

STUDENT_TEST("Rec Merge Timing (Fixed N)") {
    for (int i = 9000; i < 200000; i*=2) {
        int n = 90000;
        int k = i;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(i, recMultiMerge(all));
    }
}

STUDENT_TEST("Rec Merge Timing (Fixed K)") {
    for (int i = 90000; i < 2000000; i*=2) {
        int n = i;
        int k = 9000;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }
}

STUDENT_TEST("Rec Merge Timing both variable") {
    for (int i = 90000; i < 2000000; i*=2) {
        int n = i;
        int k = n/10;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }
}
