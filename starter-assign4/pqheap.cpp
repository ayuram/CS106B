#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/*
 * This constructor intitializes the heap as an array of size 'INITIAL_CAPACITY' along with other helper state variables associated with the array
 */
PQHeap::PQHeap() {
    _numElements = 0;
    _spaceAllocated = INITIAL_CAPACITY;
    _heap = new DataPoint[INITIAL_CAPACITY];
}

/*
 * This destructor deletes the heap from memory
 */
PQHeap::~PQHeap() {
    _numElements = 0;
    _spaceAllocated = 0;
    delete [] _heap;
}

/*
 * This function expands the heap to twice it's original size
 */
void PQHeap::expand() {
    DataPoint* newArr = new DataPoint[_spaceAllocated * 2];
    for (int i = 0; i < _numElements; i++) {
        newArr[i] = _heap[i];
    }
    delete [] _heap; // delete old reference
    _heap = newArr; // assign pointer to new array
    _spaceAllocated *= 2; // increase internal size
}

/*
 * This function takes one argument 'elem' and queues it based on priority, into the heap
 */
void PQHeap::enqueue(DataPoint elem) {
    if (size() >= _spaceAllocated - 1) { // need the minus for the dequeue to operate correctly
        expand();
    }
    _heap[_numElements] = elem;
    int newPosition = _numElements;
    while (true) {
        int parentIndex = getParentIndex(newPosition);
        if (newPosition == 0) {
            break; // new highest priority
        }
        if (_heap[parentIndex].priority < elem.priority) {
            break; // base case
        }
        DataPoint temp = _heap[parentIndex];
        _heap[parentIndex] = elem;
        _heap[newPosition] = temp;
        newPosition = parentIndex; // swap parent and child for bubbling
    }
    _numElements++;
}

/*
 * This function returns the top value of highest priority from the heap, without removing it.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot peek an empty PQHeap");
    }
    return _heap[0];
}

/*
 * This function returns and removes the element of highest priority from the heap, and reorganizes the heap
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("Cannot dequeue from an empty PQHeap");
    }
    DataPoint returnVal = _heap[0]; // value to return
    _heap[0] = _heap[size() - 1];
    int currIndex = 0;
    while (true) {
        int leftIndex = getLeftChildIndex(currIndex);
        int rightIndex = getRightChildIndex(currIndex);
        if (leftIndex > size() - 1 || rightIndex > size() - 1) {
            break; // if we are about to walk out of bounds
        }
        DataPoint left = _heap[leftIndex];
        DataPoint right = _heap[rightIndex];
        DataPoint temp = _heap[currIndex];
        if (temp.priority < left.priority && temp.priority < right.priority) {
            break; // if both children are of lower priority
        }
        if (left.priority < right.priority) {
            _heap[currIndex] = left;
            _heap[leftIndex] = temp;
            currIndex = leftIndex; // swap left with parent
        } else if (right.priority <= left.priority) {
            _heap[currIndex] = right;
            _heap[rightIndex] = temp;
            currIndex = rightIndex; // swap right with parent
        }
    }
    _numElements--; // reduce the number of elements
    return returnVal;
}

/*
 * This function returns a boolean value for whether or not the heap is empty
 */
bool PQHeap::isEmpty() const {
    return _numElements == 0;
}

/*
 * This function returns the size of the heap
 */
int PQHeap::size() const {
    return _numElements;
}

/*
 * This function clears all elements from the heap
 */
void PQHeap::clear() {
    delete [] _heap;
    _heap = new DataPoint[INITIAL_CAPACITY];
    _spaceAllocated = INITIAL_CAPACITY;
    _numElements = 0;
}

/*
 * This function traverses the heap and prints out all of its elements
 */
void PQHeap::printDebugInfo() {
    for (int i = 0; i < size(); i++) {
       cout << "{" << _heap[i].name << ", " <<_heap[i].priority << "}, ";
    }
    cout << endl;
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() {
    printDebugInfo();
    for (int i = 0; i < (size() - 2 / 2); i++) {
        DataPoint curr = _heap[i];
        int leftIndex = getLeftChildIndex(i);
        int rightIndex = getRightChildIndex(i);
        if (leftIndex < size()) {
            DataPoint leftChild = _heap[leftIndex];
            if (curr.priority > leftChild.priority) {
                error("Left child of higher priority");
            }
        }
        if (rightIndex < size()) {
            DataPoint rightChild = _heap[getRightChildIndex(i)];
            if (curr.priority > rightChild.priority) {
                error("Right child of higher priority");
            }
        }

    }
}

/*
 * This function takes one argument 'curIndex' and returns the parent index relating to the 'curIndex'
 */
int PQHeap::getParentIndex(int curIndex) {
    return (curIndex - 1) / 2;
}

/*
 * This function takes one argument 'curIndex' and returns the left child index relating to the 'curIndex'
 */
int PQHeap::getLeftChildIndex(int curIndex) {
    return 2*curIndex + 1;
}

/*
 * This function takes one argument 'curIndex' and returns the right child index relating to the 'curIndex'
 */
int PQHeap::getRightChildIndex(int curIndex) {
    return 2*curIndex + 2;
}

/* * * * * * Test Cases Below This Point * * * * * */

static void fill(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", i });
    }
}

static void empty(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

STUDENT_TEST("PQHeap peek from empty Queue") {
   PQHeap pq;
   EXPECT_ERROR(pq.peek());
}

STUDENT_TEST("PQHeap dequeue from empty Queue") {
   PQHeap pq;
   EXPECT_ERROR(pq.dequeue());
}

STUDENT_TEST("PQHeap dequeued values are of highest priority") {
   PQHeap pq;
   fill(pq, 3);
   empty(pq, 3);
}

STUDENT_TEST("PQHeap Unbalanced") {
   PQHeap pq;
   fill(pq, 7);
   empty(pq, 7);
}

STUDENT_TEST("PQHeap Clear") {
   PQHeap pq;
   fill(pq, 7);
   pq.clear();
   EXPECT_EQUAL(pq.size(), 0);
   pq.validateInternalState();
}

STUDENT_TEST("PQHeap Enqueue") {
   PQHeap pq;
   fill(pq, 7);
   EXPECT_EQUAL(pq.size(), 7);
}

STUDENT_TEST("PQHeap Enqueue Even Number") {
   PQHeap pq;
   pq.enqueue({"A", 1});
   pq.enqueue({"B", 2});
   pq.enqueue({"C", 3});
   pq.enqueue({"D", 4});
   EXPECT_EQUAL(pq.size(), 4);
   pq.dequeue();
   pq.validateInternalState();
   pq.dequeue();
   pq.validateInternalState();
   pq.dequeue();
   pq.validateInternalState();
}




/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

static void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", i });
    }
}

static void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;

    TIME_OPERATION(40000, fillQueue(pq, 40000));
    TIME_OPERATION(40000, emptyQueue(pq, 40000));
}

