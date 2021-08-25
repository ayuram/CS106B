// This file 'huffman.cpp' contains code relating to binary trees and the 'Huffman Compression' algorithm
#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

// Headers for helper functions
// function to combine 2 queues
void combineQueues(Queue<Bit>& a, Queue<Bit>& b);

// function to traverse a tree given a bit
EncodingTreeNode* traverse(Bit bit, EncodingTreeNode* root);

/*
 * This function takes 2 arguments 'bit' and an EncodingTreeNode pointer 'root', and returns the EncodingTreeNode pointer that would result in the respective traversal of '0' or '1' bit
 */
EncodingTreeNode* traverse(Bit bit, EncodingTreeNode* root) {
    if (root == nullptr) error("Cannot traverse a null tree");
    if (root->isLeaf()) error("Cannot traverse a leaf node");
    if (bit == 0) {
        return root->zero;
    } else {
        return root->one;
    }
}

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * The function iteratively follows the message bits to traverse the tree and generate an encoded string
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string value = "";
    EncodingTreeNode* next = tree;
    while (!messageBits.isEmpty()) {
        Bit bit = messageBits.dequeue();
        next = traverse(bit, next); // traverse to the corresponding node
        if (next == nullptr) {
            error ("invalid sequence");
        }
        if (next->isLeaf()) {
            value.push_back(next->getChar()); // append the character found at the leaf to the end of our string
            next = tree; // return back to the root of the tree
        }
    }
    return value;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * This function employs a recursive implementation (and likely the only natural implementation to deal with trees)
 * for converting a queue of bits and a queue of characters into the original tree
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (!treeShape.isEmpty() && !treeLeaves.isEmpty()) {
        Bit curr = treeShape.dequeue();
        if (curr == 0) { // curr being 0 implies we have hit a leaf node
            return new EncodingTreeNode(treeLeaves.dequeue()); // return leaf node
        } else {
            return new EncodingTreeNode(unflattenTree(treeShape, treeLeaves), unflattenTree(treeShape, treeLeaves)); // recursive call to return a new tree
        }
    }
    return nullptr; // if the queue is empty we know we reached the end
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * This function makes use of existing functions, namely 'unflattenTree', 'decodeText', and 'deallocateTree'
 * to return the decompressed string from the encoded data
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* tree = unflattenTree(data.treeShape, data.treeLeaves); // make a tree so we can easily traverse the data
    string val = decodeText(tree, data.messageBits); // use the existing 'decodeText' function to deal with the tree and bits
    deallocateTree(tree); // no memory leaks in this code :)
    return val;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * This function makes use of the data structures 'Map' and 'PriorityQueue' to implement the 'Huffman Compression' algorithm.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    PriorityQueue<EncodingTreeNode*> pqueue; // huffman priority queue
    Map<char, int> frequencyTable; // map to keep track of the frequency of characters
    for (char c : text) {
        frequencyTable[c]++; // increase the frequency of whatever character we found
    }
    for (char c : frequencyTable.keys()) {
        pqueue.enqueue(new EncodingTreeNode(c), frequencyTable[c]); // enqueue the characters as leaf nodes into the priority queue
    }
    while (pqueue.size() > 1) { // keep iterating until we just have 1 element left in the queue
        int leftP = pqueue.peekPriority(); // left branch's priority
        EncodingTreeNode* left = pqueue.dequeue(); // left node
        int rightP = pqueue.peekPriority(); // right branch's priority
        EncodingTreeNode* right = pqueue.dequeue(); // right node
        pqueue.enqueue(new EncodingTreeNode(left, right), leftP + rightP); // enqueue the new tree node with its new priority
    }
    if (pqueue.isEmpty()) return nullptr; // edge case for if the text was empty to begin with
    return pqueue.dequeue(); // return the last element in the queue, which should be the completed huffman tree
}


/*
 * This function takes 2 queues 'a' and 'b' and enquuees all elements of 'b' into 'a'
 */
void combineQueues(Queue<Bit>& a, Queue<Bit>& b) {
    while (!b.isEmpty()) {
        a.enqueue(b.dequeue());
    }
}
/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * This function makes use of iteration and Level-First-Search to fully encode the text into a queue of bits
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Queue<Bit> bits;
    for (char c : text) {
        Queue<Queue<Bit>> possiblePaths; // a queue of possible paths
        Queue<EncodingTreeNode*> toCheck; // a queue of tree nodes to check
        EncodingTreeNode* next = tree; // the current ndoe
        Queue<Bit> currPath; // the current path
        while (next) {
            if (next->zero) { // if the zero branch is not nullptr
                toCheck.enqueue(next->zero); // enqueue the left branch of the current node
                Queue<Bit> leftPath = currPath; // create a copy of the current path
                leftPath.enqueue(0); // enqueue '0' bit into the copied queue
                possiblePaths.enqueue(leftPath); // enqueue the copy into the possible paths
            }
            if (next->one) { // same deal but with the right branch
                toCheck.enqueue(next->one);
                Queue<Bit> rightPath = currPath;
                rightPath.enqueue(1);
                possiblePaths.enqueue(rightPath);
            }
            if(!possiblePaths.isEmpty()) currPath = possiblePaths.dequeue(); // our next path is dequeued from the 'possiblePaths' queue.
            if(!toCheck.isEmpty()) {
                next = toCheck.dequeue(); // our next node is dequeued from the 'toCheck' queue
            } else {
                next = nullptr;
            }
            if (next == nullptr) break; // break out of the loop if we are at a dead end
            if (next->isLeaf() && next->getChar() == c) {
                combineQueues(bits, currPath); // add our current path to the existing queue of bits
                break; // break out of the loop since we have found our path
            }
        }
    }
    return bits;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * This function is a recursive implementation of the 'flattenTree' algorithm,
 * which flattens each branch and modifies the 'treeShape' and 'treeLeaves' queues accordingly
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (tree == nullptr) return; // base case
    if (tree->isLeaf()) {
        treeShape.enqueue(0); // must enqueue '0' if we have found a leaf
        treeLeaves.enqueue(tree->getChar()); // also enqueue the character of the leaf
    } else {
        treeShape.enqueue(1); // enqueue '1' if we are not on a leaf node
    }
    flattenTree(tree->zero, treeShape, treeLeaves); // must start with the left branch for accurate implementation
    flattenTree(tree->one, treeShape, treeLeaves); // recursively flatten the right branch as well
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * This function makes use of other functions, namely 'buildHuffmanTree', 'flateenTree', and 'deallocateTree'
 */
EncodedData compress(string messageText) {
    EncodedData data; // the value we intend to fill up
    EncodingTreeNode* huffman = buildHuffmanTree(messageText); // create a huffman tree for fast traversals
    data.messageBits = encodeText(huffman, messageText); // fill up the message bits using 'encodeText'
    flattenTree(huffman, data.treeShape, data.treeLeaves); // flatten the tree to fill up 'treeShape' and 'treeLeaves'
    deallocateTree(huffman); // no memory leaks in this code :)
    return data;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* T = new EncodingTreeNode('T');
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* RS = new EncodingTreeNode(R, S);
    EncodingTreeNode* E = new EncodingTreeNode('E');
    EncodingTreeNode* RSE = new EncodingTreeNode(RS, E);
    EncodingTreeNode* node = new EncodingTreeNode(T, RSE);
    return node;
}

void deallocateTree(EncodingTreeNode* t) {
    if (t != nullptr) { // "base case"
        deallocateTree(t->zero); // recursive call to delete left
        deallocateTree(t->one); // recursive call to delete right
        delete t; // delete head
    }
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (a == nullptr || b == nullptr) {
        return a == b; // Both being null means they are equal
    }

    bool canRecurse = true; // I love debug mode (this bool is needed as a check for whether the leaves are equal, if we are at a leaf)
    if (a->isLeaf() && b->isLeaf()) {
        canRecurse = a->ch == b->ch;
    }


    // recursive calls to check equality of the left and right branches, and the characters within their leaves
    return canRecurse
            && areEqual(a->zero, b->zero)
            && areEqual(a->one, b->one);
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Test compression on empty string") {
    EXPECT_NO_ERROR(compress(""));
}

STUDENT_TEST("Decode Text Edge Cases") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    // decodeText on empty queue

    Queue<Bit> messageBits = {}; // empty
    EXPECT_EQUAL(decodeText(tree, messageBits), "");

    // decodeText on null tree

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_ERROR(decodeText(nullptr, messageBits));

    deallocateTree(tree);
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
