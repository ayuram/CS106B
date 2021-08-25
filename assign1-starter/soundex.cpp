/*
 * This file contains the functions necessary to convert surnames into soundex codes.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it.
 */
string removeNonLetters(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i])) {
            // remove the character at index i
            s = s.erase(i, 1);
            i--;
        }
    }
    return s;
}

/* This function takes a character and returns
 * the soundex associated digit as a character.
 */
char getSoundexCharacter(char c){
    if(c == 'B' || c == 'F' || c == 'P' || c == 'V') {
        return '1';
    } else if (c == 'C' || c == 'G' || c == 'J' || c == 'K' || c == 'Q' || c == 'S' || c == 'X' || c == 'Z'){
        return '2';
    } else if (c == 'D' || c == 'T') {
        return '3';
    } else if(c == 'L') {
        return '4';
    } else if (c == 'M' || c == 'N') {
        return '5';
    } else if (c == 'R'){
        return '6';
    } else {
        return '0';
    }
}

/* This function takes a string and returns
 * the soundex encoded version of that string
 */
string soundex(string s) {
    s = removeNonLetters(trim(toUpperCase(s)));
    string val = "";
    char lastChar = 'x';
    for(char c : s){ // convert all characters in the string to the soundex digits
        if(lastChar != getSoundexCharacter(c)){
           val += getSoundexCharacter(c);
       }
       lastChar = getSoundexCharacter(c);
    }
    val[0] = s[0]; // change the initial character of the returned string to the initial character of the entered string
    for(int i = 0; i < val.length(); i++){ // remove all '0's from the string
        if(val[i] == '0'){
            val.erase(i, 1); // remove character at index 'i';
            i--;
        }
    }
    if(val.length() > 4){ // fill in or truncate the string to 4 characters
        val = val.substr(0, 4);
    } else {
        int length = val.length();
        for(int i = 0; i < 4 - length; i++){
            val += '0';
        }
    }
    return val;
}


/*
 * This functions accepts a filepath and then prompts the user infinitely many times
 *  for a surname to query the databse, and then console logs
 *  the list of names with the same soundex code
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    while(true){
        string surname = getLine("Enter your surname : ");
        string coded = soundex(surname);
        Vector<string> newVector;
        for(string name : databaseNames){
            if(soundex(name) == coded){
                newVector.add(name);
            }
        }
        newVector.sort();
        for(string name : newVector){
            cout << name << endl;
        }
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

STUDENT_TEST("My name") {
    EXPECT_EQUAL(soundex("Raman"), "R550");
}

