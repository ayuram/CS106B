#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
using namespace std;


// This function takes one string 's' and returns a cleaned token version of that string.
string cleanToken(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i])) {
            s.erase(i, 1);
            i--;
        } else {
            break;
        }
    }
    for (int i = s.length(); i > 0; i--) {
        if(!isalpha(s[i])) {
            s.erase(i, 1);
        } else {
            break;
        }
    }
    return toLowerCase(s);
}

// This function takes a string 'text' and returns the set of cleaned tokens in that string
Set<string> gatherTokens(string text) {
    Set<string> tokens;
    for (string token : stringSplit(text, " ")) {
        string cleanedToken = cleanToken(token);
        if (!cleanedToken.empty()) {
            tokens.add(cleanedToken);
        }
    }
    return tokens;
}

// The function takes 2 arguments : a filename and a map 'index' which is passed by reference.
// It scans the dbfile for lines and fills the index with an inverted word index of that file.
// Finally, the function returns an integer value for the number of documents scanned.
int buildIndex(string dbfile, Map<string, Set<string>>& index) {
    ifstream in;
    if (!openFile(in, dbfile)) {
        error("Cannot open file named " + dbfile);
    }
    Vector<string> lines;
    readEntireFile(in, lines);
    for (int i = 0; i < lines.size(); i+=2) {
        Set<string> tokens = gatherTokens(lines[i + 1]);
        for (string token : tokens) {
          index[token].add(lines[i]);
        }
    }
    return lines.size()/2;
}

// This function takes 2 arguments, a map called 'index' and a string called 'sentence'.
// The sentence is split up into individual search terms by spaces and the terms are then input as keys to check for in the index.
// The sentence can use '+' or '-' right before the start of each query to indicate whether they want the term to be inclusive or exclusive in the search.
// Lastly, the function returns a set of strings indicating the matches to the overall query.
Set<string> findQueryMatches(Map<string, Set<string>>& index, string sentence)
{
    Set<string> result;
    Vector<string> terms = stringSplit(sentence, ' ');
    for (int i = 0; i < terms.size(); i++) {
        Set<string> partial = index[cleanToken(terms[i])];
        switch (terms[i][0]) {
            case '+' :
                result *= partial; // inclusive
                break;
            case '-' :
                result -= partial; // exclusive
                break;
            default:
                result += partial; // just combines the two
                break;
        }
    }
    return result;
}

// This function takes one argument 'dbfile' which is a string that references an actual file location
// The function scans the contents of the file, and prompts the user for search queries indefinitely, and prints out the query results using 'findQueryMatches()'.
// The user can enter an empty string to break out of the loop.
void searchEngine(string dbfile)
{
    std::cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> index;

    std::cout << "Indexed " << buildIndex(dbfile, index) << " pages containing " << index.keys().size() << " unique terms" << endl;
    while (true) {
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if(query.empty()) {
            break;
        }
        Set<string> matches = findQueryMatches(index, query);
        std::cout << "Found " << matches.size() << " matching pages" << endl;
        for(string result : matches) {
            std::cout << result << endl;
        }
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> tokens = gatherTokens("to be or not to be");
    EXPECT_EQUAL(tokens.size(), 4);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> seuss = gatherTokens("One Fish Two Fish Red fish Blue fish!!! 123");
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 20 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


STUDENT_TEST("Clean token extreme testing") {
    EXPECT_EQUAL(cleanToken("!!---,,,,,!!!!!!WORLD!!!!!,,,,,....."), "world");
    EXPECT_EQUAL(cleanToken("!!---,,,,,!!!!!!WO!RLD!!!!!,,,,,....."), "wo!rld");
}
