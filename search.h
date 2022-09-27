// TODO: remove and replace this file header comment
// This is a .h file you will edit and turn in.

// Remove starter comments and add your own
// comments on each function and on complex code sections.

#pragma once

#include <iostream>
#include <sstream>
#include <set>
#include <map>
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string token) 
{
    while (ispunct(token[0])) {
        token.replace(0, 1, "");
    }

    while (ispunct(token[token.size() - 1])) {
        token.replace(token.size()-1, 1, "");
    }

    int i = 0;
    bool containsALetter = false;
    while (token[i]) {
        if (!containsALetter && isalpha(token[i])) {
            containsALetter = true;
        }
        token[i] = tolower(token[i]);
        i++;
    }

    if (containsALetter) {
        return token;
    }
    
    return "";
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
set<string> gatherTokens(string text) 
{
    set<string> tokens;
    
    // tokenize into individual strings by spaces
    string tempString;
    stringstream myStringStream (text);
    while (getline (myStringStream, tempString, ' ')) {
        tempString = cleanToken(tempString);
        if (tempString != "") {
            tokens.insert(tempString);      // store cleaned non-empty tokens in set
        }
    }

    return tokens;
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string filename, map<string, set<string>>& index) {
    
    
    
    // return the count of web pages that were processed from the file and added to the index
    return 0;
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    
    
    // TODO:  Write this function.
    
    
    return result;  // TODO:  update this.
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string filename) {

    
    // TODO:  Write this function.
    
    
}


