// This file 'sorting.cpp' contains code regarding the quicksort operation on a linked list structure, along with relevant helper functions
#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function takes 2 ListNodes 'a' and 'b' and pushes the node 'b' at the end of 'a'
 */
void concatenate(ListNode*& a, ListNode* b) {
    if (a == nullptr) { // assuming a is a nullptr, we will simply change a's pointer to b since it is passed by reference
        a = b;
        return;
    }
    ListNode* ptr = a;
    while (ptr->next != nullptr) {
        ptr = ptr->next; // traverse to the end of a's list.
    }
    ptr->next = b; // set a's end to b
}


/*
 * This function takes 2 ListNodes 'a' and 'b' and pushes the node 'b' at the front of 'a'
 */
void pushFront(ListNode*& a, ListNode* b) {
    ListNode* temp = a; // create a temporary pointer to a
    b->next = temp; // set b's next to the temporary pointer (essentially moving it to the front)
    a = b; // rewire a to point to b's beginning
}

/*
 * This function takes 4 ListNodes 'head', 'less', 'equal', and 'greater', and 1 integer 'pivot'.
 * By iterating through the whole linked list, it rewires the nodes into 'less', 'equal', and 'greater' based on relative value to 'pivot'.
 */
void partition(ListNode* head, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot) {
    while (head != nullptr) {
        ListNode* temp = head; // temporary pointer to our current node
        head = head->next; // set head to it's next value
        temp->next = nullptr; // then cut off the tie to temp by setting temp's next to nullptr
        if (temp->data < pivot) {
            pushFront(less, temp); // push to the front of 'less'
        } else if (temp->data == pivot) {
            pushFront(equal, temp); // push to the front of 'equal'
        } else {
            pushFront(greater, temp); // push to the front of 'greater'
        }
    }
}

/*
 * This function takes one argument 'front' and recursively sorts the linked list using the 'quicksort' algorithm.
 */
void quickSort(ListNode*& front) {
    if (front == nullptr) {
        return; // base case
    }

    ListNode* less = nullptr;
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;
    int pivot = front->data;
    partition(front, less, equal, greater, pivot); // allocate nodes to 'less', 'equal', and 'greater'.

    quickSort(less); // recursively sort the lesser list
    quickSort(greater); // recursively sort the greater list

    concatenate(less, equal); // combine the lower and equal lists
    concatenate(less, greater); // combine the new lower, and greater lists.

    front = less; // switch the head to the new sorted head.
}


/* * * * * * Test Code Below This Point * * * * * */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   while (front != nullptr) {
       cout << front->data;
       if (front->next != nullptr){
           cout << ", ";
       }
       front = front->next;
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    while (front != nullptr) {
        ListNode* temp = front;
        front = front -> next;
        delete temp;
    }
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in
 * the same order. It can be used to confirm a linked list's contents
 * match the expected.
 */
ListNode* createList(Vector<int> values){
    ListNode* val = new ListNode;
    ListNode* head = val;
    for (int i = 0; i < values.size(); i++) {
        val->data = values[i];
        if (i == values.size() - 1) {
            val -> next = nullptr;
        } else {
            val->next = new ListNode;
        }
        val = val->next;
    }
    return head;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    int index = 0;
    while (front != nullptr) {
        if (index > v.size() - 1) {
            return false;
        }
        if (front->data != v[index]) {
            return false;
        }
        front = front->next;
        index++;
    }
    if (index != v.size()) return false;
    return true;
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Stress test validity"){
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }
        v.sort();
        quickSort(list);

        EXPECT(areEquivalent(list, v));

        deallocateList(list);
    }
}

STUDENT_TEST("Custom timing trials"){
    int startSize = 50000;

    for(int n = startSize; n < 60*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }
        TIME_OPERATION(n, quickSort(list));

        deallocateList(list);
    }
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}
